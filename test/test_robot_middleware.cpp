#include "ros2_robot_middleware/camera_node.hpp"
#include "ros2_robot_middleware/fusion_node.hpp"
#include "ros2_robot_middleware/imu_node.hpp"
#include "ros2_robot_middleware/lidar_node.hpp"
#include "ros2_robot_middleware/motor_ctrl_node.hpp"

#include "ros2_robot_middleware/action/move_to_pose.hpp"
#include "ros2_robot_middleware/msg/camera_image.hpp"
#include "ros2_robot_middleware/msg/imu_data.hpp"
#include "ros2_robot_middleware/msg/lidar_scan.hpp"
#include "ros2_robot_middleware/msg/perception_objects.hpp"

#include <gtest/gtest.h>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include <chrono>
#include <memory>

// ---------------------------------------------------------------------------
// Fixture — rclcpp init/shutdown once for all tests
// ---------------------------------------------------------------------------
class RobotMiddlewareTest : public ::testing::Test {
protected:
  static void SetUpTestSuite() { rclcpp::init(0, nullptr); }
  static void TearDownTestSuite() { rclcpp::shutdown(); }
};

// ---------------------------------------------------------------------------
// Helper — spin executor until predicate is true or timeout expires
// ---------------------------------------------------------------------------
template <typename Predicate>
bool spin_until(rclcpp::node_interfaces::NodeBaseInterface::SharedPtr node_iface,
                Predicate pred, std::chrono::milliseconds timeout) {
  auto start = std::chrono::steady_clock::now();
  rclcpp::executors::SingleThreadedExecutor exec;
  exec.add_node(node_iface);
  while (!pred() && (std::chrono::steady_clock::now() - start) < timeout) {
    exec.spin_once(std::chrono::milliseconds(10));
  }
  exec.remove_node(node_iface);
  return pred();
}

// ---------------------------------------------------------------------------
// 1. LidarNode — verify published data is within physical range
// ---------------------------------------------------------------------------
TEST_F(RobotMiddlewareTest, LidarProducesValidRanges) {
  auto node = std::make_shared<LidarNode>();

  ros2_robot_middleware::msg::LidarScan::SharedPtr last_msg;
  auto sub = node->create_subscription<ros2_robot_middleware::msg::LidarScan>(
      "/sensor/lidar", rclcpp::QoS(10).best_effort(),
      [&last_msg](ros2_robot_middleware::msg::LidarScan::SharedPtr msg) { last_msg = msg; });

  ASSERT_TRUE(spin_until(node->get_node_base_interface(),
                         [&last_msg] { return last_msg != nullptr; },
                         std::chrono::seconds(3)));

  EXPECT_EQ(last_msg->ranges.size(), 360u);
  EXPECT_EQ(last_msg->intensities.size(), 360u);
  EXPECT_NEAR(last_msg->angle_min, -M_PI, 0.001F);
  EXPECT_NEAR(last_msg->angle_max, M_PI, 0.001F);

  for (auto r : last_msg->ranges) {
    EXPECT_GE(r, 0.1F) << "range too low";
    EXPECT_LE(r, 6.5F) << "range too high";
  }
  for (auto intensity : last_msg->intensities) {
    EXPECT_GE(intensity, 0.0F);
    EXPECT_LE(intensity, 1.0F);
  }
}

