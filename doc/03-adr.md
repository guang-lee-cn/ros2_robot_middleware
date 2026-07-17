# Architecture Decision Records (ADR)

## 为什么需要 ADR

在架构设计过程中，每个关键技术选型都面临多个备选方案。ADR 记录了每次决策的上下文、备选方案、最终选择和取舍理由，形成可追溯的技术决策链。当项目演进或新成员加入时，可以通过 ADR 理解系统为什么是今天这个样子，而不是重新争论已经讨论过的问题。

---

## ADR-1: 节点粒度 —— 7 个独立节点

**Date:** 2025-06-10
**Status:** Accepted
**Decision maker:** guang

### 上下文

AMR 感知-决策-执行流水线涉及 4 类硬件（LiDAR/IMU/Camera/Motor）和 3 层逻辑（融合/决策/监控）。

### 备选方案

| 方案 | 描述 | 节点数 |
|------|------|--------|
| A: Monolithic | 单节点处理所有逻辑 | 1 |
| B: 按层拆分 | Sensor/Fusion/Decision/Actuation 各一层 | 4 |
| C: 按设备拆分 | 每个物理设备独立 + 逻辑层独立 | 7 |

### 决策

**选 C: 7 个独立节点。**

### 工程理由

1. **故障隔离（核心考量）**：如果 IMU 驱动 crash，节点 A 整个系统宕机；节点 C 只有 IMU 不可用，Fusion 可以降级到 LiDAR 里程计
2. **独立部署与更新**：升级 Camera 驱动不需要重启整个系统
3. **QoS 差异化**：IMU 需要 100Hz reliable，Camera 只需 5Hz best_effort——独立节点可以独立配置 DDS QoS
4. **可测试性**：每个节点可以独立测试（这正是我们 9 个 GWT 测试的基础）

### 设计原则

节点粒度直接映射到故障域边界——每个设备一个独立进程，崩溃半径最小化。这和微服务拆分按 failure domain 切边界的原则一致。

---

## ADR-2: 节点基类 —— LifecycleNode

**Date:** 2026-06-21
**Status:** Accepted
**Decision maker:** guang

### 上下文

ROS2 提供两种节点基类：`rclcpp::Node`（无状态管理）和 `rclcpp_lifecycle::LifecycleNode`（带状态机）。

### 备选方案

| 方案 | 描述 |
|------|------|
| A: rclcpp::Node | 纯数据驱动，构造函数中创建所有通信对象 |
| B: rclcpp_lifecycle::LifecycleNode | ROS2 内置状态机，生命周期回调管理资源 |
| C: 自己写状态机 | 在 Node 基础上手写状态管理 |

### 决策

**选 B: LifecycleNode。**

### 工程理由

1. **启动顺序问题**：Fusion 订阅了 LiDAR/IMU/Camera 三个 topic。如果 Fusion 先启动而传感器未启动，Fusion 会在无数据状态下运行，需要额外的"等待所有传感器就绪"逻辑。Lifecycle 通过 inactive→active 解决了这个问题
2. **资源管理**：`on_deactivate()` 暂停 timer（停止 CPU 开销），`on_cleanup()` 释放内存——这在生产环境中直接对应 CPU/内存的降额管理
3. **nav2/ros2_control/moveit2 全基于 LifecycleNode**：这是 ROS2 生态的"标准答案"，不是我们自己发明的
4. **故障恢复基础**：外部可以通过 lifecycle service 调用 `deactivate→cleanup→configure→activate` 重启节点，这正是 M2 要做的看门狗恢复

### 领域经验映射

LifecycleNode 的状态机（unconfigured→inactive→active→finalized）和通信设备的状态管理同构——先配置、再激活、降级时先停再用。这种模式在电信级系统中经过了大规模验证，移植到 AMR 节点管理上同样适用。

### 深入：为什么我们知道有 LifecycleNode 这个能力存在

**技术调研路径（任何一个资深的 ROS2 开发人员都知道的路径）：**

1. **知识来源不是 API 文档，是参考实现。** ROS2 的"参考架构"（Reference Systems）和 nav2 是两个关键来源：
   - `navigation2`（GitHub 上 ROS2 生态最大的项目之一）所有节点都是 LifecycleNode
   - `moveit2`（机械臂运动规划）也是全 LifecycleNode
   - `ros2_control`（硬件抽象层）也是全 LifecycleNode

2. **为什么这些大项目都选 LifecycleNode？**
   因为它们都有一个共同场景：**硬件/资源依赖链**。导航需要 costmap（依赖传感器），机械臂需要驱动（依赖硬件抽象层）。LifecycleNode 的 inactive→active 转换天然适合表达"等依赖就绪再开始工作"。

