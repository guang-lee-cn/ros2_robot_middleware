#include "ros2_robot_middleware/aliases.hpp"
#include "ros2_robot_middleware/decision_node.hpp"

DecisionNode::DecisionNode()
  : rclcpp_lifecycle::LifecycleNode("decision")
{
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

void DecisionNode::on_goal_response(const ClientGoalHandle::SharedPtr& goal_handle)
{
  if (!goal_handle) {
    RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
    return;
  }
  RCLCPP_INFO(this->get_logger(), "Goal accepted by motor_ctrl");
}

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
