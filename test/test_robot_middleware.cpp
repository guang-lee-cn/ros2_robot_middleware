#include "ros2_robot_middleware/camera_node.hpp"
#include "ros2_robot_middleware/decision_node.hpp"
#include "ros2_robot_middleware/fusion_node.hpp"
#include "ros2_robot_middleware/imu_node.hpp"
#include "ros2_robot_middleware/lidar_node.hpp"
#include "ros2_robot_middleware/motor_ctrl_node.hpp"

#include "ros2_robot_middleware/action/move_to_pose.hpp"
#include "ros2_robot_middleware/msg/camera_image.hpp"
#include "ros2_robot_middleware/msg/imu_data.hpp"
#include "ros2_robot_middleware/msg/lidar_scan.hpp"
#include "ros2_robot_middleware/msg/perception_objects.hpp"
#include "ros2_robot_middleware/srv/set_param.hpp"

#include <gtest/gtest.h>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include <chrono>
#include <memory>

// ---------------------------------------------------------------------------
// Fixture — rclcpp init once per suite (DDS cleanup is async, per-test
// shutdown/init is unreliable for isolation)
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

// ===================================================================
// Sensor layer tests
// ===================================================================

// ── LidarNode ──────────────────────────────────────────────────────

TEST_F(RobotMiddlewareTest, LidarProducesValidRanges) {
  // Given: LidarNode publishes simulated SICK TiM781 data at 10Hz
  auto node = std::make_shared<LidarNode>();

  ros2_robot_middleware::msg::LidarScan::SharedPtr last_msg;
  auto sub = node->create_subscription<ros2_robot_middleware::msg::LidarScan>(
      "/sensor/lidar", rclcpp::QoS(10).best_effort(),
      [&last_msg](ros2_robot_middleware::msg::LidarScan::SharedPtr msg) { last_msg = msg; });

  // When: the timer callback fires
  ASSERT_TRUE(spin_until(node->get_node_base_interface(),
                         [&last_msg] { return last_msg != nullptr; },
                         std::chrono::seconds(3)));

  // Then: 360 valid ranges and intensities within physical bounds
  EXPECT_EQ(last_msg->ranges.size(), 360u);
  EXPECT_EQ(last_msg->intensities.size(), 360u);
  EXPECT_NEAR(last_msg->angle_min, -M_PI, 0.001F);
  EXPECT_NEAR(last_msg->angle_max, M_PI, 0.001F);

  for (size_t i = 0; i < last_msg->ranges.size(); ++i) {
    EXPECT_GE(last_msg->ranges[i], 0.1F) << "range[" << i << "] below min";
    EXPECT_LE(last_msg->ranges[i], 6.5F) << "range[" << i << "] above max";
  }
  for (size_t i = 0; i < last_msg->intensities.size(); ++i) {
    EXPECT_GE(last_msg->intensities[i], 0.0F) << "intensity[" << i << "] negative";
    EXPECT_LE(last_msg->intensities[i], 1.0F) << "intensity[" << i << "] above 1.0";
  }
}

// ── ImuNode ────────────────────────────────────────────────────────

TEST_F(RobotMiddlewareTest, ImuProducesValidData) {
  // Given: ImuNode publishes simulated Bosch BMI088 data at 100Hz
  auto node = std::make_shared<ImuNode>();

  ros2_robot_middleware::msg::ImuData::SharedPtr last_msg;
  auto sub = node->create_subscription<ros2_robot_middleware::msg::ImuData>(
      "/sensor/imu", rclcpp::QoS(10).reliable(),
      [&last_msg](ros2_robot_middleware::msg::ImuData::SharedPtr msg) { last_msg = msg; });

  // When: the timer callback fires
  ASSERT_TRUE(spin_until(node->get_node_base_interface(),
                         [&last_msg] { return last_msg != nullptr; },
                         std::chrono::seconds(3)));

  // Then: angular velocity within BMI088 noise spec, acceleration within bounds
  for (int axis = 0; axis < 3; ++axis) {
    EXPECT_NEAR(last_msg->angular_velocity[axis], 0.0, 0.04)
        << "angular_velocity[" << axis << "] out of spec";
    EXPECT_NEAR(last_msg->linear_acceleration[axis], 0.0, 0.2)
        << "linear_acceleration[" << axis << "] out of spec";
  }
  EXPECT_EQ(last_msg->header.frame_id, "imu_link");
}

