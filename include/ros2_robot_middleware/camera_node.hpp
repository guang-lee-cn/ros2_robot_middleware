#ifndef ROS2_ROBOT_MIDDLEWARE_CAMERA_NODE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_CAMERA_NODE_HPP_

#include <rclcpp/rclcpp.hpp>

#include "ros2_robot_middleware/msg/camera_image.hpp"

class CameraNode : public rclcpp::Node {
public:
    CameraNode();

private:
    void timer_callback();

    rclcpp::Publisher<ros2_robot_middleware::msg::CameraImage>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

#endif // ROS2_ROBOT_MIDDLEWARE_CAMERA_NODE_HPP_
