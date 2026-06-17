#include "ros2_robot_middleware/lidar_node.hpp"

#include <cmath>

LidarNode::LidarNode()
    : Node("lidar")
{
  publisher_ = this->create_publisher<ros2_robot_middleware::msg::LidarScan>(
      "/sensor/lidar", rclcpp::QoS(10).best_effort());

  using namespace std::chrono_literals;
  timer_ = this->create_wall_timer(100ms, [this]() { timer_callback(); });
}

void LidarNode::timer_callback()
{
  auto msg = ros2_robot_middleware::msg::LidarScan{};

  msg.header.stamp    = this->now();
  msg.header.frame_id = "lidar_frame";

  constexpr int    kNumPoints      = 360;
  constexpr float  kAngleMin       = -M_PI;
  constexpr float  kAngleMax       = M_PI;
  constexpr float  kAngleIncrement = (kAngleMax - kAngleMin) / kNumPoints;
  constexpr float  kTimeIncrement  = 0.0001F;  // 0.1 ms per point (SICK TiM781)

  msg.angle_min       = kAngleMin;
  msg.angle_max       = kAngleMax;
  msg.angle_increment = kAngleIncrement;
  msg.time_increment  = kTimeIncrement;

  msg.ranges.resize(kNumPoints);
  msg.intensities.resize(kNumPoints);

  for (int i = 0; i < kNumPoints; ++i) {
    float angle = kAngleMin + i * kAngleIncrement;

    // Simulated range: base distance + sinusoidal variation
    msg.ranges[i] = 2.0F + 1.5F * std::sin(angle * 3.0F) * std::cos(angle * 2.0F);

    // Simulated intensity: inversely proportional to range
    msg.intensities[i] = 1.0F - msg.ranges[i] / 10.0F;
  }

  publisher_->publish(msg);

  RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 5000,
                       "LidarScan published: ranges[0..359] avg=%.2fm",
                       msg.ranges[0]);
}