// ── CameraNode ─────────────────────────────────────────────────────

TEST_F(RobotMiddlewareTest, CameraProducesValidImage) {
  // Given: CameraNode publishes simulated Intel RealSense D435 frames at 5Hz
  auto node = std::make_shared<CameraNode>();

  ros2_robot_middleware::msg::CameraImage::SharedPtr last_msg;
  auto sub = node->create_subscription<ros2_robot_middleware::msg::CameraImage>(
      "/sensor/camera", rclcpp::QoS(10).best_effort(),
      [&last_msg](ros2_robot_middleware::msg::CameraImage::SharedPtr msg) { last_msg = msg; });

  // When: the timer callback fires
  ASSERT_TRUE(spin_until(node->get_node_base_interface(),
                         [&last_msg] { return last_msg != nullptr; },
                         std::chrono::seconds(3)));

  // Then: 640×480 rgb8 image, 900KB payload
  EXPECT_EQ(last_msg->width, 640u);
  EXPECT_EQ(last_msg->height, 480u);
  EXPECT_EQ(last_msg->step, 1920u);
  EXPECT_EQ(last_msg->encoding, "rgb8");
  EXPECT_EQ(last_msg->data.size(), 921600u);
}

// ===================================================================
// Motor control layer tests
// ===================================================================

TEST_F(RobotMiddlewareTest, MotorCtrlReachesCloseGoalInOneStep) {
  // Given: MotorCtrlNode with step_size = 0.05, action client ready
  auto motor = std::make_shared<MotorCtrlNode>();
  auto client = rclcpp_action::create_client<ros2_robot_middleware::action::MoveToPose>(
      motor, "/cmd/move_to_pose");
  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(2)));

  auto goal = ros2_robot_middleware::action::MoveToPose::Goal{};
  goal.target_x = 0.0F;
  goal.target_y = 0.03F;  // within one step
  goal.target_theta = 0.0F;
  goal.max_speed = 0.5F;

  bool done = false;
  bool reached = false;
  auto opts = rclcpp_action::Client<ros2_robot_middleware::action::MoveToPose>::SendGoalOptions{};
  opts.result_callback =
      [&done, &reached](
          const rclcpp_action::ClientGoalHandle<ros2_robot_middleware::action::MoveToPose>::WrappedResult &result) {
        done = true;
        reached = result.result->reached;
      };

  // When: goal sent to a target within step_size distance
  client->async_send_goal(goal, opts);

  ASSERT_TRUE(spin_until(motor->get_node_base_interface(),
                         [&done] { return done; },
                         std::chrono::seconds(5)));

  // Then: goal reaches target immediately
  EXPECT_TRUE(reached);
}

TEST_F(RobotMiddlewareTest, MotorCtrlStepsToFarGoal) {
  // Given: MotorCtrlNode with step_size = 0.05, target at distance 0.15 (4 steps)
  auto motor = std::make_shared<MotorCtrlNode>();
  auto client = rclcpp_action::create_client<ros2_robot_middleware::action::MoveToPose>(
      motor, "/cmd/move_to_pose");
  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(2)));

  auto goal = ros2_robot_middleware::action::MoveToPose::Goal{};
  goal.target_x = 0.0F;
  goal.target_y = 0.15F;  // requires multiple steps
  goal.target_theta = 0.0F;
  goal.max_speed = 0.5F;

  bool done = false;
  bool reached = false;
  auto opts = rclcpp_action::Client<ros2_robot_middleware::action::MoveToPose>::SendGoalOptions{};
  opts.result_callback =
      [&done, &reached](
          const rclcpp_action::ClientGoalHandle<ros2_robot_middleware::action::MoveToPose>::WrappedResult &result) {
        done = true;
        reached = result.result->reached;
      };

  // When: goal sent requiring multiple interpolation steps
  client->async_send_goal(goal, opts);

  ASSERT_TRUE(spin_until(motor->get_node_base_interface(),
                         [&done] { return done; },
                         std::chrono::seconds(5)));

  // Then: goal eventually reached after stepping
  EXPECT_TRUE(reached);
}

