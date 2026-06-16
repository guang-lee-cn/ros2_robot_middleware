#!/bin/bash
set -e

PKG_DIR="$(cd "$(dirname "$0")/.." && pwd)"

# Detect if we are already inside a ROS2 workspace (parent has src/)
PARENT="$(dirname "$PKG_DIR")"
if [ "$(basename "$PARENT")" = "src" ] && [ -d "$(dirname "$PARENT")/src" ]; then
  # Case 1: package is inside an existing workspace — build there
  WS_DIR="$(dirname "$PARENT")"
  echo "[dev mode] Building in existing workspace: ${WS_DIR}"
else
  # Case 2: standalone clone — create a temporary workspace with symlink
  WS_DIR="${HOME}/ros2_ws_middleware"
  mkdir -p "${WS_DIR}/src"
  ln -sf "${PKG_DIR}" "${WS_DIR}/src/ros2_robot_middleware"
  echo "[standalone] Created workspace: ${WS_DIR}"
fi

source /opt/ros/jazzy/setup.bash
cd "${WS_DIR}"
colcon build --packages-select ros2_robot_middleware
echo "Done. Source with: source ${WS_DIR}/install/setup.bash"
