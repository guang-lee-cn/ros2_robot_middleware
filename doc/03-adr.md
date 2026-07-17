# Architecture Decision Records (ADR)

Each record captures the context, alternatives, decision, and rationale behind a significant architectural choice. ADRs provide a historical audit trail and help future maintainers understand why the system is shaped the way it is.

---

## ADR-1: Node Granularity — 8 Independent Nodes

**Date:** 2025-06-10 (updated 2026-07-17)
**Status:** Accepted

### Context

The AMR perception-to-actuation pipeline spans 4 hardware types (LiDAR, IMU, Camera, Motor) and 3 logical layers (fusion, decision, monitoring). A single monolithic node vs. multiple independent nodes was evaluated.

### Alternatives

| Option | Description | Node Count |
|--------|-------------|------------|
| A: Monolithic | Single node handles all logic | 1 |
| B: Layer-based | One node per logical layer | 4 |
| C: Device-based | One node per physical device + logical layers | 8 |

### Decision

**Option C: 8 independent nodes** (7 core pipeline + fleet manager).

### Rationale

1. **Fault isolation (primary driver):** If the IMU driver crashes, Option A takes down the entire system. With Option C, only IMU is unavailable — Fusion degrades to LiDAR odometry instead of stopping completely.
2. **Independent deployment:** Upgrading the Camera driver does not require restarting the entire system.
3. **Per-node QoS:** IMU needs 100Hz reliable; Camera needs 5Hz best-effort. Independent nodes allow independent DDS QoS configuration.
4. **Testability:** Each node can be tested in isolation (the basis for all 13 GWT tests).

Node granularity maps directly to the failure domain boundary. Each device is a separate process with a minimized crash radius, analogous to microservice decomposition principles.

---

## ADR-2: Node Base Class — LifecycleNode

**Date:** 2026-06-21
**Status:** Accepted

### Context

ROS2 provides two node base classes: `rclcpp::Node` (stateless) and `rclcpp_lifecycle::LifecycleNode` (managed state machine).

### Alternatives

| Option | Description |
|--------|-------------|
| A: `rclcpp::Node` | Data-driven; all communication objects created in constructor |
| B: `LifecycleNode` | Built-in state machine; resource lifecycle managed via callbacks |
| C: Custom state machine | State management hand-written on top of `rclcpp::Node` |

### Decision

**Option B: LifecycleNode.**

### Rationale

1. **Startup dependency chain:** Fusion subscribes to 3 sensor topics. If Fusion starts before sensors, it runs with empty data requiring complex "wait for readiness" logic. The inactive→active transition solves this declaratively.
2. **Resource management:** `on_deactivate()` pauses timers (CPU), `on_cleanup()` frees memory — directly mapping to production-grade CPU/memory management.
3. **Ecosystem standard:** navigation2, moveit2, and ros2_control all use LifecycleNode. Following the reference architecture reduces design risk.
4. **Recovery foundation:** External lifecycle service calls (`deactivate→cleanup→configure→activate`) enable watchdog-based node restart (implemented in M2).

### Research methodology

The knowledge did not come from API documentation — it came from reading reference implementations:
- `navigation2` — the largest ROS2 ecosystem project — uses LifecycleNode exclusively
- `moveit2` and `ros2_control` follow the same pattern

These projects share a common scenario: hardware/resource dependency chains. Navigation needs costmaps (depends on sensors); manipulation needs drivers (depends on hardware abstraction). LifecycleNode's inactive→active transition naturally models "wait for dependencies before starting work."

**Rule:** If a node has preconditions ("X must be ready before I can work"), use LifecycleNode.

In our pipeline:
- Fusion requires LiDAR/IMU/Camera to be ready → LifecycleNode
- Decision requires Fusion to be ready → LifecycleNode
- MotorCtrl requires Decision to be ready → LifecycleNode

---

## ADR-3: Motor Control Interface — Action over Service

**Date:** 2025-06-12
**Status:** Accepted

### Context

