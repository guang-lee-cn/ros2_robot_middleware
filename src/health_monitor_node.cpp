#include "ros2_robot_middleware/health_monitor_node.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <sstream>
#include <thread>

static constexpr double kWarnRatio = 0.8;

HealthMonitorNode::HealthMonitorNode()
  : rclcpp_lifecycle::LifecycleNode("health_monitor")
{
}

HealthMonitorNode::CallbackReturn
HealthMonitorNode::on_configure(const rclcpp_lifecycle::State &)
{
  declare_parameters();
  load_parameters();
  create_subscriptions();
  create_report_publisher();
  create_service_server();
  create_restart_clients();

  RCLCPP_INFO(this->get_logger(),
              "HealthMonitor configured: %d nodes, %.1fs interval",
              kNumNodes, check_interval_s_);

  return CallbackReturn::SUCCESS;
}

HealthMonitorNode::CallbackReturn
HealthMonitorNode::on_activate(const rclcpp_lifecycle::State &)
{
  create_health_timer();
  setup_prometheus();

  pub_->on_activate();

  RCLCPP_INFO(this->get_logger(),
              "HealthMonitor activated: Prometheus on :%d/metrics",
              kPrometheusPort);

  return CallbackReturn::SUCCESS;
}

HealthMonitorNode::CallbackReturn
HealthMonitorNode::on_deactivate(const rclcpp_lifecycle::State &)
{
  timer_.reset();

  pub_->on_deactivate();

  if (prom_socket_ >= 0) {
    ::shutdown(prom_socket_, SHUT_RDWR);
    close(prom_socket_);
    prom_socket_ = -1;
  }
  if (prom_thread_.joinable()) {
    prom_thread_.join();
  }

  return CallbackReturn::SUCCESS;
}

HealthMonitorNode::CallbackReturn
HealthMonitorNode::on_cleanup(const rclcpp_lifecycle::State &)
{
  for (int i = 0; i < kNumNodes; ++i) {
    subs_[i].reset();
  }
  pub_.reset();
  health_srv_.reset();

  last_seen_.clear();
  timeouts_.clear();

  return CallbackReturn::SUCCESS;
}

HealthMonitorNode::CallbackReturn
HealthMonitorNode::on_shutdown(const rclcpp_lifecycle::State &)
{
  timer_.reset();

  if (prom_socket_ >= 0) {
    ::shutdown(prom_socket_, SHUT_RDWR);
    close(prom_socket_);
    prom_socket_ = -1;
  }
  if (prom_thread_.joinable()) {
    prom_thread_.join();
  }

  for (int i = 0; i < kNumNodes; ++i) {
    subs_[i].reset();
  }
  pub_.reset();
  health_srv_.reset();

  return CallbackReturn::SUCCESS;
}

void HealthMonitorNode::declare_parameters()
{
  this->declare_parameter<double>("check_interval_s", 1.0);
  for (const auto &cfg : kNdes) {
    std::string key = std::string(cfg.node) + "_timeout_s";
    this->declare_parameter<double>(key, cfg.default_timeout_s);
  }
}

void HealthMonitorNode::load_parameters()
{
  check_interval_s_ = this->get_parameter("check_interval_s").as_double();
  for (const auto &cfg : kNdes) {
    std::string key = std::string(cfg.node) + "_timeout_s";
    timeouts_[cfg.node] = this->get_parameter(key).as_double();
  }
}

void HealthMonitorNode::create_subscriptions()
{
  for (int i = 0; i < kNumNodes; ++i) {
    subs_[i] = this->create_subscription<std_msgs::msg::String>(
      kNdes[i].topic, rclcpp::QoS(10).reliable(),
      [this, node = std::string(kNdes[i].node)](std_msgs::msg::String::SharedPtr /*msg*/) {
        last_seen_[node] = this->now();
      });
  }
}

void HealthMonitorNode::create_health_timer()
{
  using namespace std::chrono_literals;
  auto period = std::chrono::milliseconds(
    static_cast<int>(check_interval_s_ * 1000));
  timer_ = this->create_wall_timer(period, [this]() { check_health(); });
}

