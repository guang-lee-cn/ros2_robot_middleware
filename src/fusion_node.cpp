#include "ros2_robot_middleware/fusion_node.hpp"

#include <cmath>

FusionNode::FusionNode() : rclcpp_lifecycle::LifecycleNode("fusion") {
}

// ── Lifecycle callbacks ──────────────────────────────────────────────────────

FusionNode::CallbackReturn FusionNode::on_configure(const rclcpp_lifecycle::State &) {
    auto qos_best_effort = rclcpp::QoS(10).best_effort();
    auto qos_reliable    = rclcpp::QoS(10).reliable();

    sub_lidar_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
        "/sensor/lidar", qos_best_effort,
        [this](sensor_msgs::msg::LaserScan::SharedPtr msg) { lidar_callback(msg); });

    sub_imu_ = this->create_subscription<sensor_msgs::msg::Imu>(
        "/sensor/imu", qos_reliable,
        [this](sensor_msgs::msg::Imu::SharedPtr msg) { imu_callback(msg); });

    sub_camera_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/sensor/camera", qos_best_effort,
        [this](sensor_msgs::msg::Image::SharedPtr msg) { camera_callback(msg); });

    fusion_pub_ = this->create_publisher<ros2_robot_middleware::msg::PerceptionObjects>(
        "/perception/objects", rclcpp::QoS(10).reliable());

    heartbeat_pub_ = this->create_publisher<std_msgs::msg::String>(
        "/sensor/fusion/heartbeat", rclcpp::QoS(10).reliable());

    return CallbackReturn::SUCCESS;
}

FusionNode::CallbackReturn FusionNode::on_activate(const rclcpp_lifecycle::State &) {
    using namespace std::chrono_literals;
    timer_ = this->create_wall_timer(200ms, [this]() { timer_callback(); });

    heartbeat_timer_ = this->create_wall_timer(1s, [this]() { update_heartbeat_status(); });

    fusion_pub_->on_activate();
    heartbeat_pub_->on_activate();

    return CallbackReturn::SUCCESS;
}

FusionNode::CallbackReturn FusionNode::on_deactivate(const rclcpp_lifecycle::State &) {
    timer_.reset();
    heartbeat_timer_.reset();

    fusion_pub_->on_deactivate();
    heartbeat_pub_->on_deactivate();

    return CallbackReturn::SUCCESS;
}

FusionNode::CallbackReturn FusionNode::on_cleanup(const rclcpp_lifecycle::State &) {
    sub_lidar_.reset();
    sub_imu_.reset();
    sub_camera_.reset();
    fusion_pub_.reset();
    heartbeat_pub_.reset();

    lidar_cache_.reset();
    imu_cache_.reset();
    camera_cache_.reset();

    return CallbackReturn::SUCCESS;
}

FusionNode::CallbackReturn FusionNode::on_shutdown(const rclcpp_lifecycle::State &) {
    timer_.reset();
    heartbeat_timer_.reset();
    sub_lidar_.reset();
    sub_imu_.reset();
    sub_camera_.reset();
    fusion_pub_.reset();
    heartbeat_pub_.reset();

    return CallbackReturn::SUCCESS;
}

// ── Sensor callbacks — 缓存最新数据 + 记录到达时间戳 ──────────────────────────

void FusionNode::lidar_callback(sensor_msgs::msg::LaserScan::SharedPtr msg) {
    lidar_cache_ = msg;
    lidar_stamp_ = this->now();
}

void FusionNode::imu_callback(sensor_msgs::msg::Imu::SharedPtr msg) {
    imu_cache_ = msg;
    imu_stamp_ = this->now();
}

void FusionNode::camera_callback(sensor_msgs::msg::Image::SharedPtr msg) {
    camera_cache_ = msg;
    camera_stamp_ = this->now();
}

// ── Timer callback — 入口：评估降级级别 → 分发到对应处理函数 ─────────────────