// ---------------------------------------------------------------------------
// 2. ImuNode — verify angular velocity and acceleration within spec
// ---------------------------------------------------------------------------
TEST_F(RobotMiddlewareTest, ImuProducesValidData) {
  auto node = std::make_shared<ImuNode>();

  ros2_robot_middleware::msg::ImuData::SharedPtr last_msg;
  auto sub = node->create_subscription<ros2_robot_middleware::msg::ImuData>(
      "/sensor/imu", rclcpp::QoS(10).reliable(),
      [&last_msg](ros2_robot_middleware::msg::ImuData::SharedPtr msg) { last_msg = msg; });

  ASSERT_TRUE(spin_until(node->get_node_base_interface(),
                         [&last_msg] { return last_msg != nullptr; },
                         std::chrono::seconds(3)));

  // Bosch BMI088 noise: angular ±0.02 rad/s, accel ±0.1 m/s² (with margin)
  for (int axis = 0; axis < 3; ++axis) {
    EXPECT_NEAR(last_msg->angular_velocity[axis], 0.0, 0.04) << "ω[" << axis << "] out of range";
    EXPECT_NEAR(last_msg->linear_acceleration[axis], 0.0, 0.2) << "a[" << axis << "] out of range";
  }
  EXPECT_EQ(last_msg->header.frame_id, "imu_link");
}

// ---------------------------------------------------------------------------
// 3. CameraNode — verify image dimensions and encoding
// ---------------------------------------------------------------------------
TEST_F(RobotMiddlewareTest, CameraProducesValidImage) {
  auto node = std::make_shared<CameraNode>();

  ros2_robot_middleware::msg::CameraImage::SharedPtr last_msg;
  auto sub = node->create_subscription<ros2_robot_middleware::msg::CameraImage>(
      "/sensor/camera", rclcpp::QoS(10).best_effort(),
      [&last_msg](ros2_robot_middleware::msg::CameraImage::SharedPtr msg) { last_msg = msg; });

  ASSERT_TRUE(spin_until(node->get_node_base_interface(),
                         [&last_msg] { return last_msg != nullptr; },
                         std::chrono::seconds(3)));

  EXPECT_EQ(last_msg->width, 640u);
  EXPECT_EQ(last_msg->height, 480u);
  EXPECT_EQ(last_msg->step, 1920u);
  EXPECT_EQ(last_msg->encoding, "rgb8");
  EXPECT_EQ(last_msg->data.size(), 921600u);
}

// ---------------------------------------------------------------------------
// 4. FusionNode — inject sensor data with a known object, verify detection
// ---------------------------------------------------------------------------
TEST_F(RobotMiddlewareTest, FusionDetectsNearbyObject) {
  auto fusion = std::make_shared<FusionNode>();

  // Subscribe to fusion output
  ros2_robot_middleware::msg::PerceptionObjects::SharedPtr last_output;
  auto out_sub = fusion->create_subscription<ros2_robot_middleware::msg::PerceptionObjects>(
      "/perception/objects", rclcpp::QoS(10).reliable(),
      [&last_output](ros2_robot_middleware::msg::PerceptionObjects::SharedPtr msg) { last_output = msg; });

  // Build a lidar frame with a clear object at 0° (~1.5m → hits < 3m threshold)
  auto lidar_msg = ros2_robot_middleware::msg::LidarScan{};
  lidar_msg.header.stamp = fusion->now();
  lidar_msg.header.frame_id = "lidar_frame";
  lidar_msg.angle_min = -M_PI;
  lidar_msg.angle_max = M_PI;
  lidar_msg.angle_increment = 2.0F * M_PI / 360.0F;
  lidar_msg.time_increment = 0.0001F;
  lidar_msg.ranges.resize(360);
  lidar_msg.intensities.resize(360);

  // All ranges default to far (no hit), except a cluster near 0° (index ~180)
  for (int i = 0; i < 360; ++i) {
    int dist_from_center = std::abs(i - 180);
    if (dist_from_center <= 10) {
      lidar_msg.ranges[i] = 1.5F;              // nearby object
      lidar_msg.intensities[i] = 0.8F;
    } else {
      lidar_msg.ranges[i] = 10.0F;             // too far → no detection
      lidar_msg.intensities[i] = 0.1F;
    }
  }

  // Dummy imu + camera (just need non-null to satisfy cache check)
  auto imu_msg = ros2_robot_middleware::msg::ImuData{};
  imu_msg.header.stamp = fusion->now();
  imu_msg.header.frame_id = "imu_link";

  auto cam_msg = ros2_robot_middleware::msg::CameraImage{};
  cam_msg.header.stamp = fusion->now();
  cam_msg.header.frame_id = "camera_link";
  cam_msg.width = 640;
  cam_msg.height = 480;
  cam_msg.step = 1920;
  cam_msg.encoding = "rgb8";
  cam_msg.data.resize(921600);

  // Publish sensor data to fusion's input topics
  auto lidar_pub = fusion->create_publisher<ros2_robot_middleware::msg::LidarScan>(
      "/sensor/lidar", rclcpp::QoS(10).best_effort());
  auto imu_pub = fusion->create_publisher<ros2_robot_middleware::msg::ImuData>(
      "/sensor/imu", rclcpp::QoS(10).reliable());
  auto cam_pub = fusion->create_publisher<ros2_robot_middleware::msg::CameraImage>(
      "/sensor/camera", rclcpp::QoS(10).best_effort());

  lidar_pub->publish(lidar_msg);
  imu_pub->publish(imu_msg);
  cam_pub->publish(cam_msg);

  // Spin until fusion timer fires and we get output
  ASSERT_TRUE(spin_until(fusion->get_node_base_interface(),
                         [&last_output] { return last_output != nullptr; },
                         std::chrono::seconds(3)));

  EXPECT_GE(last_output->objects.size(), 1u)
      << "Expected at least 1 object from near-range lidar cluster";
  if (!last_output->objects.empty()) {
    EXPECT_EQ(last_output->objects[0].z, 0.0F);
  }
}

