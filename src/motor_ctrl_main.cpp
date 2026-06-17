#include "ros2_robot_middleware/motor_ctrl_node.hpp"

#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    cout << "motor_ctrl node started" << endl;
    rclcpp::spin(std::make_shared<MotorCtrlNode>());
    rclcpp::shutdown();
    return 0;
}