void FusionNode::timer_callback() {
    auto old_level = degradation_;
    degradation_   = evaluate_degradation();

    // KF 预测步：用实际 dt 而非硬编码 200ms
    // 面试关键：实时系统中 timer 不准是常态——系统负载/GC/IO 都会影响回调延迟。
    // 用实际 dt 保证 KF 在 timer 抖动时也能正确预测。
    auto now = this->now();
    if (last_kf_predict_.nanoseconds() > 0 && imu_cache_) {
      double dt = (now - last_kf_predict_).seconds();
      if (dt > 0.001 && dt < 1.0) {  // 过滤异常 dt（首次调用、长时间阻塞）
        kf_.predict(dt,
                    imu_cache_->linear_acceleration.x,
                    imu_cache_->linear_acceleration.y);
      }
    }
    last_kf_predict_ = now;

    auto msg            = ros2_robot_middleware::msg::PerceptionObjects{};
    msg.header.stamp    = this->now();
    msg.header.frame_id = "base_link";

    switch (degradation_) {
    case DegradationLevel::FULL:      fuse_full(msg);       break;
    case DegradationLevel::NO_LIDAR:  fuse_no_lidar(msg);   break;
    case DegradationLevel::NO_CAMERA: fuse_no_camera(msg);  break;
    case DegradationLevel::NO_IMU:    fuse_no_imu(msg);     break;
    case DegradationLevel::CRITICAL:
        // 不做融合，发布空结果 — Decision层会收到空列表并停止前进
        break;
    }

    // KF 更新步：用第一个检测物体的位置做观测修正
    if (!msg.objects.empty()) {
      const auto &first = msg.objects[0];
      kf_.update(first.x, first.y);
    }

    fusion_pub_->publish(msg);

    // 降级级别变化时打日志
    if (degradation_ != old_level) {
      RCLCPP_WARN(this->get_logger(), "Degradation: %d → %d",
                   static_cast<int>(old_level), static_cast<int>(degradation_));
    }

    RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 5000,
                         "PerceptionObjects published: %zu object(s) [level=%d]",
                         msg.objects.size(), static_cast<int>(degradation_));
}

// ── 心跳入口 — 根据降级级别发送不同状态字符串 ─────────────────────────────────

void FusionNode::update_heartbeat_status() {
    auto msg = std_msgs::msg::String{};

    switch (degradation_) {
    case DegradationLevel::FULL:      msg.data = "alive";              break;
    case DegradationLevel::NO_LIDAR:  msg.data = "degraded_no_lidar";  break;
    case DegradationLevel::NO_CAMERA: msg.data = "degraded_no_camera"; break;
    case DegradationLevel::NO_IMU:    msg.data = "degraded_no_imu";    break;
    case DegradationLevel::CRITICAL:  msg.data = "critical";           break;
    }

    heartbeat_pub_->publish(msg);
}

// ── 数据新鲜度检查 ───────────────────────────────────────────────────────────

bool FusionNode::is_stale(rclcpp::Time last_update, double timeout_s) const {
    if (last_update.nanoseconds() == 0) {
        return true;  // 从未收到过数据
    }
    return (this->now() - last_update).seconds() > timeout_s;
}

FusionNode::DegradationLevel FusionNode::evaluate_degradation() const {
    bool lidar_missing  = is_stale(lidar_stamp_,  kLidarTimeout);
    bool imu_missing    = is_stale(imu_stamp_,    kImuTimeout);
    bool camera_missing = is_stale(camera_stamp_, kCameraTimeout);

    int missing = (lidar_missing ? 1 : 0) + (imu_missing ? 1 : 0) + (camera_missing ? 1 : 0);

    if (missing >= 2) return DegradationLevel::CRITICAL;
    if (missing == 0) return DegradationLevel::FULL;

    // 只有一个传感器缺失：精确判断
    if (lidar_missing)  return DegradationLevel::NO_LIDAR;
    if (imu_missing)    return DegradationLevel::NO_IMU;
    if (camera_missing) return DegradationLevel::NO_CAMERA;

    return DegradationLevel::FULL;
}

// ── 融合路径：全部正常 — 保持原有聚类逻辑 ─────────────────────────────────────

void FusionNode::fuse_full(ros2_robot_middleware::msg::PerceptionObjects &output) {
    constexpr int kNumPoints      = 360;
    constexpr int kMaxObjects     = 5;
    constexpr float kRangeThreshold = 3.0F;
    constexpr int kClusterGap     = 5;

    int start = -1;
    for (int i = 0; i < kNumPoints && static_cast<int>(output.objects.size()) < kMaxObjects; ++i) {
        bool hit = (lidar_cache_->ranges[i] > 0.1F && lidar_cache_->ranges[i] < kRangeThreshold);

        if (hit && start < 0) {
            start = i;
        } else if (!hit && start >= 0) {
            if (i - start > kClusterGap) {
                int mid     = (start + i) / 2;
                float angle = lidar_cache_->angle_min + mid * lidar_cache_->angle_increment;
                float range = lidar_cache_->ranges[mid];
                float x     = range * std::cos(angle);
                float y     = range * std::sin(angle);

                auto obj = ros2_robot_middleware::msg::Object{};
                obj.id   = "obj_" + std::to_string(output.objects.size());
                obj.x    = x;
                obj.y    = y;
                obj.z    = 0.0F;

                output.objects.push_back(obj);
                start = -1;
            } else {
                start = -1;
            }
        }
    }
}

// ── 融合路径：LiDAR 缺失 → Camera 深度图投影到 2D ─────────────────────────────

