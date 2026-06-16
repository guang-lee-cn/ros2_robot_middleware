#include <rclcpp/rclcpp.hpp>

#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    cout << "decision node started" << endl;
    rclcpp::spin(std::make_shared<rclcpp::Node>("decision_placeholder"));
    rclcpp::shutdown();
    return 0;
}
