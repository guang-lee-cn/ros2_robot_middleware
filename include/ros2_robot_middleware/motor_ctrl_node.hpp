#ifndef ROS2_ROBOT_MIDDLEWARE_MOTOR_CTRL_NODE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_MOTOR_CTRL_NODE_HPP_

#include "ros2_robot_middleware/action/move_to_pose.hpp"
#include "ros2_robot_middleware/srv/set_param.hpp"

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

class MotorCtrlNode : public rclcpp::Node {
public:
    MotorCtrlNode();

private:
    // ── Action Server ──
    rclcpp_action::Server<ros2_robot_middleware::action::MoveToPose>::SharedPtr action_server_;

    rclcpp_action::GoalResponse
    handle_goal(const rclcpp_action::GoalUUID &uuid,
                std::shared_ptr<const ros2_robot_middleware::action::MoveToPose::Goal> goal);

    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<rclcpp_action::ServerGoalHandle<ros2_robot_middleware::action::MoveToPose>> goal_handle);

    void execute(
        const std::shared_ptr<rclcpp_action::ServerGoalHandle<ros2_robot_middleware::action::MoveToPose>> goal_handle);

    // ── Service Server ──
    rclcpp::Service<ros2_robot_middleware::srv::SetParam>::SharedPtr service_server_;

    void handle_set_param(const std::shared_ptr<ros2_robot_middleware::srv::SetParam::Request> request,
                          std::shared_ptr<ros2_robot_middleware::srv::SetParam::Response> response);

    // ── Speed config ──
    float step_size_ = 0.05F;
};

#endif