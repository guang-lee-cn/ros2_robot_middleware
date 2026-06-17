#ifndef ROS2_ROBOT_MIDDLEWARE_HEALTH_MONITOR_NODE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_HEALTH_MONITOR_NODE_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "ros2_robot_middleware/msg/health_report.hpp"
#include "ros2_robot_middleware/msg/health_status.hpp"
#include "ros2_robot_middleware/srv/set_param.hpp"

class HealthMonitorNode : public rclcpp::Node {
public:
  HealthMonitorNode();

private:
  void declare_parameters();
  void load_parameters();
  void create_subscriptions();
  void create_health_timer();
  void create_service_server();
  void create_publisher();
  void setup_prometheus();
  void check_health();

  static constexpr int kNumNodes = 6;
  static constexpr int kPrometheusPort = 9090;

  struct NodeConfig {
    const char *node;
    const char *topic;
    double default_timeout_s;
  };
  static constexpr std::array<NodeConfig, kNumNodes> kNdes{{
    {"lidar",      "/sensor/lidar/heartbeat",     1.5},
    {"imu",        "/sensor/imu/heartbeat",       0.5},
    {"camera",     "/sensor/camera/heartbeat",    3.0},
    {"fusion",     "/sensor/fusion/heartbeat",    1.0},
    {"decision",   "/decision/heartbeat",         2.0},
    {"motor_ctrl", "/cmd/status",                 2.0},
  }};

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subs_[kNumNodes];
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<ros2_robot_middleware::msg::HealthReport>::SharedPtr pub_;
  rclcpp::Service<ros2_robot_middleware::srv::SetParam>::SharedPtr health_srv_;

  std::unordered_map<std::string, rclcpp::Time> last_seen_;
  std::unordered_map<std::string, double> timeouts_;
  double check_interval_s_ = 1.0;

  int prom_socket_ = -1;
  void prometheus_accept();
  std::string prometheus_metrics() const;
};

#endif  // ROS2_ROBOT_MIDDLEWARE_HEALTH_MONITOR_NODE_HPP_
