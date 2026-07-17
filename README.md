# ROS2 AMR Middleware — 商用级自主移动机器人中间件

从零构建的感知-融合-决策-执行-监控全链路 ROS2 中间件，覆盖生命周期管理、故障容错、QoS 调优、多机集群、安全加密、Gazebo 仿真。

**目标岗位：** 自变量机器人 / DJI / Geek+ / Amazon Robotics — 机器人软件工程师（C++）

## 架构全景

```
                    ┌──────────────────────────────────────────────┐
                    │              Fleet Manager (M4)              │
                    │   /fleet/status   /fleet/health              │
                    └────┬──────────────────────────┬──────────────┘
                         │ /amr1/health/report      │ /amr2/health/report
    ┌────────────────────┼──────────────────────────┼──────────────────────┐
    │  AMR-1 (domain=1)  │                          │                      │
    │                    ▼                          ▼                      │
    │  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐            │
    │  │  Lidar   │  │   IMU    │  │  Camera  │  │Health Mon│            │
    │  │  10Hz    │  │  100Hz   │  │  5Hz     │  │ watchdog │            │
    │  └────┬─────┘  └────┬─────┘  └────┬─────┘  │ M2 恢复  │            │
    │       │   LaserScan │    Imu      │  Image  └──────────┘            │
    │       ▼             ▼             ▼                                 │
    │  ┌─────────────────────────────────────┐                            │
    │  │           Fusion Node (M2/M3)        │                            │
    │  │  5级降级 + 2D Kalman Filter         │                            │
    │  └──────────────┬──────────────────────┘                            │
    │                 │ PerceptionObjects                                 │
    │                 ▼                                                   │
    │  ┌──────────────────────────┐                                      │
    │  │    Decision Node (M2)    │                                      │
    │  │  预占 + 重试 (Action)     │                                      │
    │  └──────────────┬───────────┘                                      │
    │                 │ MoveToPose Goal                                   │
    │                 ▼                                                   │
    │  ┌──────────────────────────┐                                      │
    │  │   Motor Ctrl (Action)    │                                      │
    │  │  SetParam Service        │                                      │
    │  └──────────────────────────┘                                      │
    └────────────────────────────────────────────────────────────────────┘

    所有节点: rclcpp_lifecycle::LifecycleNode (M1)
    传感器层: sensor_msgs 标准类型 (M3.1) — 可对接 sick_scan2/realsense-ros
    DDS QoS:  per-topic 差异化 + Fast-DDS XML profile (M3.3)
    安全加密: SROS2 per-node enclave + governance.p7s (M5)
    仿真环境: Gazebo Harmonic + AMR SDF模型 (M3.4)
```

## Quick Start

```bash
# ROS 2 Jazzy required
cd ros2_ws
colcon build --packages-select ros2_robot_middleware
source install/setup.bash

# 启动所有 8 个节点
ros2 launch ros2_robot_middleware system.launch.py

# 多 AMR + Fleet Manager
ros2 launch ros2_robot_middleware fleet_multi.launch.py

# 运行 13 个集成测试
colcon test --packages-select ros2_robot_middleware --return-code-on-test-failure

# Gazebo 仿真 (headless — 移除 -s 可启用 GUI)
ros2 launch ros2_robot_middleware simulation.launch.py
```

## 里程碑总览 (Phase 1 → Phase 2)

| 里程碑 | 内容 | 状态 |
|--------|------|------|
| **P1** | 7 节点感知-决策-执行管线 + Docker | ✅ |
| **M1** | 全节点 LifecycleNode 改造 | ✅ |
| **M2** | 5级传感器降级 + 看门狗恢复 + Action 预占 | ✅ |
| **M3.1** | 自定义 msg → sensor_msgs 标准化 | ✅ |
| **M3.2** | 自研 2D Kalman Filter (恒加速度模型) | ✅ |
| **M3.3** | Fast-DDS XML Profile + KF 自适应 dt | ✅ |
| **M3.4** | Gazebo Harmonic 仿真环境 | ✅ |
| **M4** | Fleet Manager + 多 AMR 集群架构 | ✅ |
| **M5** | SROS2 安全加密 (keystore + ACL) | ✅ |
| **M6** | GitHub Actions CI + clang-tidy | ✅ |

## 技术决策 (ADR)

| # | 决策 | 为什么 |
|---|------|--------|
| ADR-1 | 7 节点按设备拆分 | 故障隔离——单传感器 crash 不拖垮融合层 |
| ADR-2 | LifecycleNode | 启动依赖链 + 故障恢复基础 + nav2/ros2_control 标准 |
| ADR-3 | Action 做电机控制 | 长耗时任务需要 Goal→Feedback→Result + 取消 |
| ADR-4 | Prometheus TCP HTTP | 消除 bridge 中间层——数据路径每少一跳就少一个故障点 |
| ADR-5 | std_msgs/String 做心跳 | 来源/时间/状态三维度正交承载——topic 名/DDS 时间戳/字符串 payload |
| ADR-6 | 回调缓存融合 | 当前够用，M3 升级为 EKF+时间戳对齐 |
| ADR-7 | 单线程 Executor | 当前够用，M3 评估多线程方案 |
| ADR-8 | 按 topic 差异化 QoS | IMU reliable / Camera best_effort——绝对测量 vs 增量测量的本质差异 |

