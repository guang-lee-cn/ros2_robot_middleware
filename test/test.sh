#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PKG_DIR="$(dirname "$SCRIPT_DIR")"

PARENT="$(dirname "$PKG_DIR")"
if [ "$(basename "$PARENT")" = "src" ] && [ -d "$(dirname "$PARENT")/src" ]; then
  WS_DIR="$(dirname "$PARENT")"
  echo "[dev mode] Workspace: ${WS_DIR}"
else
  WS_DIR="${HOME}/ros2_ws_middleware"
  mkdir -p "${WS_DIR}/src"
  ln -sf "${PKG_DIR}" "${WS_DIR}/src/ros2_robot_middleware"
  echo "[standalone] Workspace: ${WS_DIR}"
fi

source /opt/ros/jazzy/setup.bash
source "${WS_DIR}/install/setup.bash" 2>/dev/null || true

# Step 1: Build production
echo "=== Building production ==="
cd "${WS_DIR}"
colcon build --packages-select ros2_robot_middleware

# Step 2: Build test as standalone CMake project
echo "=== Building tests ==="
TEST_BUILD_DIR="${WS_DIR}/build/ros2_robot_middleware_test"
cmake -S "${PKG_DIR}/test" -B "${TEST_BUILD_DIR}" \
  -DPROD_BUILD_DIR="${WS_DIR}/build/ros2_robot_middleware" \
  -DCMAKE_PREFIX_PATH="/opt/ros/jazzy;${WS_DIR}/install"
cmake --build "${TEST_BUILD_DIR}"

# Step 3: Run
echo "=== Running tests ==="
cd "${TEST_BUILD_DIR}" && ctest --output-on-failure
