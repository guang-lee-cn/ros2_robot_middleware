#ifndef DE1A4315_7241_4C81_A1A1_0C18C85E993A
#define DE1A4315_7241_4C81_A1A1_0C18C85E993A

#include <rclcpp/rclcpp.hpp>

class LidarNode : public rclcpp::Node {
public:
    LidarNode() : Node("lidar") {
    }
};
#endif /* DE1A4315_7241_4C81_A1A1_0C18C85E993A */
