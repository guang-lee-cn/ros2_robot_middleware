#include "ros2_robot_middleware/camera_node.hpp"

#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    cout << "camera node started" << endl;
    rclcpp::spin(std::make_shared<CameraNode>());
    rclcpp::shutdown();
    return 0;
}
