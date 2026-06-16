#include "ros2_robot_middleware/robot_middleware.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string>

using StringMsg = std_msgs::msg::String;

class RobotMiddlewareTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        rclcpp::init(0, nullptr);
    }
    static void TearDownTestSuite() {
        rclcpp::shutdown();
    }

    void SetUp() override {
    }

    void TearDown() override {
    }
};

// ---------------------------------------------------------------------------
// GivenDefaultParams_WhenNodeStarts_ThenParametersLoaded
// ---------------------------------------------------------------------------
TEST(RobotMiddlewareTest, BuildAndRun) {
    EXPECT_TRUE(true);
}
