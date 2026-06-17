#include "ros2_robot_middleware/health_monitor_node.hpp"

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<HealthMonitorNode>());
  rclcpp::shutdown();
  return 0;
}
