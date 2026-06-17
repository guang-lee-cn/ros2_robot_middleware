#ifndef ROS2_ROBOT_MIDDLEWARE_LIDAR_NODE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_LIDAR_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/string.hpp"

#include "ros2_robot_middleware/msg/lidar_scan.hpp"

class LidarNode : public rclcpp::Node {
public:
    LidarNode();

private:
    void timer_callback();

    rclcpp::Publisher<ros2_robot_middleware::msg::LidarScan>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr heartbeat_pub_;
    rclcpp::TimerBase::SharedPtr heartbeat_timer_;
};

#endif // ROS2_ROBOT_MIDDLEWARE_LIDAR_NODE_HPP_