void FusionNode::fuse_no_lidar(ros2_robot_middleware::msg::PerceptionObjects &output) {
    if (!camera_cache_) return;

    // 面试官视角："你用 Camera 怎么替代 LiDAR？"
    // 答：假设地面平坦，对图像做简单的强度分割，找到高对比度区域
    //    作为障碍物候选。真实场景应用 depth-from-mono 或用到深度摄像头。
    //    这里是一个架构上的降级路径占位，证明系统不会因为 LiDAR 挂了就停机。

    constexpr int kMaxObjects     = 3;     // Camera 检测能力弱，限制对象数
    constexpr int kBlockSize      = 80;    // 80×80 像素块
    constexpr int kBlocksX        = 8;     // 640 / 80
    constexpr int kBlocksY        = 6;     // 480 / 80
    constexpr float kFovRad       = 1.2F;  // 约 69° FOV，近似 RealSense D435

    const auto &data = camera_cache_->data;

    int count = 0;
    for (int by = 0; by < kBlocksY && count < kMaxObjects; ++by) {
        for (int bx = 0; bx < kBlocksX && count < kMaxObjects; ++bx) {
            // 计算 80×80 块内的平均强度
            long sum = 0;
            int pixels = 0;
            for (int py = 0; py < kBlockSize; ++py) {
                for (int px = 0; px < kBlockSize; ++px) {
                    int px_idx = (by * kBlockSize + py) * camera_cache_->step +
                                 (bx * kBlockSize + px) * 3;  // rgb8: 3 bytes/pixel
                    if (px_idx + 2 < static_cast<int>(data.size())) {
                        // 亮度 = 0.299R + 0.587G + 0.114B (BT.601)
                        sum += static_cast<long>(data[px_idx]) * 30 +
                               static_cast<long>(data[px_idx + 1]) * 59 +
                               static_cast<long>(data[px_idx + 2]) * 11;
                        ++pixels;
                    }
                }
            }
            if (pixels == 0) continue;

            uint8_t avg = static_cast<uint8_t>(sum / (pixels * 100));

            // 低于阈值的暗区域视为障碍物（物体遮挡了背景光）
            if (avg < 60) {
                // 像素坐标 → 世界坐标（简化透视投影逆运算）
                // 假设地面平坦，所有物体在 y=0 平面上
                float norm_x = (static_cast<float>(bx) + 0.5F) / kBlocksX - 0.5F;
                float norm_y = (static_cast<float>(by) + 0.5F) / kBlocksY - 0.5F;
                // 距离与图像行号相关：图像下方 = 近处
                float dist   = 1.5F + norm_y * 2.5F;  // 1.5m ~ 4m 范围
                float world_x = std::tan(norm_x * kFovRad) * dist;

                auto obj = ros2_robot_middleware::msg::Object{};
                obj.id   = "cam_obj_" + std::to_string(count);
                obj.x    = world_x;
                obj.y    = dist;  // 前方距离
                obj.z    = 0.0F;

                output.objects.push_back(obj);
                ++count;
            }
        }
    }
}

// ── 融合路径：Camera 缺失 → 纯 LiDAR 聚类（与 FULL 逻辑相同，无视觉分类）────

void FusionNode::fuse_no_camera(ros2_robot_middleware::msg::PerceptionObjects &output) {
    // 面试官视角："Camera 都不在了，你的融合还能做什么？"
    // 答：LiDAR 仍然是主要感知来源。没有 Camera 意味着：
    //    1. 物体没有视觉分类（不知道是人还是柱子）
    //    2. 聚类精度不变（LiDAR 0.01m 级）
    //    3. 这是最常见的降级场景，因为 Camera 数据处理量大、故障率高
    //
    // 实际上 fuse_full 也不依赖 camera 数据做聚类——Camera 只是 flag。
    // 所以 NO_CAMERA 和 FULL 的聚类逻辑完全一样。
    // 区别只在于心跳字符串和降级标记。
    fuse_full(output);
}

// ── 融合路径：IMU 缺失 → LiDAR 帧间匹配（ICP 近似）──────────────────────────

void FusionNode::fuse_no_imu(ros2_robot_middleware::msg::PerceptionObjects &output) {
    // 面试官视角："IMU 100Hz 惯性数据没了，你怎么补偿？"
    // 答：IMU 提供高频位姿增量（ω·dt），没了之后用 LiDAR 帧间匹配做低频位姿估算。
    //    LiDAR 10Hz → 更新频率从 100Hz 降到 10Hz，机器人运动补偿精度下降。
    //    当前阶段：用 ICP 近似占位。M3 集成 robot_localization 后会真做帧间匹配。
    //
    // 当前实现：和 FULL 一样的聚类逻辑，但输出对象的位置加了一个固定的不确定性偏置
    fuse_full(output);

    // 给每个对象附加 0.1m 的不确定性（模拟无 IMU 时的漂移）
    // M3 会用 robot_localization 的 EKF 来消除这个偏置
    for (auto &obj : output.objects) {
        obj.x += 0.05F;  // 模拟漂移
        obj.y += 0.05F;
    }
}