TEST_F(RobotMiddlewareTest, MotorCtrlSetParamUpdatesStepSize) {
  // Given: MotorCtrlNode with SetParam service
  auto motor = std::make_shared<MotorCtrlNode>();
  auto client = motor->create_client<ros2_robot_middleware::srv::SetParam>("/cmd/set_param");
  ASSERT_TRUE(client->wait_for_service(std::chrono::seconds(2)));

  // When: set_param "step_size" = 0.10
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

  // Then: acknowledged with success
  auto response = future.get();
  EXPECT_TRUE(response->success);
  EXPECT_EQ(response->message, "Parameter updated");
}

TEST_F(RobotMiddlewareTest, MotorCtrlSetParamUnknownParameter) {
  // Given: MotorCtrlNode with SetParam service
  auto motor = std::make_shared<MotorCtrlNode>();
  auto client = motor->create_client<ros2_robot_middleware::srv::SetParam>("/cmd/set_param");
  ASSERT_TRUE(client->wait_for_service(std::chrono::seconds(2)));

  // When: set_param called with an unrecognized parameter name
  auto request = std::make_shared<ros2_robot_middleware::srv::SetParam::Request>();
  request->param_name = "unknown_param";
  request->value = 1.0;
  auto future = client->async_send_request(request);

  ASSERT_TRUE(spin_until(motor->get_node_base_interface(),
                         [&future] {
                           return future.wait_for(std::chrono::milliseconds(0)) ==
                                  std::future_status::ready;
                         },
                         std::chrono::seconds(2)));

  // Then: success is still true (non-fatal), but message indicates unknown
  auto response = future.get();
  EXPECT_TRUE(response->success);
  EXPECT_EQ(response->message, "Unknown parameter");
}

// ===================================================================
// Decision layer tests
// ===================================================================

TEST_F(RobotMiddlewareTest, DecisionSendsGoalWhenPerceptionArrives) {
  // Given: DecisionNode + a mock action server recording incoming goals
  auto decision = std::make_shared<DecisionNode>();

  // Mock action server: accept all goals, store the received coordinates
  float received_x = -1.0F;
  float received_y = -1.0F;
  bool goal_received = false;

  auto mock_server = rclcpp_action::create_server<ros2_robot_middleware::action::MoveToPose>(
      decision, "/cmd/move_to_pose",
      [](const rclcpp_action::GoalUUID &,
         std::shared_ptr<const ros2_robot_middleware::action::MoveToPose::Goal>) {
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
      },
      [](const std::shared_ptr<rclcpp_action::ServerGoalHandle<ros2_robot_middleware::action::MoveToPose>>) {
        return rclcpp_action::CancelResponse::ACCEPT;
      },
      [&received_x, &received_y, &goal_received](
          const std::shared_ptr<rclcpp_action::ServerGoalHandle<ros2_robot_middleware::action::MoveToPose>> gh) {
        received_x = gh->get_goal()->target_x;
        received_y = gh->get_goal()->target_y;
        goal_received = true;
        auto result = std::make_shared<ros2_robot_middleware::action::MoveToPose::Result>();
        result->reached = true;
        result->final_x = received_x;
        result->final_y = received_y;
        gh->succeed(result);
      });

  // When: a PerceptionObjects message with one object at (2.0, 1.5) arrives
  auto perception = ros2_robot_middleware::msg::PerceptionObjects{};
  perception.header.frame_id = "base_link";
  auto obj = ros2_robot_middleware::msg::Object{};
  obj.id = "obj_0";
  obj.x = 2.0F;
  obj.y = 1.5F;
  obj.z = 0.0F;
  perception.objects.push_back(obj);

  auto pub = decision->create_publisher<ros2_robot_middleware::msg::PerceptionObjects>(
      "/perception/objects", rclcpp::QoS(10).reliable());
  pub->publish(perception);

  ASSERT_TRUE(spin_until(decision->get_node_base_interface(),
                         [&goal_received] { return goal_received; },
                         std::chrono::seconds(3)));

  // Then: a MoveToPose goal is sent with matching object coordinates
  EXPECT_TRUE(goal_received);
  EXPECT_FLOAT_EQ(received_x, 2.0F);
  EXPECT_FLOAT_EQ(received_y, 1.5F);
}

