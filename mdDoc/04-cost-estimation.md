# 成本核算

## 1. 运行时资源

### 1.1 单节点资源剖面

| 节点 | 频率 | 消息大小 | CPU 负载 | 说明 |
|------|------|---------|---------|------|
| lidar_node | 10Hz | ~1.5 KB (360 floats) | 低 | 每帧 360 次 sin/cos 计算 |
| imu_node | 100Hz | ~100 B | 低 | 简单的三角运算 |
| camera_node | 5Hz | ~900 KB (640x480x3) | 中 | 每帧填充 900KB 随机数据 |
| fusion_node | 5Hz | ~100 B | 低 | 下采样 + 路径点循环 |
| decision_node | 事件驱动 | <1 KB | 极低 | 仅在收到感知数据时触发 |
| motor_ctrl_node | 10Hz feedback | ~100 B | 极低 | 直线插值，100ms 步长 |
| Fast-DDS | — | — | 中 | RTPS 发现、QoS 匹配、CDR 序列化 |

### 1.2 汇总

| 资源 | 最低配置 | 推荐配置 | 说明 |
|------|---------|---------|------|
| CPU | 2 核 | 4 核 | 6 节点都是定时器驱动，大部分时间 idle。2 核足够，但 camera_node 每 200ms 有一次 900KB 序列化，4 核更从容 |
| 内存 | 1 GB | 2 GB | 每个 rclcpp 进程 baseline ~80MB，6 个 ~480MB。Fast-DDS 内部缓冲区 ~200MB。Docker daemon 自身 ~300MB |
| 磁盘 | 5 GB | 10 GB | ROS2 Jazzy base image ~1.5GB，colcon build 产物 ~500MB，Docker layers |
| 网络 | localhost | localhost | 所有节点跑在同一 Docker host，不走物理网卡 |

### 1.3 Docker Compose 资源分配

```yaml
# 6 个容器的资源限制建议
lidar_container:     cpus: 0.3, mem_limit: 150M
imu_container:       cpus: 0.3, mem_limit: 150M
camera_container:    cpus: 0.5, mem_limit: 250M   # 900KB 每帧
fusion_container:    cpus: 0.4, mem_limit: 200M   # 缓存 3 个传感器消息
decision_container:  cpus: 0.2, mem_limit: 150M
motor_ctrl_container:cpus: 0.3, mem_limit: 150M
────────────────────────────────────────────────
合计                  cpus: 2.0, mem: 1.05 GB
```

---

## 2. 开发时间线

### 2.1 假设

- 每 session = 2-3 小时（工作日晚上或周末）
- 每周可投入 2-3 sessions
- 代码由你手写，我 review

### 2.2 逐 Phase 估算

| Phase | 内容 | 估算 | 风险 buffer |
|-------|------|------|------------|
| 5 | 环境搭建 + CI | 1 session | 首次配置 Docker 可能踩环境坑，+1 |
| 6.1 | 传感器层 3 节点 | 2 sessions | lidar/imu/camera 结构相似，第二个起加速 |
| 6.2 | 融合层 | 1 session | 多传感器缓存逻辑 |
| 6.3 | 决策层 | 1 session | Action Client，首次接触 rclcpp_action |
| 6.4 | 执行层 | 1 session | Action Server + Service，含取消处理 |
| 6.5 | RMW 横向对比 | 1 session | Fast-DDS vs Cyclone DDS 性能基准 |
| 6.6 | 实时性扩展 | 1 session | ros2_realtime 确定性通信实验 |
| 7 | 可观测性 | 1 session | 结构化日志 + rqt 监控 |
| 8 | 集成测试 | 2 sessions | 端到端测试 + gtest 覆盖率 |
| 9 | Docker 交付 | 1 session | Docker Compose + 健康检查 |
| 10 | 文档交付 | 1 session | Design Doc 终稿 + CHANGELOG + Release |

### 2.3 里程碑

```
Week 1 (7/16-)  Phase 5:  Docker 环境 + CI 绿钩
Week 2          Phase 6.1: 传感器层 3 节点 + 单测
Week 3          Phase 6.2-6.3: 融合层 + 决策层
Week 4          Phase 6.4-6.6: 执行层 + RMW 对比 + 实时扩展
Week 5          Phase 7-8:   可观测性 + 集成测试
Week 6          Phase 9-10:  Docker 交付 + 文档 + Release Tag
```

| 里程碑 | 可验证产出 |
|--------|-----------|
| Week 2 末 | `./test/test.sh` 跑通传感器层 3 个测试 |
| Week 4 末 | 全链路 6 节点在 Docker 中通信正常 |
| Week 6 末 | `git clone → setup_deps.sh → test.sh` 三步走通，CI 全绿 |

### 2.4 成本总结

| 维度 | 数值 |
|------|------|
| 开发环境 | 笔记本 Ubuntu 24.04 (WSL2)，4 核 8GB+ |
| 部署环境 | Docker (localhost)，2 核 2GB 可跑 |
| 总开发时间 | 12-14 sessions，约 6 周 |
| 金钱成本 | 0（全部开源/免费） |