3. **规则：如果节点有执行条件（"必须 X 先就绪我才能工作"），就应该用 LifecycleNode。**

   在我们的项目中：
   - Fusion 必须 LiDAR/IMU/Camera 都就绪才能融合 → LifecycleNode
   - Decision 必须 Fusion 先就绪 → LifecycleNode
   - MotorCtrl 必须 Decision 先就绪 → LifecycleNode

---

## ADR-3: 电机控制接口 —— Action 而非 Service

**Date:** 2025-06-12
**Status:** Accepted
**Decision maker:** guang

### 上下文

Decision 需要命令 MotorCtrl 移动到目标坐标。涉及 3 个需求：发送目标、接收进度反馈、支持中途取消。

### 备选方案

| 方案 | 描述 | 支持取消 | 支持反馈 |
|------|------|---------|---------|
| A: Topic `geometry_msgs/PoseStamped` | 发布目标位置 | 否 | 否 |
| B: Service `MoveToPose.srv` | 请求-回复 | 否 | 否 |
| C: Action `MoveToPose.action` | 目标-反馈-结果 | 是 | 是 |

### 决策

**选 C: Action。**

### 工程理由

- Service 的语义是"一问一答，瞬时完成"（如参数设置、状态查询）
- Action 的语义是"目标-反馈-结果"，匹配电机运动的物理特性：
  - **中间反馈**：当前位置、剩余距离、完成百分比
  - **可取消性**：新感知数据到达时需立即中止旧目标
  - **结果通知**：成功到达、被取消、超时失败

### 设计原则

电机运动是秒级的长耗时操作，物理世界存在不确定性——新障碍物出现时需要立刻取消当前目标、发送新目标。Action 的 Goal-Feedback-Result 三阶段协议是 ROS2 生态中唯一支持这种 preemption 模式的通信机制。

---

## ADR-4: 健康监控暴露 —— Prometheus HTTP vs ROS2 Topic

**Date:** 2025-06-17
**Status:** Accepted
**Decision maker:** guang

### 上下文

HealthMonitor 需要对外暴露节点健康数据，供运维系统消费。

### 备选方案

| 方案 | 描述 |
|------|------|
| A: ROS2 Topic `/health/report` | 通过 DDS 发布，ROS2 生态内消费 |
| B: Prometheus HTTP `:9090/metrics` | 标准 Prometheus 文本格式，HTTP 拉取 |
| C: A + B 双通道 | 同时提供两种接口 |

### 决策

**选 C: 双通道。** `/health/report` 供 ROS2 内部消费（其他节点、Fleet Manager），`:9090/metrics` 供 Prometheus/Grafana 运维栈消费。

### 为什么选择直接嵌入式 HTTP 而非 bridge 进程

如果只用 ROS2 Topic 暴露指标，对接 Prometheus 时就需要额外写一个 ROS2→Prometheus bridge 节点：订阅 `/health/report` → 解析 HealthReport → 转 Prometheus 格式 → 开 HTTP 端口。每多一个 bridge 就多一个故障点、多一跳延迟、多一处数据转换风险。

我们在 `prometheus_accept()` 中用原始 TCP socket 直接实现 HTTP endpoint，消除了中间层。这是通信设备的设计原则：数据采集路径上每少一个组件，系统可用性就上一个台阶。

---

## ADR-5: 心跳协议 —— std_msgs/msg/String

**Date:** 2025-06-14
**Status:** Accepted
**Decision maker:** guang

### 上下文

6 个业务节点需要周期性地向 HealthMonitor 报告存活状态。

### 备选方案

| 方案 | 描述 |
|------|------|
| A: 自定义 `.msg` 文件 | 定义 `Heartbeat.msg`，包含 node_id, timestamp, state 等字段 |
| B: `std_msgs/msg/String` | 用标准消息类型，payload 为简单字符串 |
| C: `std_msgs/msg/Empty` | 无 payload，仅靠 topic 名区分来源 |

### 决策

**选 B: String。**

### 工程理由

1. **最小化了接口表面积**：自定义 msg 需要定义、生成、维护。String 是 ROS2 内置类型，零维护成本
2. **payload 扩展性**：String 可以携带状态信息（"alive", "idle", "degraded"），Empty 做不到。未来 M2 降级策略中，节点可以发布 "degraded_lidar_only" 而不是简单的 "alive"
3. **调试友好**：`ros2 topic echo /sensor/lidar/heartbeat` 直接看到人类可读的字符串，不需要 `--raw` 反序列化
4. **协议最小化原则**：心跳的"来源"由 DDS topic 名承载（`/sensor/lidar/heartbeat`），"时间"由 DDS 时间戳承载，不需要在 payload 里重复。String 只承载一个"状态标签"——信息不冗余