// ---------------------------------------------------------------------------
// 5. MotorCtrlNode — send close-range goal, verify reached
// ---------------------------------------------------------------------------
TEST_F(RobotMiddlewareTest, MotorCtrlReachesCloseGoal) {
  auto motor = std::make_shared<MotorCtrlNode>();
  auto client = rclcpp_action::create_client<ros2_robot_middleware::action::MoveToPose>(
      motor, "/cmd/move_to_pose");

  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(2)));

  auto goal = ros2_robot_middleware::action::MoveToPose::Goal{};
  goal.target_x = 0.0F;
  goal.target_y = 0.03F;  // within one step (step_size_ = 0.05)
  goal.target_theta = 0.0F;
  goal.max_speed = 0.5F;

  bool done = false;
  bool reached = false;
  auto send_goal_options = rclcpp_action::Client<ros2_robot_middleware::action::MoveToPose>::SendGoalOptions{};
  send_goal_options.result_callback =
      [&done, &reached](
          const rclcpp_action::ClientGoalHandle<ros2_robot_middleware::action::MoveToPose>::WrappedResult &result) {
        done = true;
        reached = result.result->reached;
      };

  client->async_send_goal(goal, send_goal_options);

  ASSERT_TRUE(spin_until(motor->get_node_base_interface(),
                         [&done] { return done; },
                         std::chrono::seconds(5)));

  EXPECT_TRUE(reached);
}

// ---------------------------------------------------------------------------
// 6. SetParam Service — verify step_size_ update
// ---------------------------------------------------------------------------
TEST_F(RobotMiddlewareTest, MotorCtrlSetParamUpdatesConfig) {
  auto motor = std::make_shared<MotorCtrlNode>();
  auto client = motor->create_client<ros2_robot_middleware::srv::SetParam>(
      "/cmd/set_param");

  ASSERT_TRUE(client->wait_for_service(std::chrono::seconds(2)));

  auto request = std::make_shared<ros2_robot_middleware::srv::SetParam::Request>();
  request->param_name = "step_size";
  request->value = 0.10;

  auto future = client->async_send_request(request);

  ASSERT_TRUE(spin_until(motor->get_node_base_interface(),
                         [&future] {
                           return future.wait_for(std::chrono::milliseconds(0)) ==
                                  std::future_status::ready;
                         },
                         std::chrono::seconds(2)));

  auto response = future.get();
  EXPECT_TRUE(response->success);
  EXPECT_EQ(response->message, "Parameter updated");
}