详见 [doc/03-adr.md](doc/03-adr.md)

## 接口

### 标准消息类型 (M3.1)
| Topic | 类型 | QoS |
|-------|------|-----|
| `/sensor/lidar` | `sensor_msgs/msg/LaserScan` | best_effort |
| `/sensor/imu` | `sensor_msgs/msg/Imu` | reliable |
| `/sensor/camera` | `sensor_msgs/msg/Image` | best_effort |

### 自定义消息
| Interface | Type | 说明 |
|-----------|------|------|
| `PerceptionObjects` | msg | Object[] objects (融合输出) |
| `HealthStatus` | msg | node_name, status(OK/WARN/ERROR/FATAL/STALE), timeout |
| `HealthReport` | msg | HealthStatus[] nodes (监控报告) |
| `SetParam` | srv | 运行时调参 |
| `MoveToPose` | action | Goal→Feedback→Result (电机控制) |

## 测试

**13 个 GWT 模式集成测试** (GoogleTest + LifecycleNode)

```
Sensor 层:   LidarNode, ImuNode, CameraNode (物理边界验证)
MotorCtrl:   近距离/远距离到达, 已知/未知参数设置
Decision:    感知到目标分发
Fusion:      全部传感器正常 → 检测物体
降级 (M2):   IMU缺失→NO_IMU, LiDAR缺失→NO_LIDAR, 全正常→FULL, 双缺失→CRITICAL
```

覆盖率: `doc/coverage/html/index.html`

## 项目结构

```
ros2_robot_middleware/
├── include/ros2_robot_middleware/
│   ├── lidar_node.hpp             # 传感器层 (sensor_msgs)
│   ├── imu_node.hpp
│   ├── camera_node.hpp
│   ├── fusion_node.hpp            # 融合层 + KF + 降级
│   ├── decision_node.hpp          # 决策层 + 预占
│   ├── motor_ctrl_node.hpp        # 执行层 Action Server
│   ├── health_monitor_node.hpp    # 监控层 + 看门狗
│   ├── fleet_manager_node.hpp     # 集群调度 (M4)
│   ├── kalman_filter.hpp          # 2D 卡尔曼滤波库 (M3.2)
│   └── aliases.hpp                # 类型别名
├── src/                            # 8 node .cpp + 8 main.cpp
├── test/test_robot_middleware.cpp  # 13 GWT 集成测试
├── msg/                            # 自定义消息 (PerceptionObjects, Health*, ...)
├── srv/SetParam.srv
├── action/MoveToPose.action
├── config/
│   ├── params.yaml
│   ├── fastdds_profiles.xml        # per-topic QoS (M3.3)
│   └── sros2/                      # 安全密钥 + 权限 (M5)
├── worlds/                         # Gazebo 仿真 (M3.4)
│   ├── warehouse.sdf
│   └── amr.sdf
├── launch/
│   ├── system.launch.py            # 8 节点启动
│   ├── fleet_multi.launch.py       # 多 AMR + Fleet Manager
│   ├── simulation.launch.py        # Gazebo + ros_gz_bridge
│   └── system_secure.launch.py     # SROS2 加密启动
├── .github/workflows/ci.yml        # CI/CD (M6)
├── .clang-tidy                     # 静态分析规范
├── doc/                            # PRD, 设计文档, ADR, 面试备战
└── toolkit/                        # Docker/Podman 容器化
```

## Tech Stack

| Component | Choice |
|-----------|--------|
| ROS 2 | Jazzy Jalisco (LTS, EOL 2029) |
| RMW | Fast-DDS (eProsima) + XML Profile |
| 消息标准 | sensor_msgs (M3.1) — 互操作 sick_scan2/realsense-ros |
| 构建 | colcon + ament_cmake + static library |
| 测试 | GoogleTest + 13 GWT + LifecycleNode fixture |
| CI/CD | GitHub Actions + clang-tidy (M6) |
| 仿真 | Gazebo Harmonic + ros_gz_bridge (M3.4) |
| 安全 | SROS2 + DDS-Security (M5) |
| 语言 | C++17 |

## Docs

- [PRD](doc/01-prd.md) — 产品需求与用例
- [设计文档](doc/02-design-doc.md) — 拓扑、分层、接口契约
- [ADR 架构决策记录](doc/03-adr.md) — 8 篇技术决策
- [DDS 定制](doc/06-dds-customization.md) — XML profiles, RMW 调优
- [面试备战](doc/interview/自变量机器人/) — 传感器降级、卡尔曼滤波、岗位备战

## License

Apache 2.0
