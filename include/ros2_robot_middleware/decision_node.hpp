#ifndef ROS2_ROBOT_MIDDLEWARE_DECISION_NODE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_DECISION_NODE_HPP_

#include "ros2_robot_middleware/action/move_to_pose.hpp"
#include "ros2_robot_middleware/msg/perception_objects.hpp"

#include <rclcpp_action/rclcpp_action.hpp>

#include <rclcpp/rclcpp.hpp>

class DecisionNode : public rclcpp::Node {
public:
    DecisionNode();

private:
    void on_perception(const ros2_robot_middleware::msg::PerceptionObjects::SharedPtr &objs);
    void on_goal_response(
        const rclcpp_action::ClientGoalHandle<ros2_robot_middleware::action::MoveToPose>::SharedPtr &goalhdl);
    void
    on_result(const rclcpp_action::ClientGoalHandle<ros2_robot_middleware::action::MoveToPose>::WrappedResult &result);

    rclcpp::Subscription<ros2_robot_middleware::msg::PerceptionObjects>::SharedPtr decision_sub_;

    rclcpp_action::Client<ros2_robot_middleware::action::MoveToPose>::SharedPtr client_;
};

#endif // ROS2_ROBOT_MIDDLEWARE_DECISION_NODE_HPP_
