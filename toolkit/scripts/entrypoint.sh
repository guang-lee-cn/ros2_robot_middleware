#!/bin/bash
set -e

NODE="${1:?Usage: entrypoint.sh <node_name>}"
NODE_EXEC="${NODE}_node"

source /opt/ros/jazzy/setup.bash
source /ws/install/setup.bash

exec "/ws/install/ros2_robot_middleware/lib/ros2_robot_middleware/${NODE_EXEC}"
