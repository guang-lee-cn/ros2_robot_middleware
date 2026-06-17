cmake_minimum_required(VERSION 3.8)
project(ros2_robot_middleware_tests)

# PROD_BUILD_DIR must be injected by test.sh
if(NOT DEFINED PROD_BUILD_DIR)
  message(FATAL_ERROR "PROD_BUILD_DIR not defined. Pass -DPROD_BUILD_DIR=<path> from test.sh")
endif()

# CMAKE_PREFIX_PATH must include the ROS2 install prefix (set by test.sh)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(action_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# Production headers
# CMAKE_SOURCE_DIR == <pkg>/test, so ../include → <pkg>/include
include_directories(
  ${CMAKE_SOURCE_DIR}/../include
  ${PROD_BUILD_DIR}/rosidl_generator_cpp
)

# Production static library
add_library(robot_middleware_lib STATIC IMPORTED)
set_target_properties(robot_middleware_lib PROPERTIES
  IMPORTED_LOCATION ${PROD_BUILD_DIR}/librobot_middleware_lib.a
)

find_library(TYPESUPPORT_CPP
  ros2_robot_middleware__rosidl_typesupport_cpp
  PATHS ${PROD_BUILD_DIR})
find_library(TYPESUPPORT_C
  ros2_robot_middleware__rosidl_typesupport_c
  PATHS ${PROD_BUILD_DIR})

# Discover gtest_vendor via CMAKE_PREFIX_PATH
foreach(_prefix ${CMAKE_PREFIX_PATH})
  if(EXISTS "${_prefix}/src/gtest_vendor/src/gtest-all.cc")
    set(GTEST_VENDOR_DIR "${_prefix}/src/gtest_vendor")
    break()
  endif()
endforeach()
if(NOT GTEST_VENDOR_DIR)
  message(FATAL_ERROR "gtest_vendor not found under CMAKE_PREFIX_PATH")
endif()

add_library(gtest STATIC ${GTEST_VENDOR_DIR}/src/gtest-all.cc)
target_include_directories(gtest PUBLIC
  ${GTEST_VENDOR_DIR}/include
  ${GTEST_VENDOR_DIR})
target_compile_features(gtest PUBLIC cxx_std_11)
target_link_libraries(gtest pthread)

add_library(gtest_main STATIC ${GTEST_VENDOR_DIR}/src/gtest_main.cc)
target_include_directories(gtest_main PUBLIC
  ${GTEST_VENDOR_DIR}/include)

# Test executable
add_executable(test_robot_middleware test_robot_middleware.cpp)
target_link_libraries(test_robot_middleware
  robot_middleware_lib
  gtest gtest_main
  pthread
)
ament_target_dependencies(test_robot_middleware rclcpp std_msgs rclcpp_action action_msgs)
target_link_libraries(test_robot_middleware
  ${TYPESUPPORT_CPP}
  ${TYPESUPPORT_C}
)

enable_testing()
add_test(NAME test_robot_middleware COMMAND test_robot_middleware)
