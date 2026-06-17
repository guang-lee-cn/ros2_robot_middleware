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
COV_CMAKE="-DCMAKE_CXX_FLAGS=--coverage -DCMAKE_EXE_LINKER_FLAGS=--coverage"
if [ "${COVERAGE}" = "1" ] || [ "${COVERAGE}" = "true" ]; then
  COV_COLCON="--cmake-args ${COV_CMAKE}"  # colcon wraps cmake with --cmake-args
  echo "[coverage] Instrumentation enabled"
else
  COV_COLCON=""
  COV_CMAKE=""
fi

# ── Step 1: Build production ─────────────────────────────────────────
echo "=== Building production ==="
cd "${WS_DIR}"
# shellcheck disable=SC2086
colcon build --packages-select ros2_robot_middleware ${COV_COLCON}

# ── Step 2: Build test as standalone CMake project ───────────────────
echo "=== Building tests ==="
TEST_BUILD_DIR="${WS_DIR}/build/ros2_robot_middleware_test"
# shellcheck disable=SC2086
cmake -S "${PKG_DIR}/test" -B "${TEST_BUILD_DIR}" \
  -DPROD_BUILD_DIR="${WS_DIR}/build/ros2_robot_middleware" \
  -DCMAKE_PREFIX_PATH="/opt/ros/jazzy;${WS_DIR}/install" \
  ${COV_CMAKE}
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

    # Capture from production build (where the business logic lives)
    lcov --capture \
      --directory "${WS_DIR}/build/ros2_robot_middleware" \
      --directory "${TEST_BUILD_DIR}" \
      --output-file "${COVERAGE_DIR}/coverage.info" \
      --rc branch_coverage=1 \
      --ignore-errors mismatch,mismatch 2>&1 | tail -3

    # Keep only project source files
    lcov --remove "${COVERAGE_DIR}/coverage.info" \
      '/usr/*' '/opt/*' '*/rosidl*' '*/CMakeCXXCompilerId*' '*/test/*' \
      --output-file "${COVERAGE_DIR}/coverage_filtered.info" \
      --rc branch_coverage=1 \
      --ignore-errors mismatch,unused 2>&1 | tail -3

    # Text summary
    echo ""
    lcov --list "${COVERAGE_DIR}/coverage_filtered.info" --rc branch_coverage=1 2>&1 | tail -20

    # HTML report (skip if filtered file empty)
    if [ -s "${COVERAGE_DIR}/coverage_filtered.info" ]; then
      genhtml "${COVERAGE_DIR}/coverage_filtered.info" \
        --output-directory "${COVERAGE_DIR}/html" \
        --rc branch_coverage=1 \
        --title "ros2_robot_middleware" 2>&1 | tail -3
      echo ""
      echo "Coverage report: ${COVERAGE_DIR}/html/index.html"
    fi
  else
    echo "[coverage] lcov not found — install with: sudo apt install lcov -y"
  fi
fi
