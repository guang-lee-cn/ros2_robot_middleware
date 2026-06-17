#!/bin/bash
set -e

# ── RMW Benchmark: Fast-DDS vs Cyclone DDS ────────────────────────
# Uses ros2 topic hz / delay (C++ subscriber, no Python per-echo overhead)
#
# Tests:
#   1. best_effort message rate  — lidar 10Hz, ros2 topic hz 30s
#   2. best_effort header delay  — lidar header.stamp vs arrival
#   3. reliable header delay     — imu header.stamp vs arrival
#   4. discovery time            — cold start → first message

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PKG_DIR="$(dirname "$SCRIPT_DIR")"
WS_DIR="$(dirname "$(dirname "$PKG_DIR")")"
source /opt/ros/jazzy/setup.bash
source "${WS_DIR}/install/setup.bash" 2>/dev/null || true

RESULTS_DIR="${PKG_DIR}/mdDoc/benchmark_data"
mkdir -p "${RESULTS_DIR}"

cleanup() {
  pkill -f ros2_robot_middleware 2>/dev/null || true
  pkill -f "ros2 topic hz"      2>/dev/null || true
  pkill -f "ros2 topic delay"   2>/dev/null || true
  sleep 1
}

run_test() {
  local rmw_name="$1"
  local rmw_env="$2"

  export RMW_IMPLEMENTATION="${rmw_env}"
  cleanup

  echo ""
  echo "============================================================"
  echo "  RMW: ${rmw_name}"
  echo "============================================================"

  # ── 1. Discovery time ───────────────────────────────────────────
  echo "[1/4] Discovery time..."
  ros2 run ros2_robot_middleware lidar_node --ros-args -r __node:=b_lidar > /dev/null 2>&1 &
  NODE_PID=$!

  T0=$(date +%s%N)
  for i in $(seq 1 50); do
    if timeout 1 ros2 topic echo /sensor/lidar --qos-reliability best_effort --once 2>/dev/null | grep -q "angle_min"; then
      break
    fi
  done
  T1=$(date +%s%N)
  DISC_MS=$(( (T1 - T0) / 1000000 ))

  kill ${NODE_PID} 2>/dev/null || true
  wait ${NODE_PID} 2>/dev/null || true
  echo "  Discovery: ${DISC_MS} ms"

  # ── 2. Best-effort message rate & loss ──────────────────────────
  echo "[2/4] Best-effort rate & loss (30s) ..."
  ros2 run ros2_robot_middleware lidar_node --ros-args -r __node:=b_lidar > /dev/null 2>&1 &
  NODE_PID=$!
  sleep 1

  HZ_OUT=$(mktemp)
  timeout 30 ros2 topic hz /sensor/lidar --spin-time 1 --window 30 2>&1 | tee "${HZ_OUT}" || true

  AVG_HZ=$(grep "average rate:" "${HZ_OUT}" | tail -1 | awk '{print $3}')
  [ -z "${AVG_HZ}" ] && AVG_HZ="N/A"
  LOSS_PCT=$(echo "scale=2; 100.0 - (100.0 * ${AVG_HZ} / 10.0)" | bc 2>/dev/null || echo "N/A")
  echo "  Rate: ${AVG_HZ}/10 Hz  Loss: ${LOSS_PCT}%"

  kill ${NODE_PID} 2>/dev/null || true
  wait ${NODE_PID} 2>/dev/null || true
  rm -f "${HZ_OUT}"

  # ── 3. Best-effort header delay ────────────────────────────────
  echo "[3/4] Best-effort header delay (30s) ..."
  ros2 run ros2_robot_middleware lidar_node --ros-args -r __node:=b_lidar > /dev/null 2>&1 &
  NODE_PID=$!
  sleep 1

  DELAY_OUT=$(mktemp)
  timeout 30 ros2 topic delay /sensor/lidar --spin-time 1 --window 30 2>&1 | tee "${DELAY_OUT}" || true

  # ros2 topic delay averages: parse the summary line
  AVG_DELAY=$(grep "average delay:" "${DELAY_OUT}" | tail -1 | awk '{print $3}')
  [ -z "${AVG_DELAY}" ] && AVG_DELAY="N/A"
  echo "  Best-effort avg delay: ${AVG_DELAY}s"

  kill ${NODE_PID} 2>/dev/null || true
  wait ${NODE_PID} 2>/dev/null || true
  rm -f "${DELAY_OUT}"

  # ── 4. Reliable header delay ────────────────────────────────────
  echo "[4/4] Reliable header delay (30s) ..."
  ros2 run ros2_robot_middleware imu_node --ros-args -r __node:=b_imu > /dev/null 2>&1 &
  NODE_PID=$!
  sleep 1

  RELAY_OUT=$(mktemp)
  timeout 30 ros2 topic delay /sensor/imu --spin-time 1 --window 30 2>&1 | tee "${RELAY_OUT}" || true

  AVG_RELAY=$(grep "average delay:" "${RELAY_OUT}" | tail -1 | awk '{print $3}')
  [ -z "${AVG_RELAY}" ] && AVG_RELAY="N/A"
  echo "  Reliable avg delay: ${AVG_RELAY}s"

  kill ${NODE_PID} 2>/dev/null || true
  wait ${NODE_PID} 2>/dev/null || true
  rm -f "${RELAY_OUT}"

  cleanup

  # ── Save ─────────────────────────────────────────────────────────
  echo "${rmw_name},${DISC_MS},${LOSS_PCT},${AVG_DELAY},${AVG_RELAY}" > "${RESULTS_DIR}/${rmw_name}.csv"
}

echo "RMW Benchmark — $(date)"

run_test "Fast-DDS"   ""
run_test "CycloneDDS" "rmw_cyclonedds_cpp"

# ── Summary ──────────────────────────────────────────────────────
echo ""
echo "============================================================"
echo "  Results"
echo "============================================================"
printf "%-14s %8s %8s %14s %14s\n" "RMW" "Disc(ms)" "Loss(%)" "BE_delay(s)" "REL_delay(s)"
printf "%-14s %8s %8s %14s %14s\n" "────" "──────" "──────" "──────────" "──────────"
for csv in "${RESULTS_DIR}"/*.csv; do
  IFS=',' read -r name disc loss be rel < "${csv}"
  printf "%-14s %8s %8s %14s %14s\n" "${name}" "${disc}" "${loss}" "${be}" "${rel}"
done

echo ""
echo "Done. Raw data: ${RESULTS_DIR}"