void HealthMonitorNode::check_health()
{
  auto report = ros2_robot_middleware::msg::HealthReport{};
  report.header.stamp = this->now();
  report.header.frame_id = "health_monitor";

  const auto now = this->now();

  for (const auto &cfg : kNdes) {
    auto status = ros2_robot_middleware::msg::HealthStatus{};
    status.node_name = cfg.node;
    status.timeout_s = timeouts_[cfg.node];

    auto it = last_seen_.find(cfg.node);
    if (it == last_seen_.end()) {
      status.status = "STALE";
      status.last_seen_s = -1.0;
    } else {
      status.last_seen_s = (now - it->second).seconds();
      if (status.last_seen_s > status.timeout_s) {
        status.status = "ERROR";
      } else if (status.last_seen_s > status.timeout_s * kWarnRatio) {
        status.status = "WARN";
      } else {
        status.status = "OK";
      }
    }

    report.nodes.push_back(status);

    if (status.status == "ERROR") {
      // 看门狗恢复：尝试通过 lifecycle service 重启故障节点
      auto &recovery = recovery_[cfg.node];
      if (recovery.attempts < kMaxRestartRetries) {
        RCLCPP_ERROR_THROTTLE(this->get_logger(), *this->get_clock(), 5000,
                              "[%s] ERROR (attempt %d/%d): triggering restart",
                              cfg.node, recovery.attempts + 1, kMaxRestartRetries);
        try_restart_sequence(cfg.node);
        recovery.attempts++;
      } else {
        RCLCPP_ERROR(this->get_logger(), "[%s] FATAL: %d restart attempts failed",
                     cfg.node, kMaxRestartRetries);
        status.status = "FATAL";
      }
    } else if (status.status == "OK") {
      // 节点恢复后重置尝试计数
      recovery_[cfg.node].attempts = 0;
    } else if (status.status == "STALE") {
      RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 5000,
                           "[%s] STALE: no data received yet", cfg.node);
    }
  }

  pub_->publish(report);
}

void HealthMonitorNode::create_service_server()
{
  health_srv_ = this->create_service<ros2_robot_middleware::srv::SetParam>(
    "/health/check",
    [this](const std::shared_ptr<ros2_robot_middleware::srv::SetParam::Request> req,
           std::shared_ptr<ros2_robot_middleware::srv::SetParam::Response> resp) {
      auto it = last_seen_.find(req->param_name);
      if (it == last_seen_.end()) {
        resp->success = false;
        resp->message = "Unknown node: " + req->param_name;
        return;
      }
      double elapsed = (this->now() - it->second).seconds();
      double timeout = timeouts_[req->param_name];
      if (elapsed > timeout) {
        resp->success = false;
        resp->message = "ERROR: " + std::to_string(elapsed) + "s";
      } else if (elapsed > timeout * kWarnRatio) {
        resp->success = true;
        resp->message = "WARN: " + std::to_string(elapsed) + "s";
      } else {
        resp->success = true;
        resp->message = "OK: " + std::to_string(elapsed) + "s";
      }
    });
}

void HealthMonitorNode::create_report_publisher()
{
  pub_ = this->create_publisher<ros2_robot_middleware::msg::HealthReport>(
    "/health/report", rclcpp::QoS(10).reliable());
}

void HealthMonitorNode::create_restart_clients()
{
  for (const auto &cfg : kNdes) {
    restart_clients_[cfg.node] =
      this->create_client<lifecycle_msgs::srv::ChangeState>(
        std::string(cfg.node) + "/change_state");
  }
}