// ===================================================================
// Fusion layer tests (last — publishes perception data)
// ===================================================================

// Helper: build a lidar frame with configurable range at given angular span
static ros2_robot_middleware::msg::LidarScan
make_lidar(float range_near, float range_far, int center_idx, int half_width) {
  auto msg = ros2_robot_middleware::msg::LidarScan{};
  msg.header.frame_id = "lidar_frame";
  msg.angle_min = -M_PI;
  msg.angle_max = M_PI;
  msg.angle_increment = 2.0F * M_PI / 360.0F;
  msg.time_increment = 0.0001F;
  msg.ranges.resize(360);
  msg.intensities.resize(360);
  for (int i = 0; i < 360; ++i) {
    int dist_from_center = std::abs(i - center_idx);
    if (dist_from_center <= half_width) {
      msg.ranges[i] = range_near;
      msg.intensities[i] = 0.8F;
    } else {
      msg.ranges[i] = range_far;
      msg.intensities[i] = 0.1F;
    }
  }
  return msg;
}

static ros2_robot_middleware::msg::ImuData make_imu() {
  auto msg = ros2_robot_middleware::msg::ImuData{};
  msg.header.frame_id = "imu_link";
  return msg;
}

static ros2_robot_middleware::msg::CameraImage make_camera() {
  auto msg = ros2_robot_middleware::msg::CameraImage{};
  msg.header.frame_id = "camera_link";
  msg.width = 640;
  msg.height = 480;
  msg.step = 1920;
  msg.encoding = "rgb8";
  msg.data.resize(921600);
  return msg;
}

TEST_F(RobotMiddlewareTest, FusionDetectsNearbyObject) {
  // Given: FusionNode with all 3 sensor caches populated,
  //        lidar has a cluster at 1.5m (within the 3m detection threshold)
  auto fusion = std::make_shared<FusionNode>();

  ros2_robot_middleware::msg::PerceptionObjects::SharedPtr last_output;
  auto out_sub = fusion->create_subscription<ros2_robot_middleware::msg::PerceptionObjects>(
      "/perception/objects", rclcpp::QoS(10).reliable(),
      [&last_output](ros2_robot_middleware::msg::PerceptionObjects::SharedPtr msg) { last_output = msg; });

  auto lidar_pub = fusion->create_publisher<ros2_robot_middleware::msg::LidarScan>(
      "/sensor/lidar", rclcpp::QoS(10).best_effort());
  auto imu_pub = fusion->create_publisher<ros2_robot_middleware::msg::ImuData>(
      "/sensor/imu", rclcpp::QoS(10).reliable());
  auto cam_pub = fusion->create_publisher<ros2_robot_middleware::msg::CameraImage>(
      "/sensor/camera", rclcpp::QoS(10).best_effort());

  // When: all three sensors are published
  lidar_pub->publish(make_lidar(1.5F, 10.0F, 180, 10));
  imu_pub->publish(make_imu());
  cam_pub->publish(make_camera());

  ASSERT_TRUE(spin_until(fusion->get_node_base_interface(),
                         [&last_output] { return last_output != nullptr; },
                         std::chrono::seconds(3)));

  // Then: at least 1 object detected from the near-range cluster
  EXPECT_GE(last_output->objects.size(), 1u);
  if (!last_output->objects.empty()) {
    EXPECT_EQ(last_output->objects[0].z, 0.0F);
  }
}
