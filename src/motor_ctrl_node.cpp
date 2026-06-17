#include "ros2_robot_middleware/motor_ctrl_node.hpp"
#include "ros2_robot_middleware/aliases.hpp"

MotorCtrlNode::MotorCtrlNode() : Node("motor_ctrl") {
    // ── Action Server ──
    action_server_ = rclcpp_action::create_server<MoveToPose>(
        this, "/cmd/move_to_pose",
        [this](const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const MoveToPose::Goal> goal) {
            return handle_goal(uuid, goal);
        },
        [this](const std::shared_ptr<ServerGoalHandle> goal_handle) { return handle_cancel(goal_handle); },
        [this](const std::shared_ptr<ServerGoalHandle> goal_handle) { execute(goal_handle); });

    // ── Service Server ──
    service_server_ = this->create_service<SetParam>(
        "/cmd/set_param", [this](const std::shared_ptr<SetParam::Request> req,
                                 std::shared_ptr<SetParam::Response> resp) { handle_set_param(req, resp); });
}

rclcpp_action::GoalResponse MotorCtrlNode::handle_goal(const rclcpp_action::GoalUUID &/*uuid*/,
                                                       std::shared_ptr<const MoveToPose::Goal> goal) {
    RCLCPP_INFO(this->get_logger(), "Received goal: target=(%.2f, %.2f, %.2f) speed=%.2f", goal->target_x,
                goal->target_y, goal->target_theta, goal->max_speed);
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse MotorCtrlNode::handle_cancel(const std::shared_ptr<ServerGoalHandle> /*goal_handle*/) {
    RCLCPP_INFO(this->get_logger(), "Cancel request received");
    return rclcpp_action::CancelResponse::ACCEPT;
}

void MotorCtrlNode::execute(const std::shared_ptr<ServerGoalHandle> goal_handle) {
    const auto goal = goal_handle->get_goal();
    float current_x = 0.0F;
    float current_y = 0.0F;

    rclcpp::Rate rate(10); // 10Hz

    while (rclcpp::ok()) {
        // ── 1. Check cancel ──
        if (goal_handle->is_canceling()) {
            auto result          = std::make_shared<MoveToPose::Result>();
            result->reached      = false;
            result->final_x      = current_x;
            result->final_y      = current_y;
            result->elapsed_time = 0;
            goal_handle->canceled(result);
            RCLCPP_INFO(this->get_logger(), "Goal canceled");
            return;
        }

        // ── 2. Check arrival ──
        float dx   = goal->target_x - current_x;
        float dy   = goal->target_y - current_y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < step_size_) {
            current_x = goal->target_x;
            current_y = goal->target_y;

            auto result          = std::make_shared<MoveToPose::Result>();
            result->reached      = true;
            result->final_x      = current_x;
            result->final_y      = current_y;
            result->elapsed_time = 0; // simplified
            goal_handle->succeed(result);
            RCLCPP_INFO(this->get_logger(), "Goal reached: (%.2f, %.2f)", current_x, current_y);
            return;
        }

        // ── 3. Step forward ──
        float ratio = step_size_ / dist;
        current_x += dx * ratio;
        current_y += dy * ratio;

        // ── 4. Publish feedback ──
        auto feedback                = std::make_shared<MoveToPose::Feedback>();
        feedback->current_x          = current_x;
        feedback->current_y          = current_y;
        feedback->distance_remaining = dist - step_size_;
        feedback->percent_complete =
            (1.0F - (dist / std::sqrt(goal->target_x * goal->target_x + goal->target_y * goal->target_y))) * 100.0F;

        goal_handle->publish_feedback(feedback);
        rate.sleep(); // 100ms step
    }
}

void MotorCtrlNode::handle_set_param(const std::shared_ptr<SetParam::Request> request,
                                     std::shared_ptr<SetParam::Response> response) {
    RCLCPP_INFO(this->get_logger(),
                "SetParam: %s = %.4f", request->param_name.c_str(), request->value);
    if (request->param_name == "step_size") {
        step_size_ = static_cast<float>(request->value);
        response->message = "Parameter updated";
    } else {
        response->message = "Unknown parameter";
    }
    response->success = true;
}