Decision needs to command MotorCtrl to move to a target pose. The interface must support: sending a target, receiving progress feedback, and mid-task cancellation.

### Alternatives

| Option | Description | Cancellation | Feedback |
|--------|-------------|-------------|----------|
| A: Topic | Publish target position | No | No |
| B: Service | Request-reply | No | No |
| C: Action | Goal-feedback-result | Yes | Yes |

### Decision

**Option C: Action.**

### Rationale

- Service semantics are "ask-then-answer, instant completion" (e.g., parameter queries)
- Action semantics are "goal-feedback-result", matching the physics of motor motion:
  - **Intermediate feedback:** current position, remaining distance, percent complete
  - **Preemption:** new perception data must cancel the in-flight goal immediately
  - **Terminal notification:** success, cancellation, or timeout

Motor motion is a multi-second long-running task. The physical world interrupts — a new obstacle requires immediate goal cancellation and reissuance. This preemption pattern is only supported by the Action protocol.

---

## ADR-4: Health Monitoring Exposure — Prometheus HTTP + ROS2 Topic

**Date:** 2025-06-17
**Status:** Accepted

### Context

HealthMonitor must expose node health data for both internal ROS2 consumers and external operations tooling (Grafana, alerting).

### Alternatives

| Option | Description |
|--------|-------------|
| A: ROS2 Topic only | Publish via DDS, consumed within ROS2 ecosystem |
| B: Prometheus HTTP only | Standard Prometheus text format, HTTP pull |
| C: Dual-channel (A + B) | Both interfaces simultaneously |

### Decision

**Option C: Dual-channel.** `/health/report` for internal ROS2 consumption (other nodes, Fleet Manager); `:9090/metrics` for Prometheus/Grafana operations stack.

### Rationale

The dual-channel approach eliminates an intermediate bridge process. If only ROS2 Topic were used, integrating with Prometheus would require a separate ROS2→Prometheus bridge node that:
1. Subscribes to `/health/report`
2. Deserializes HealthReport messages
3. Converts to Prometheus text format
4. Exposes an HTTP endpoint

Each additional bridge adds a potential crash point, one network hop of latency, and one data transformation step where errors can occur. The embedded HTTP endpoint (implemented with raw TCP sockets in `prometheus_accept()`) eliminates the middle layer. Fewer components on the data path means higher system availability.

---

## ADR-5: Heartbeat Protocol — std_msgs/msg/String

**Date:** 2025-06-14
**Status:** Accepted

### Context

Six business nodes must periodically report liveness to HealthMonitor.

### Alternatives

| Option | Description |
|--------|-------------|
| A: Custom `.msg` file | Define `Heartbeat.msg` with node_id, timestamp, state fields |
| B: `std_msgs/msg/String` | Standard type, simple string payload |
| C: `std_msgs/msg/Empty` | No payload; source identified by topic name only |

### Decision

**Option B: String.**

### Rationale

1. **Minimal interface surface area:** Custom messages require definition, code generation, and ongoing maintenance. String is a ROS2 built-in type with zero maintenance cost.
2. **Payload extensibility:** String can carry state labels ("alive", "idle", "degraded_no_lidar"). Empty cannot. During M2 degradation, nodes publish state-tagged heartbeats without changing the message contract.
3. **Debugging ergonomics:** `ros2 topic echo /sensor/lidar/heartbeat` produces human-readable output without deserialization flags.
4. **Protocol minimization:** Three heartbeat dimensions are carried orthogonally:
   - **Source (who):** DDS topic name — a single publisher cannot publish to two topics
   - **Time (when):** DDS message timestamp — written by the middleware, independent of application clock
   - **Status (what):** String payload — the only dimension that needs application-level data

---

## ADR-6: Data Fusion Strategy — Callback Cache (Current Baseline)

**Date:** 2025-06-16
**Status:** Draft — re-evaluation planned

### Current Implementation

Callback-based cache: each sensor callback updates its latest data snapshot. A periodic timer (200ms) reads all caches and performs fusion.

### Known Limitations

