#include "ros2_robot_middleware/camera_node.hpp"

CameraNode::CameraNode() : Node("camera") {
    publisher_ = this->create_publisher<ros2_robot_middleware::msg::CameraImage>(
        "/sensor/camera",
        rclcpp::QoS(10).best_effort()); // todo: 这里的数据要保留多少条？

    using namespace std::chrono_literals;
    timer_ = this->create_wall_timer(200ms, [this]() { timer_callback(); });
}

void CameraNode::timer_callback() {
    auto msg = ros2_robot_middleware::msg::CameraImage{};

    msg.header.stamp    = this->now();
    msg.header.frame_id = "camera_link"; // todo:这个名称是如何确认的？

    msg.height   = 480;
    msg.width    = 640;
    msg.step     = 640 * 3; // 每行字节数 = width × channels
    msg.encoding = "rgb8";
    msg.data.resize(480 * 640 * 3);
    // 填充随机像素
    for (auto &byte : msg.data) {
        byte = rand() % 256;
    }

    publisher_->publish(msg);

    RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 10000,
                         "CameraImage published: %dx%d %s %.1fKB",
                         msg.width, msg.height, msg.encoding.c_str(),
                         msg.data.size() / 1024.0);
}
