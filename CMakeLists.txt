cmake_minimum_required(VERSION 3.8)
project(ros2_robot_middleware)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(action_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Custom interfaces
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/LidarScan.msg"
  "msg/ImuData.msg"
  "msg/CameraImage.msg"
  "msg/Object.msg"
  "msg/PerceptionObjects.msg"
  "srv/SetParam.srv"
  "action/MoveToPose.action"
  DEPENDENCIES std_msgs action_msgs
)

# Static library — all business logic, linked by both executables and tests
add_library(robot_middleware_lib STATIC
  src/lidar_node.cpp
  src/imu_node.cpp
  src/camera_node.cpp
  src/fusion_node.cpp
  src/decision_node.cpp
  src/motor_ctrl_node.cpp
)

target_include_directories(robot_middleware_lib PUBLIC
  include
  ${CMAKE_CURRENT_BINARY_DIR}/rosidl_generator_cpp
)

ament_target_dependencies(robot_middleware_lib
  rclcpp rclcpp_action std_msgs
)

rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(robot_middleware_lib "${cpp_typesupport_target}")

# Thin executables — one per node
set(NODE_NAMES lidar imu camera fusion decision motor_ctrl)
foreach(node ${NODE_NAMES})
  add_executable(${node}_node src/${node}_main.cpp)
  target_link_libraries(${node}_node robot_middleware_lib)
  install(TARGETS ${node}_node DESTINATION lib/${PROJECT_NAME})
endforeach()

install(DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
