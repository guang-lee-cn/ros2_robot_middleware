#include "ros2_robot_middleware/aliases.hpp"
#include "ros2_robot_middleware/decision_node.hpp"

DecisionNode::DecisionNode()
    : Node("decision")
{
  decision_sub_ = this->create_subscription<PerceptionObjects>(
      "/perception/objects", rclcpp::QoS(10).reliable(),
      [this](PerceptionObjects::SharedPtr msg) { on_perception(msg); });

  client_ = rclcpp_action::create_client<MoveToPose>(this, "/cmd/move_to_pose");
}

// ---------------------------------------------------------------------------
// Perception callback — pick nearest object, send as MoveToPose goal
// ---------------------------------------------------------------------------

void DecisionNode::on_perception(const PerceptionObjects::SharedPtr& objs)
{
  if (objs->objects.empty()) {
    return;
  }

  if (!client_->wait_for_action_server(std::chrono::seconds(1))) {
    RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 5000,
                         "Action server not available");
    return;
  }

  auto goal = MoveToPose::Goal{};
  goal.target_x     = objs->objects[0].x;
  goal.target_y     = objs->objects[0].y;
  goal.target_theta = 0.0F;
  goal.max_speed    = 0.5F;

  auto send_goal_options = rclcpp_action::Client<MoveToPose>::SendGoalOptions{};
  send_goal_options.goal_response_callback =
      [this](ClientGoalHandle::SharedPtr goal_handle) { on_goal_response(goal_handle); };
  send_goal_options.result_callback =
      [this](const ClientGoalHandle::WrappedResult& result) { on_result(result); };

  client_->async_send_goal(goal, send_goal_options);
}

// ---------------------------------------------------------------------------
// Goal response — server accepted or rejected
// ---------------------------------------------------------------------------

void DecisionNode::on_goal_response(const ClientGoalHandle::SharedPtr& goal_handle)
{
  if (!goal_handle) {
    RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
    return;
  }
  RCLCPP_INFO(this->get_logger(), "Goal accepted by motor_ctrl");
}

// ---------------------------------------------------------------------------
// Result — motion completed
// ---------------------------------------------------------------------------

void DecisionNode::on_result(const ClientGoalHandle::WrappedResult& result)
{
  switch (result.code) {
    case rclcpp_action::ResultCode::SUCCEEDED:
      RCLCPP_INFO(this->get_logger(),
                  "MoveToPose succeeded: reached (%.2f, %.2f) in %.2fs",
                  result.result->final_x, result.result->final_y,
                  result.result->elapsed_time);
      break;
    case rclcpp_action::ResultCode::CANCELED:
      RCLCPP_INFO(this->get_logger(), "MoveToPose canceled");
      break;
    default:
      RCLCPP_ERROR(this->get_logger(), "MoveToPose failed");
      break;
  }
}
