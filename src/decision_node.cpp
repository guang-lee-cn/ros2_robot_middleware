#include "ros2_robot_middleware/aliases.hpp"
#include "ros2_robot_middleware/decision_node.hpp"

#include <rclcpp_components/register_node_macro.hpp>

DecisionNode::DecisionNode()
  : rclcpp_lifecycle::LifecycleNode("decision")
{
}

DecisionNode::DecisionNode(const rclcpp::NodeOptions &options)
  : rclcpp_lifecycle::LifecycleNode("decision", options) {
}

DecisionNode::CallbackReturn
DecisionNode::on_configure(const rclcpp_lifecycle::State &)
{
  decision_sub_ = this->create_subscription<PerceptionObjects>(
    "/perception/objects", rclcpp::QoS(10).reliable(),
    [this](PerceptionObjects::SharedPtr msg) { on_perception(msg); });

  client_ = rclcpp_action::create_client<MoveToPose>(this, "/cmd/move_to_pose");

  heartbeat_pub_ = this->create_publisher<std_msgs::msg::String>(
    "/decision/heartbeat", rclcpp::QoS(10).reliable());

  return CallbackReturn::SUCCESS;
}

DecisionNode::CallbackReturn
DecisionNode::on_activate(const rclcpp_lifecycle::State &)
{
  using namespace std::chrono_literals;
  heartbeat_timer_ = this->create_wall_timer(1s, [this]() {
    auto msg = std_msgs::msg::String{};
    msg.data = "alive";
    heartbeat_pub_->publish(msg);
  });

  heartbeat_pub_->on_activate();

  return CallbackReturn::SUCCESS;
}

DecisionNode::CallbackReturn
DecisionNode::on_deactivate(const rclcpp_lifecycle::State &)
{
  heartbeat_timer_.reset();
  heartbeat_pub_->on_deactivate();

  return CallbackReturn::SUCCESS;
}

DecisionNode::CallbackReturn
DecisionNode::on_cleanup(const rclcpp_lifecycle::State &)
{
  decision_sub_.reset();
  client_.reset();
  heartbeat_pub_.reset();

  return CallbackReturn::SUCCESS;
}

DecisionNode::CallbackReturn
DecisionNode::on_shutdown(const rclcpp_lifecycle::State &)
{
  heartbeat_timer_.reset();
  decision_sub_.reset();
  client_.reset();
  heartbeat_pub_.reset();

  return CallbackReturn::SUCCESS;
}

void DecisionNode::on_perception(const PerceptionObjects::SharedPtr& objs)
{
  if (objs->objects.empty()) {
    return;
  }

  // Preemption: 新感知数据到达 → 取消旧 goal → 发新 goal
  // 面试关键：Action 的 preemption 模式是机器人导航的标准做法。
  //   异步发送 cancel 后不等结果直接发新 goal（motor_ctrl 自己处理排队）。
  cancel_active_goal();

  send_goal(objs->objects[0].x, objs->objects[0].y);
}

void DecisionNode::send_goal(float target_x, float target_y)
{
  if (!client_->action_server_is_ready()) {
    RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 5000,
                         "Action server not available");
    return;
  }

  // 记住目标便于重试
  last_target_x_ = target_x;
  last_target_y_ = target_y;

  auto goal         = MoveToPose::Goal{};
  goal.target_x     = target_x;
  goal.target_y     = target_y;
  goal.target_theta = 0.0F;
  goal.max_speed    = 0.5F;

  auto send_goal_options = rclcpp_action::Client<MoveToPose>::SendGoalOptions{};
  send_goal_options.goal_response_callback =
    [this](ClientGoalHandle::SharedPtr goal_handle) { on_goal_response(goal_handle); };
  send_goal_options.result_callback =
    [this](const ClientGoalHandle::WrappedResult& result) { on_result(result); };

  client_->async_send_goal(goal, send_goal_options);
}

void DecisionNode::cancel_active_goal()
{
  if (!active_goal_) return;

  RCLCPP_INFO(this->get_logger(), "Preempting previous goal");
  client_->async_cancel_goal(active_goal_);
  active_goal_.reset();
  retry_count_ = 0;
}

void DecisionNode::on_goal_response(const ClientGoalHandle::SharedPtr& goal_handle)
{
  if (!goal_handle) {
    if (retry_count_ < kMaxRetries) {
      retry_count_++;
      RCLCPP_WARN(this->get_logger(), "Goal rejected, retrying %d/%d (%.2f, %.2f)",
                   retry_count_, kMaxRetries, last_target_x_, last_target_y_);
      send_goal(last_target_x_, last_target_y_);
    } else {
      RCLCPP_ERROR(this->get_logger(), "Goal rejected after %d retries, giving up",
                   kMaxRetries);
      retry_count_ = 0;
    }
    return;
  }

  active_goal_  = goal_handle;
  retry_count_  = 0;
  RCLCPP_INFO(this->get_logger(), "Goal accepted by motor_ctrl");
}

void DecisionNode::on_result(const ClientGoalHandle::WrappedResult& result)
{
  active_goal_.reset();

  switch (result.code) {
    case rclcpp_action::ResultCode::SUCCEEDED:
      RCLCPP_INFO(this->get_logger(),
                  "MoveToPose succeeded: reached (%.2f, %.2f)",
                  result.result->final_x, result.result->final_y);
      break;
    case rclcpp_action::ResultCode::CANCELED:
      RCLCPP_INFO(this->get_logger(), "MoveToPose canceled");
      break;
    default:
      RCLCPP_ERROR(this->get_logger(), "MoveToPose failed");
      break;
  }
}

RCLCPP_COMPONENTS_REGISTER_NODE(DecisionNode)
