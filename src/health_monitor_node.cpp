#include "ros2_robot_middleware/health_monitor_node.hpp"

#include "ros2_robot_middleware/msg/health_report.hpp"
#include "ros2_robot_middleware/msg/health_status.hpp"
#include "ros2_robot_middleware/srv/set_param.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <sstream>
#include <thread>

static constexpr double kWarnRatio = 0.8;

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
HealthMonitorNode::HealthMonitorNode()
  : Node("health_monitor")
{
  declare_parameters();
  load_parameters();
  create_subscriptions();
  create_health_timer();
  create_service_server();
  create_publisher();
  setup_prometheus();

  RCLCPP_INFO(this->get_logger(),
              "HealthMonitor started: %d nodes, %.1fs interval, Prometheus :%d",
              kNumNodes, check_interval_s_, kPrometheusPort);
}

// ---------------------------------------------------------------------------
// Parameters
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// Subscriptions — all nodes publish std_msgs/String heartbeat
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// Periodic health check
// ---------------------------------------------------------------------------
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
      RCLCPP_ERROR_THROTTLE(this->get_logger(), *this->get_clock(), 5000,
                            "[%s] ERROR: %.1fs since last data (limit %.1fs)",
                            cfg.node, status.last_seen_s, status.timeout_s);
    } else if (status.status == "STALE") {
      RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 5000,
                           "[%s] STALE: no data received yet", cfg.node);
    }
  }

  pub_->publish(report);
}

// ---------------------------------------------------------------------------
// Health query service
// ---------------------------------------------------------------------------
void HealthMonitorNode::create_service_server()
{
  auto srv = this->create_service<ros2_robot_middleware::srv::SetParam>(
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
  health_srv_ = srv;
}

// ---------------------------------------------------------------------------
// Publisher
// ---------------------------------------------------------------------------
void HealthMonitorNode::create_publisher()
{
  using MsgT = ros2_robot_middleware::msg::HealthReport;
  pub_ = rclcpp::Node::create_publisher<MsgT>(
    "/health/report", rclcpp::QoS(10).reliable());
}

// ---------------------------------------------------------------------------
// Prometheus metrics endpoint
// ---------------------------------------------------------------------------
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
  RCLCPP_INFO(this->get_logger(), "Prometheus metrics on :%d/metrics", kPrometheusPort);

  std::thread([this]() {
    while (rclcpp::ok() && prom_socket_ >= 0) {
      prometheus_accept();
    }
  }).detach();
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
