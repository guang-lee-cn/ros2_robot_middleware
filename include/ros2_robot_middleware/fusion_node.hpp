#ifndef ROS2_ROBOT_MIDDLEWARE_FUSION_NODE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_FUSION_NODE_HPP_

#include <cstdint>

#include "ros2_robot_middleware/kalman_filter.hpp"
#include "ros2_robot_middleware/msg/perception_objects.hpp"
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <std_msgs/msg/string.hpp>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>

class FusionNode : public rclcpp_lifecycle::LifecycleNode {
public:
  FusionNode();

  using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

  CallbackReturn on_configure(const rclcpp_lifecycle::State &);
  CallbackReturn on_activate(const rclcpp_lifecycle::State &);
  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &);
  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &);

  // 暴露降级状态给测试
  enum class DegradationLevel : uint8_t {
    FULL        = 0,  // 全部传感器正常
    NO_LIDAR    = 1,  // LiDAR 缺失，Camera 降级兜底
    NO_CAMERA   = 2,  // Camera 缺失，纯 LiDAR 聚类
    NO_IMU      = 3,  // IMU 缺失，无运动补偿
    CRITICAL    = 4,  // 仅剩一个传感器或全挂
  };

  DegradationLevel degradation_level() const { return degradation_; }

  explicit FusionNode(const rclcpp::NodeOptions &options);

private:
  void timer_callback();
  void update_heartbeat_status();

  void lidar_callback(sensor_msgs::msg::LaserScan::SharedPtr msg);
  void imu_callback(sensor_msgs::msg::Imu::SharedPtr msg);
  void camera_callback(sensor_msgs::msg::Image::SharedPtr msg);

  // 各降级路径的融合逻辑
  void fuse_full(ros2_robot_middleware::msg::PerceptionObjects &output);
  void fuse_no_lidar(ros2_robot_middleware::msg::PerceptionObjects &output);
  void fuse_no_camera(ros2_robot_middleware::msg::PerceptionObjects &output);
  void fuse_no_imu(ros2_robot_middleware::msg::PerceptionObjects &output);

  // 传感器数据新鲜度检查
  bool is_stale(rclcpp::Time last_update, double timeout_s) const;
  DegradationLevel evaluate_degradation() const;

  rclcpp::Time lidar_stamp_;
  rclcpp::Time imu_stamp_;
  rclcpp::Time camera_stamp_;

  static constexpr double kLidarTimeout  = 1.5;
  static constexpr double kImuTimeout    = 0.5;
  static constexpr double kCameraTimeout = 3.0;

  DegradationLevel degradation_ = DegradationLevel::FULL;

  rclcpp_lifecycle::LifecyclePublisher<ros2_robot_middleware::msg::PerceptionObjects>::SharedPtr fusion_pub_;

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr sub_lidar_;
  rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr sub_imu_;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_camera_;

  sensor_msgs::msg::LaserScan::SharedPtr lidar_cache_;
  sensor_msgs::msg::Imu::SharedPtr imu_cache_;
  sensor_msgs::msg::Image::SharedPtr camera_cache_;

  rclcpp::TimerBase::SharedPtr timer_;

  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>::SharedPtr heartbeat_pub_;
  KalmanFilter2D kf_;
  rclcpp::Time last_kf_predict_;

  rclcpp::TimerBase::SharedPtr heartbeat_timer_;
};

#endif  // ROS2_ROBOT_MIDDLEWARE_FUSION_NODE_HPP_
