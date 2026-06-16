#include "ros2_robot_middleware/fusion_node.hpp"

#include <cmath>

FusionNode::FusionNode()
    : Node("fusion")
{
  auto qos_best_effort = rclcpp::QoS(10).best_effort();
  auto qos_reliable    = rclcpp::QoS(10).reliable();

  sub_lidar_  = this->create_subscription<ros2_robot_middleware::msg::LidarScan>(
      "/sensor/lidar", qos_best_effort,
      [this](ros2_robot_middleware::msg::LidarScan::SharedPtr msg) { lidar_callback(msg); });

  sub_imu_    = this->create_subscription<ros2_robot_middleware::msg::ImuData>(
      "/sensor/imu", qos_reliable,
      [this](ros2_robot_middleware::msg::ImuData::SharedPtr msg) { imu_callback(msg); });

  sub_camera_ = this->create_subscription<ros2_robot_middleware::msg::CameraImage>(
      "/sensor/camera", qos_best_effort,
      [this](ros2_robot_middleware::msg::CameraImage::SharedPtr msg) { camera_callback(msg); });

  fusion_pub_ = this->create_publisher<ros2_robot_middleware::msg::PerceptionObjects>(
      "/perception/objects", rclcpp::QoS(10).reliable());

  using namespace std::chrono_literals;
  timer_ = this->create_wall_timer(200ms, [this]() { timer_callback(); });
}

// ---------------------------------------------------------------------------
// Subscriber callbacks — update cache
// ---------------------------------------------------------------------------

void FusionNode::lidar_callback(
    const ros2_robot_middleware::msg::LidarScan::SharedPtr& msg)
{
  lidar_cache_ = msg;
}

void FusionNode::imu_callback(
    const ros2_robot_middleware::msg::ImuData::SharedPtr& msg)
{
  imu_cache_ = msg;
}

void FusionNode::camera_callback(
    const ros2_robot_middleware::msg::CameraImage::SharedPtr& msg)
{
  camera_cache_ = msg;
}

// ---------------------------------------------------------------------------
// Timer callback — fuse sensor data into perception output
// ---------------------------------------------------------------------------

void FusionNode::timer_callback()
{
  // Wait until all three sensors have delivered at least one frame
  if (!lidar_cache_ || !imu_cache_ || !camera_cache_) {
    return;
  }

  auto msg = ros2_robot_middleware::msg::PerceptionObjects{};
  msg.header.stamp    = this->now();
  msg.header.frame_id = "base_link";

  // Simplified object extraction: scan lidar ranges for consecutive "hits"
  // below a distance threshold and cluster them into objects
  constexpr int    kNumPoints     = 360;
  constexpr int    kMaxObjects    = 5;
  constexpr float  kRangeThreshold = 3.0F;  // objects within 3 m
  constexpr float  kClusterGap     = 5;      // max angular gap (in bins) within a cluster

  int start = -1;
  for (int i = 0; i < kNumPoints && static_cast<int>(msg.objects.size()) < kMaxObjects; ++i) {
    bool hit = (lidar_cache_->ranges[i] > 0.1F && lidar_cache_->ranges[i] < kRangeThreshold);

    if (hit && start < 0) {
      start = i;  // cluster begin
    } else if (!hit && start >= 0) {
      if (i - start > kClusterGap) {
        // close cluster, emit an object at the midpoint angle
        int    mid     = (start + i) / 2;
        float  angle   = lidar_cache_->angle_min + mid * lidar_cache_->angle_increment;
        float  range   = lidar_cache_->ranges[mid];
        float  x       = range * std::cos(angle);
        float  y       = range * std::sin(angle);

        auto obj = ros2_robot_middleware::msg::Object{};
        obj.id = "obj_" + std::to_string(msg.objects.size());
        obj.x  = x;
        obj.y  = y;
        obj.z  = 0.0F;

        msg.objects.push_back(obj);
        start = -1;
      } else {
        start = -1;
      }
    }
  }

  fusion_pub_->publish(msg);
}
