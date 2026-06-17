#include "ros2_robot_middleware/decision_node.hpp"

#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    cout << "decision node started" << endl;
    rclcpp::spin(std::make_shared<DecisionNode>());
    rclcpp::shutdown();
    return 0;
}