### 设计原则

使用 ROS2 内置的 `std_msgs/String` 而非自定义 Heartbeat 消息，体现了"够用就好"的工程判断。心跳的三个维度——来源（who）、时间（when）、状态（what）——分别由 DDS topic 名、DDS 时间戳、字符串 payload 三个正交机制承载，信息不冗余。不创建自定义消息意味着更少的接口表面积、更低的维护成本和更简单的调试体验。

---

## ADR-6: 数据融合策略 —— 回调缓存（当前）

**Date:** 2025-06-16
**Status:** Draft（计划 M3 重评估）

### 当前决策

基于回调的简单缓存：每个传感器回调更新对应的最新数据缓存，timer 触发时读取所有缓存做融合。

### 已知局限

- **无时间戳对齐**：LiDAR 50ms 前的数据和 Camera 刚到的数据直接融合，无人驾驶场景不可接受
- **无丢帧补偿**：某个传感器丢了一个采样周期，融合会使用旧数据而不自知

### M3 拟评估方案

| 方案 | 描述 | 计算开销 |
|------|------|---------|
| ApproximateTime | message_filters 近似时间同步 | 低 |
| ExactTime | 严格时间戳对齐 | 中 |
| Kalman Filter | 预测+更新，自然处理异步 | 高 |

---

## ADR-7: Executor 模型 —— 默认单线程（当前）

**Date:** 2025-06-16
**Status:** Draft（计划 M3 重评估）

### 当前决策

所有节点使用默认 `SingleThreadedExecutor`（`rclcpp::spin()` 内部创建）。

### 已知局限

- IMU 100Hz 回调如果执行时间 > 10ms，后续回调排队积压
- 不同优先级的回调（心跳 vs 数据）在同一队列中，无法抢占

### M3 拟评估方案

| 方案 | 适用节点 |
|------|---------|
| `StaticSingleThreadedExecutor` | 高频 sensor 节点（独立 CPU 核） |
| `MultiThreadedExecutor` | CPU 密集的 fusion/decision |
| `SingleThreadedExecutor` | 低优先级 health_monitor |

---

## ADR-8: QoS 选择 —— 按 topic 差异化

**Date:** 2025-06-16
**Status:** Draft（计划 M3 量化）

### 当前决策

| Topic | QoS | 理由 |
|-------|-----|------|
| `/sensor/lidar` | best_effort + keep_last(10) | 允许丢帧，10Hz 丢 1 帧不影响 |
| `/sensor/camera` | best_effort + keep_last(10) | 5Hz 大 payload(900KB)，reliable 会阻塞 |
| `/sensor/imu` | reliable + keep_last(10) | 100Hz 惯性数据，丢帧影响位姿推算 |
| 所有心跳 topic | reliable + keep_last(10) | 心跳丢失必须被感知（触发告警） |
| `/perception/objects` | reliable + keep_last(10) | 融合结果必须可靠传递 |
| `/health/report` | reliable + keep_last(10) | 健康报告必须可靠传递 |

### M3 待量化验证

- best_effort 在 900KB camera payload 下的实际丢帧率
- reliable 在 IMU 100Hz 下的端到端延迟（预期 < 2ms）
- `depth=10` 是否足够（IMU 100Hz × 0.01s = 1 帧/回调，10 帧 buffer 绰绰有余；Camera 5Hz 下 buffer 90KB×10=900KB）

---

## ADR 总结

| ADR | 决策 | 成熟度 |
|-----|------|--------|
| ADR-1 | 7 节点按设备拆分 | Accepted |
| ADR-2 | LifecycleNode 基类 | Accepted |
| ADR-3 | Action 做电机控制 | Accepted |
| ADR-4 | Prometheus HTTP + ROS2 Topic 双通道 | Accepted |
| ADR-5 | std_msgs/String 做心跳 | Accepted |
| ADR-6 | 回调缓存融合（待升级） | Draft |
| ADR-7 | 默认 Executor（待升级） | Draft |
| ADR-8 | 按 topic 差异化 QoS（待量化） | Draft |

**Accepted = 已决策，可以讲清楚为什么。Draft = 当前实现够用，但 M3 会重新评估。**

---

## ADR-9: 状态估计库选型 — 自研 KF vs 开源方案

**Date:** 2026-07-17
**Status:** Accepted
**Decision maker:** guang

### 上下文

当前项目使用自研的 4 状态（x, y, vx, vy）恒加速度线性卡尔曼滤波器。需要评估是否应切换到商用级开源方案。