- **No timestamp alignment:** LiDAR data from 50ms ago is fused with just-arrived Camera data without time correction
- **No dropout compensation:** If a sensor misses a cycle, fusion uses stale data silently

### Future Evaluation

| Approach | Description | Computational Cost |
|----------|-------------|-------------------|
| ApproximateTime | message_filters approximate time synchronization | Low |
| ExactTime | Strict timestamp alignment | Medium |
| Kalman Filter | Predict + update, naturally handles asynchrony | High |

A 2D Kalman Filter has been implemented (M3.2) for ego-motion tracking. Full per-object KF tracking with timestamp alignment is a future enhancement.

---

## ADR-7: Executor Model — Single-Threaded (Current Baseline)

**Date:** 2025-06-16
**Status:** Draft — re-evaluation planned

### Current Implementation

All nodes use the default `SingleThreadedExecutor` (created internally by `rclcpp::spin()`).

### Known Limitations

- IMU 100Hz callback execution time > 10ms causes queue buildup
- Callbacks with different priorities (heartbeat vs. data) share the same queue with no preemption

### Future Evaluation

| Executor | Target Nodes |
|----------|-------------|
| `StaticSingleThreadedExecutor` | High-frequency sensor nodes (dedicated CPU core) |
| `MultiThreadedExecutor` | CPU-intensive fusion/decision |
| `SingleThreadedExecutor` | Low-priority health_monitor |

The single-threaded executor was deliberately chosen for Phase 1/2: it eliminates data races between callbacks, simplifies the Kalman Filter integration (no mutex needed on `state_` and `cov_`), and is sufficient for the current sensor simulation load.

---

## ADR-8: QoS Strategy — Per-Topic Differentiation

**Date:** 2025-06-16
**Status:** Accepted — with quantification notes

### Decision

| Topic | QoS | Rationale |
|-------|-----|-----------|
| `/sensor/lidar` | best_effort + keep_last(10) | Tolerates frame loss; 10Hz, 1 dropped frame has no impact |
| `/sensor/camera` | best_effort + keep_last(10) | 5Hz, 900KB payload; reliable would cause head-of-line blocking |
| `/sensor/imu` | reliable + keep_last(10) | 100Hz inertial data; frame loss breaks the integration chain (accumulated error) |
| All heartbeat topics | reliable + keep_last(10) | Heartbeat loss must be detected (triggers alerting) |
| `/perception/objects` | reliable + keep_last(10) | Fusion results must be delivered reliably |
| `/health/report` | reliable + keep_last(10) | Health reports must be delivered reliably |

### Fundamental distinction

The QoS choice maps to the sensor's measurement type:
- **Absolute measurements** (Camera, LiDAR): each frame is a self-contained snapshot of the world. Losing one frame means losing one snapshot — the next frame is still complete. → best_effort
- **Incremental measurements** (IMU): each frame is a delta. Position is computed as `θ = ∫ω·dt`. Losing one frame means losing one integration segment, and the error accumulates permanently across all subsequent frames. → reliable

### Quantification notes (future work)

- best_effort actual frame loss rate with 900KB Camera payload
- reliable end-to-end latency at IMU 100Hz (expected < 2ms)
- `depth=10` adequacy: IMU at 100Hz × 0.01s = 1 frame/callback, 10-frame buffer is sufficient; Camera at 5Hz buffers 90KB × 10 = 900KB

---

## ADR Summary

| ADR | Decision | Maturity |
|-----|----------|----------|
| ADR-1 | 8 nodes, per-device decomposition | Accepted |
| ADR-2 | LifecycleNode base class | Accepted |
| ADR-3 | Action for motor control | Accepted |
| ADR-4 | Prometheus HTTP + ROS2 Topic dual-channel | Accepted |
| ADR-5 | std_msgs/String for heartbeats | Accepted |
| ADR-6 | Callback-cache fusion | Draft — KF integration in progress |
| ADR-7 | Single-threaded executor | Draft — re-evaluate with real sensor load |
| ADR-8 | Per-topic QoS differentiation | Accepted — quantification pending |
