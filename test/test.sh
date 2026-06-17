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

# ── Coverage toggle ──────────────────────────────────────────────────
HAS_LCov=false
command -v lcov > /dev/null 2>&1 && HAS_LCov=true
COVERAGE="${COVERAGE:-$HAS_LCov}"  # env override, else auto-detect
if [ "${COVERAGE}" = "1" ] || [ "${COVERAGE}" = "true" ]; then
  COVERAGE_FLAGS="--cmake-args -DCMAKE_CXX_FLAGS=--coverage -DCMAKE_EXE_LINKER_FLAGS=--coverage"
  echo "[coverage] Instrumentation enabled"
else
  COVERAGE_FLAGS=""
fi

# ── Step 1: Build production ─────────────────────────────────────────
echo "=== Building production ==="
cd "${WS_DIR}"
# shellcheck disable=SC2086
colcon build --packages-select ros2_robot_middleware ${COVERAGE_FLAGS}

# ── Step 2: Build test as standalone CMake project ───────────────────
echo "=== Building tests ==="
TEST_BUILD_DIR="${WS_DIR}/build/ros2_robot_middleware_test"
# shellcheck disable=SC2086
cmake -S "${PKG_DIR}/test" -B "${TEST_BUILD_DIR}" \
  -DPROD_BUILD_DIR="${WS_DIR}/build/ros2_robot_middleware" \
  -DCMAKE_PREFIX_PATH="/opt/ros/jazzy;${WS_DIR}/install" \
  ${COVERAGE_FLAGS}
cmake --build "${TEST_BUILD_DIR}"

# ── Step 3: Run ──────────────────────────────────────────────────────
echo "=== Running tests ==="
cd "${TEST_BUILD_DIR}" && ctest --output-on-failure

# ── Step 4: Coverage report (if lcov available) ──────────────────────
if [ "${COVERAGE}" = "1" ] || [ "${COVERAGE}" = "true" ]; then
  if command -v lcov > /dev/null 2>&1; then
    echo "=== Generating coverage report ==="
    COVERAGE_DIR="${PKG_DIR}/mdDoc/coverage"
    mkdir -p "${COVERAGE_DIR}"

    # Capture from both test build and production build
    lcov --capture \
      --directory "${TEST_BUILD_DIR}" \
      --directory "${WS_DIR}/build/ros2_robot_middleware" \
      --output-file "${COVERAGE_DIR}/coverage.info" \
      --rc lcov_branch_coverage=1 \
      --no-external 2>&1 || true

    # Filter to project source only
    lcov --remove "${COVERAGE_DIR}/coverage.info" \
      '/usr/*' '*/gtest*' '*/rosidl*' '*/CMakeCXXCompilerId*' \
      --output-file "${COVERAGE_DIR}/coverage_filtered.info" \
      --rc lcov_branch_coverage=1 2>&1 || true

    # Text summary
    echo ""
    lcov --list "${COVERAGE_DIR}/coverage_filtered.info" --rc lcov_branch_coverage=1 2>&1 || true

    # HTML report
    genhtml "${COVERAGE_DIR}/coverage_filtered.info" \
      --output-directory "${COVERAGE_DIR}/html" \
      --rc lcov_branch_coverage=1 \
      --title "ros2_robot_middleware" 2>&1 || true

    echo ""
    echo "Coverage report: ${COVERAGE_DIR}/html/index.html"
  else
    echo "[coverage] lcov not found — install with: sudo apt install lcov -y"
  fi
fi
