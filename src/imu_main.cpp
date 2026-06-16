#include "ros2_robot_middleware/imu_node.hpp"

#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    cout << "imu node started" << endl;
    rclcpp::spin(std::make_shared<ImuNode>());
    rclcpp::shutdown();
    return 0;
}
