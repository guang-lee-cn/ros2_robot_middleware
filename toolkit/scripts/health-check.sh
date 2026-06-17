#!/bin/bash
NODE="${1:?Usage: health-check.sh <node_name>}"

source /opt/ros/jazzy/setup.bash
source /ws/install/setup.bash

# Check if node is visible in the ROS2 graph
ros2 node list 2>/dev/null | grep -q "/${NODE}"
