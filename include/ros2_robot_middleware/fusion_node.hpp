#ifndef ROS2_ROBOT_MIDDLEWARE_FUSION_NODE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_FUSION_NODE_HPP_

#include "ros2_robot_middleware/msg/camera_image.hpp"
#include "ros2_robot_middleware/msg/imu_data.hpp"
#include "ros2_robot_middleware/msg/lidar_scan.hpp"
#include "ros2_robot_middleware/msg/perception_objects.hpp"
#include "std_msgs/msg/string.hpp"

#include <rclcpp/rclcpp.hpp>

class FusionNode : public rclcpp::Node {
public:
    FusionNode();

private:
    void timer_callback();

    void lidar_callback(const ros2_robot_middleware::msg::LidarScan::SharedPtr &msg);
    void imu_callback(const ros2_robot_middleware::msg::ImuData::SharedPtr &msg);
    void camera_callback(const ros2_robot_middleware::msg::CameraImage::SharedPtr &msg);

    rclcpp::Publisher<ros2_robot_middleware::msg::PerceptionObjects>::SharedPtr fusion_pub_;

    rclcpp::Subscription<ros2_robot_middleware::msg::LidarScan>::SharedPtr sub_lidar_;
    rclcpp::Subscription<ros2_robot_middleware::msg::ImuData>::SharedPtr sub_imu_;
    rclcpp::Subscription<ros2_robot_middleware::msg::CameraImage>::SharedPtr sub_camera_;

    ros2_robot_middleware::msg::LidarScan::SharedPtr lidar_cache_;
    ros2_robot_middleware::msg::ImuData::SharedPtr imu_cache_;
    ros2_robot_middleware::msg::CameraImage::SharedPtr camera_cache_;

    rclcpp::TimerBase::SharedPtr timer_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr heartbeat_pub_;
    rclcpp::TimerBase::SharedPtr heartbeat_timer_;
};

#endif // ROS2_ROBOT_MIDDLEWARE_FUSION_NODE_HPP_
