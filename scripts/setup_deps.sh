#!/bin/bash
set -e

# ROS2 apt repository (if not already configured)
if ! dpkg -s ros-jazzy-rmw-fastrtps-cpp &>/dev/null; then
  sudo apt update
  sudo apt install -y ros-jazzy-desktop python3-colcon-common-extensions
fi

if ! dpkg -s ros-jazzy-rclcpp-action &>/dev/null; then
    sudo apt install -y ros-jazzy-rclcpp-action
fi

if ! dpkg -s ros-jazzy-action-msgs &>/dev/null; then
    sudo apt install -y ros-jazzy-action-msgs
fi