bool HealthMonitorNode::try_restart_sequence(const std::string &node_name)
{
  auto it = restart_clients_.find(node_name);
  if (it == restart_clients_.end()) return false;

  auto &client = it->second;
  using Transition = lifecycle_msgs::msg::Transition;

  // 如果 service 不可达（节点进程已死），直接返回失败
  if (!client->wait_for_service(std::chrono::seconds(1))) {
    RCLCPP_WARN(this->get_logger(), "[%s] lifecycle service unreachable", node_name.c_str());
    return false;
  }

  // 重启序列: deactivate(4) → cleanup(2) → configure(1) → activate(3)
  // 和 5G 基站板卡重启流程同构：先停止 → 清理 → 重配 → 激活
  const std::array<std::pair<uint8_t, const char *>, 4> sequence = {{
    {Transition::TRANSITION_DEACTIVATE, "deactivate"},
    {Transition::TRANSITION_CLEANUP,    "cleanup"},
    {Transition::TRANSITION_CONFIGURE,  "configure"},
    {Transition::TRANSITION_ACTIVATE,   "activate"},
  }};

  for (const auto &[id, label] : sequence) {
    auto request = std::make_shared<lifecycle_msgs::srv::ChangeState::Request>();
    request->transition.id    = id;
    request->transition.label = label;

    auto future   = client->async_send_request(request);
    auto status   = future.wait_for(std::chrono::seconds(2));

    if (status != std::future_status::ready) {
      RCLCPP_WARN(this->get_logger(), "[%s] %s timed out", node_name.c_str(), label);
      return false;
    }

    auto response = future.get();
    if (!response->success) {
      RCLCPP_WARN(this->get_logger(), "[%s] %s rejected (wrong state?)",
                   node_name.c_str(), label);
      return false;
    }
  }

  RCLCPP_INFO(this->get_logger(), "[%s] restart sequence completed successfully",
               node_name.c_str());
  return true;
}

void HealthMonitorNode::setup_prometheus()
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    RCLCPP_WARN(this->get_logger(), "Prometheus socket creation failed");
    return;
  }

  int opt = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(kPrometheusPort);

  if (bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
    RCLCPP_WARN(this->get_logger(), "Prometheus bind :%d failed", kPrometheusPort);
    close(fd);
    return;
  }

  if (listen(fd, 5) < 0) {
    RCLCPP_WARN(this->get_logger(), "Prometheus listen failed");
    close(fd);
    return;
  }

  prom_socket_ = fd;

  prom_thread_ = std::thread([this]() {
    while (prom_socket_ >= 0) {
      prometheus_accept();
    }
  });
}

void HealthMonitorNode::prometheus_accept()
{
  sockaddr_in client{};
  socklen_t len = sizeof(client);
  int conn = accept(prom_socket_, reinterpret_cast<sockaddr *>(&client), &len);
  if (conn < 0) return;

  char buf[1024]{};
  ssize_t n = recv(conn, buf, sizeof(buf) - 1, 0);
  std::string body;

  if (n > 0) {
    std::string request(buf, n);
    if (request.find("GET /metrics") != std::string::npos) {
      std::string metrics = prometheus_metrics();
      body = "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/plain; version=0.0.4\r\n"
             "Content-Length: " + std::to_string(metrics.size()) + "\r\n"
             "Connection: close\r\n"
             "\r\n" + metrics;
    } else {
      body = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
    }
  }

  send(conn, body.data(), body.size(), MSG_NOSIGNAL);
  close(conn);
}

std::string HealthMonitorNode::prometheus_metrics() const
{
  std::ostringstream out;
  out << "# HELP ros2_node_health_seconds Seconds since last data from node\n";
  out << "# TYPE ros2_node_health_seconds gauge\n";

  for (const auto &cfg : kNdes) {
    auto it = last_seen_.find(cfg.node);
    double val = (it != last_seen_.end())
                   ? (this->now() - it->second).seconds()
                   : -1.0;
    out << "ros2_node_health_seconds{node=\"" << cfg.node << "\"} " << val << "\n";
  }

  out << "# HELP ros2_node_timeout_seconds Configured timeout\n";
  out << "# TYPE ros2_node_timeout_seconds gauge\n";
  for (const auto &cfg : kNdes) {
    out << "ros2_node_timeout_seconds{node=\"" << cfg.node << "\"} "
        << timeouts_.at(cfg.node) << "\n";
  }

  return out.str();
}
