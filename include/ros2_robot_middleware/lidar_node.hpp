#ifndef ROS2_ROBOT_MIDDLEWARE_LIDAR_NODE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_LIDAR_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include "std_msgs/msg/string.hpp"

#include "ros2_robot_middleware/msg/lidar_scan.hpp"

class LidarNode : public rclcpp_lifecycle::LifecycleNode {
public:
  LidarNode();

  using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

  CallbackReturn on_configure(const rclcpp_lifecycle::State &);
  CallbackReturn on_activate(const rclcpp_lifecycle::State &);
  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &);
  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &);

private:
  void timer_callback();

  rclcpp_lifecycle::LifecyclePublisher<ros2_robot_middleware::msg::LidarScan>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>::SharedPtr heartbeat_pub_;
  rclcpp::TimerBase::SharedPtr heartbeat_timer_;
};

#endif  // ROS2_ROBOT_MIDDLEWARE_LIDAR_NODE_HPP_
