# Roadmap — Future Work

## Completed (Phase 1–2)

- [x] 8-node perception-to-actuation pipeline
- [x] LifecycleNode migration (M1)
- [x] Sensor degradation + watchdog recovery + action preemption (M2)
- [x] sensor_msgs standardization (M3.1)
- [x] 2D Kalman Filter with Joseph form + Mahalanobis outlier rejection (M3.2–3.3)
- [x] Gazebo Harmonic simulation (M3.4)
- [x] Fleet Manager + multi-AMR (M4)
- [x] SROS2 security (M5)
- [x] CI/CD + clang-tidy (M6)
- [x] compute_container: fusion + decision + motor_ctrl in single process

## Short Term

### End-to-End Latency Benchmark
- Measure: LiDAR data generation → motor_ctrl starts execution
- Compare: independent processes vs compute_container
- Expected: ~2ms improvement from eliminating DDS serialization hops
- Tools: `ros2 topic delay`, custom timestamp injection

### MotorCtrl Heartbeat Fix
- Issue: health_monitor reports motor_ctrl as STALE inside compute_container
- Root cause investigation: timer scheduling in MultiThreadedExecutor vs status publish rate

## Medium Term

### SLAM / nav2 Integration Feasibility
- Replace simple callback-cache fusion with `slam_toolbox` (2D LiDAR SLAM)
- Replace DecisionNode rule engine with `nav2` behavior tree
- Interface: PerceptionObjects → nav2 CostmapLayer
- Dependency: `ros-jazzy-slam-toolbox`, `ros-jazzy-navigation2`

### Executor Model Optimization
- Sensor nodes: `StaticSingleThreadedExecutor` for deterministic scheduling
- compute_container: current `MultiThreadedExecutor` — measure thread contention
- Health monitor: separate `SingleThreadedExecutor` for isolation

## Long Term

### LLM-Based Decision Engine
- Replace rule-based `DecisionNode::on_perception()` with LLM inference
- Architecture:
  ```
  PerceptionObjects → JSON prompt → llama.cpp (local) → target (x, y)
  ```
- Reserved interface: `send_goal(target_x, target_y)` unchanged
- Fallback: rule engine if LLM unavailable

### Real Sensor Driver Integration
- LiDAR: `sick_scan2` → publishes `sensor_msgs/msg/LaserScan` (already compatible)
- IMU: `bno055` → publishes `sensor_msgs/msg/Imu` (already compatible)
- Camera: `realsense-ros` → publishes `sensor_msgs/msg/Image` (already compatible)
- Required: Gazebo GPU mode or physical hardware

### Multi-AMR Production Deployment
- ROS_DOMAIN_ID isolation per AMR
- Fast-DDS Partitions for Fleet Manager cross-domain communication
- MQTT Bridge to cloud (Grafana dashboard)
- Docker Compose multi-instance with `network_mode: host`
