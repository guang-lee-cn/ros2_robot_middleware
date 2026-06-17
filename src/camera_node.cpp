#include "ros2_robot_middleware/camera_node.hpp"

CameraNode::CameraNode()
  : rclcpp_lifecycle::LifecycleNode("camera")
{
}

CameraNode::CallbackReturn
CameraNode::on_configure(const rclcpp_lifecycle::State &)
{
  publisher_ = this->create_publisher<ros2_robot_middleware::msg::CameraImage>(
    "/sensor/camera", rclcpp::QoS(10).best_effort());

  heartbeat_pub_ = this->create_publisher<std_msgs::msg::String>(
    "/sensor/camera/heartbeat", rclcpp::QoS(10).reliable());

  return CallbackReturn::SUCCESS;
}

CameraNode::CallbackReturn
CameraNode::on_activate(const rclcpp_lifecycle::State &)
{
  using namespace std::chrono_literals;
  timer_ = this->create_wall_timer(200ms, [this]() { timer_callback(); });

  heartbeat_timer_ = this->create_wall_timer(1s, [this]() {
    auto msg = std_msgs::msg::String{};
    msg.data = "alive";
    heartbeat_pub_->publish(msg);
  });

  publisher_->on_activate();
  heartbeat_pub_->on_activate();

  return CallbackReturn::SUCCESS;
}

CameraNode::CallbackReturn
CameraNode::on_deactivate(const rclcpp_lifecycle::State &)
{
  timer_.reset();
  heartbeat_timer_.reset();

  publisher_->on_deactivate();
  heartbeat_pub_->on_deactivate();

  return CallbackReturn::SUCCESS;
}

CameraNode::CallbackReturn
CameraNode::on_cleanup(const rclcpp_lifecycle::State &)
{
  publisher_.reset();
  heartbeat_pub_.reset();

  return CallbackReturn::SUCCESS;
}

CameraNode::CallbackReturn
CameraNode::on_shutdown(const rclcpp_lifecycle::State &)
{
  timer_.reset();
  heartbeat_timer_.reset();
  publisher_.reset();
  heartbeat_pub_.reset();

  return CallbackReturn::SUCCESS;
}

void CameraNode::timer_callback()
{
  auto msg = ros2_robot_middleware::msg::CameraImage{};

  msg.header.stamp    = this->now();
  msg.header.frame_id = "camera_link";

  msg.height   = 480;
  msg.width    = 640;
  msg.step     = 640 * 3;
  msg.encoding = "rgb8";
  msg.data.resize(480 * 640 * 3);
  for (auto &byte : msg.data) {
    byte = rand() % 256;
  }

  publisher_->publish(msg);

  RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 10000,
                       "CameraImage published: %dx%d %s %.1fKB",
                       msg.width, msg.height, msg.encoding.c_str(),
                       msg.data.size() / 1024.0);
}