### 备选方案

| 方案 | 描述 | 状态维度 | 依赖 | 安装方式 |
|------|------|---------|------|---------|
| A: robot_localization | ROS2 生态标准，nav2 集成，15 维 EKF/UKF | 15 | Eigen + tf2 + ROS2 | sudo apt |
| B: FusionCore | 2025.05 新发布，23 维 UKF + bias 估计，Apache 2.0 | 23 | Eigen (header-only) | vendor 或 apt |
| C: Fuse | 因子图优化，Google Ceres 后端 | 可变 | Ceres + ROS2 插件 | sudo apt |
| D: 自研 KF (保留) | 4 维线性 KF，零外部依赖 | 4 | 无 | 项目内 |

### 决策

**选 D: 保留自研 4 状态 KF，修复数值稳定性/野值剔除后达到商用级标准。**

生产环境如需升级，推荐方案 B (FusionCore)。

### 评估矩阵

| 评估维度 | A: robot_localization | B: FusionCore | C: Fuse | D: 自研 (改进后) |
|---------|----------------------|---------------|---------|-----------------|
| 生产验证 | ✅ nav2 集成 | ⚠️ 2025.05 新发布 | ✅ | ✅ 13 测试通过 |
| 状态模型匹配度 | ❌ 15 维过度 | ❌ 23 维过度 | ⚠️ 可配 | ✅ 4 维刚好 |
| 野值剔除 | ⚠️ 手动标量阈值 | ✅ Mahalanobis chi-squared | ✅ | ✅ Mahalanobis 3σ |
| 数值稳定性 | ✅ | ✅ Joseph 形式 | ✅ | ✅ Joseph 形式 |
| bias 在线估计 | ❌ 不支持 | ✅ gyro/accel/encoder | ❌ | N/A (无 bias 状态) |
| 零外部依赖 | ❌ | ❌ (需要 Eigen) | ❌ | ✅ |
| 可安装性 | ❌ 需 sudo | ⚠️ vendor Eigen | ❌ 需 sudo | ✅ 项目内 |
| 2D AMR 适配度 | ⚠️ two_d_mode | ⚠️ 四元数过度 | ⚠️ | ✅ 专为 2D 设计 |

### 为什么不用 FusionCore（当前阶段）

1. **模型过度**：23 维 UKF 包含四元数姿态、gyro bias、accel bias、encoder bias——这些是为室外 GPS 导航设计的。仓库 AMR 在平面上行驶，不需要姿态估计和 bias 在线标定。
2. **依赖引入**：FusionCore 依赖 Eigen3（头文件库，约 2MB header-only）。当前项目刻意保持零外部数学库依赖，Eigen 的引入增加了构建复杂度。
3. **调试成本**：23 维 UKF 发散时的调试难度远大于 4 维 KF——需要在 23×23 协方差矩阵中定位发散源。
4. **网络不可用**：GitHub clone 超时，vendor 成本高。

### 自研 KF 的商用级改进（v1.1.0）

针对初版三个缺陷做了定向修复：

| 缺陷 | 初版 | 改进后 |
|------|------|--------|
| 数值稳定性 | `P = (I-KH)P` 标准形式，浮点累积误差可能导致 P 非对称正定 | **Joseph 形式**: `P = (I-KH)P(I-KH)^T + KRK^T`，始终对称正定 |
| 野值剔除 | 无，镜面反射/噪点产生的异常聚类中心直接纳入更新 | **Mahalanobis 距离 3σ 门限**: `|z-Hx| > 3√(P_ii+R)` 时拒绝更新 |
| 多目标跟踪 | 全局单例，只跟踪一个物体 | 当前阶段接受——DecisionNode 只取 objects[0] |

### 生产环境升级路径

当以下条件任一满足时，切换到 FusionCore：
1. 引入 GPS/RTK 做室外定位，需要在线估计 IMU bias
2. 引入轮式编码器，需要 encoder WZ bias 估计
3. 多传感器异步到达，需要 UKF 的非线性测量模型（替代线性 H 矩阵）
4. 构建环境有 apt 包管理器

切换时只需替换 `kalman_filter.hpp` 的内部实现，FusionNode 的调用代码（`predict(dt, ax, ay)` + `update(zx, zy)`）保持不变。

### 参考文献

- robot_localization: [https://github.com/cra-ros-pkg/robot_localization](https://github.com/cra-ros-pkg/robot_localization)
- FusionCore: [https://github.com/manankharwar/fusioncore](https://github.com/manankharwar/fusioncore)
- Moore & Stouch (2014): "A Generalized Extended Kalman Filter Implementation for the Robot Operating System"
