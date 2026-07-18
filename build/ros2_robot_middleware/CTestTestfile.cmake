# CMake generated Testfile for 
# Source directory: /home/guang/code/ros2_ws/src/ros2_amr_framework
# Build directory: /home/guang/code/ros2_ws/src/ros2_amr_framework/build/ros2_robot_middleware
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_robot_middleware "/usr/bin/python3" "-u" "/opt/ros/jazzy/share/ament_cmake_test/cmake/run_test.py" "/home/guang/code/ros2_ws/src/ros2_amr_framework/build/ros2_robot_middleware/test_results/ros2_robot_middleware/test_robot_middleware.gtest.xml" "--package-name" "ros2_robot_middleware" "--output-file" "/home/guang/code/ros2_ws/src/ros2_amr_framework/build/ros2_robot_middleware/ament_cmake_gtest/test_robot_middleware.txt" "--command" "/home/guang/code/ros2_ws/src/ros2_amr_framework/build/ros2_robot_middleware/test_robot_middleware" "--gtest_output=xml:/home/guang/code/ros2_ws/src/ros2_amr_framework/build/ros2_robot_middleware/test_results/ros2_robot_middleware/test_robot_middleware.gtest.xml")
set_tests_properties(test_robot_middleware PROPERTIES  LABELS "gtest" REQUIRED_FILES "/home/guang/code/ros2_ws/src/ros2_amr_framework/build/ros2_robot_middleware/test_robot_middleware" TIMEOUT "60" WORKING_DIRECTORY "/home/guang/code/ros2_ws/src/ros2_amr_framework/build/ros2_robot_middleware" _BACKTRACE_TRIPLES "/opt/ros/jazzy/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/jazzy/share/ament_cmake_gtest/cmake/ament_add_gtest_test.cmake;95;ament_add_test;/opt/ros/jazzy/share/ament_cmake_gtest/cmake/ament_add_gtest.cmake;93;ament_add_gtest_test;/home/guang/code/ros2_ws/src/ros2_amr_framework/CMakeLists.txt;91;ament_add_gtest;/home/guang/code/ros2_ws/src/ros2_amr_framework/CMakeLists.txt;0;")
subdirs("ros2_robot_middleware__py")
subdirs("ros2_robot_middleware__rs")
subdirs("gtest")
