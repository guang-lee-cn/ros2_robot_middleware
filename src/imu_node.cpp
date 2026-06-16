#include "ros2_robot_middleware/imu_node.hpp"

ImuNode::ImuNode() : Node("imu") {
    publisher_ = this->create_publisher<ros2_robot_middleware::msg::ImuData>("/sensor/imu", rclcpp::QoS(10).reliable());

    using namespace std::chrono_literals;
    timer_ = this->create_wall_timer(10ms, [this]() { timer_callback(); });
}

void ImuNode::timer_callback() {
    auto msg = ros2_robot_middleware::msg::ImuData{};

    msg.header.stamp    = this->now();
    msg.header.frame_id = "imu_link"; // todo:这个名称是如何确认的？

    constexpr char X = 0;
    constexpr char Y = 1;
    constexpr char Z = 2;

    msg.angular_velocity[X]    = (rand() % 2001 - 1000) / 50000.0F;
    msg.angular_velocity[Y]    = (rand() % 2002 - 1000) / 50000.0F;
    msg.angular_velocity[Z]    = (rand() % 2003 - 1000) / 50000.0F;
    msg.linear_acceleration[X] = (rand() % 2001 - 1000) / 10000.0F;
    msg.linear_acceleration[Y] = (rand() % 2002 - 1000) / 10000.0F;
    msg.linear_acceleration[Z] = (rand() % 2003 - 1000) / 10000.0F;

    publisher_->publish(msg);
}
