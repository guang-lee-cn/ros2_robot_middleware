# M7：可观测性系统设计 — Traces / Metrics / Logs

> 状态：实施中
> 目标：建立端到端的可观测性体系，热路径延迟 < 50ns，独立于被观测系统

---

## 一、核心设计原则

1. **热路径零分配**：IMU/Lidar callback 内不做 malloc、不做 syscall、不做字符串操作
2. **观测通道独立**：Traces/Metrics/Logs 不与 DDS 共享通道——DDS 挂了不影响观测
3. **嵌入而非独立**：观测代码嵌入 Node 内部，不建独立 Node 传输观测数据
4. **优先复用 ROS2 生态**：不引入新的 SDK 依赖，LTTng 和 Prometheus 都是 ROS2 已集成的方案

---

## 二、技术选型

| 支柱 | 选择 | 理由 |
|------|------|------|
| **Traces** | **LTTng** | ROS2 原生集成（`ros2 trace`），内核级追踪，tracepoint 开销 ~ns 级 |
| **Metrics** | **Prometheus** + `std::atomic` | 复用 ADR-4 HTTP 端点，热路径原子操作 ~10ns |
| **Logs** | **SPSC Ring Buffer**（binary）+ 后台序列化 | 热路径无锁 push ~10ns，JSON 序列化推到后台线程 |
| **Trace 传播** | LTTng session ID | 跨进程自动关联，无需 DDS 消息内传 W3C header |

### 为什么不选 OpenTelemetry

- OTel C++ SDK 有内存分配和线程模型开销
- ROS2 已内建 LTTng tracepoint，rclcpp 层已埋点，无需额外集成
- AMR 是实时嵌入式系统，不是云原生微服务——选型逻辑不同

---

## 三、架构

```
每个 Node 内部：

  实时路径（callback）                非实时路径（后台线程）
  ┌────────────────────┐           ┌──────────────────────┐
  │  业务逻辑            │           │  ring_buffer::pop()   │
  │    │                │           │    │                  │
  │    ├─ LTTng trace_  │           │    ├─ JSON serialize  │
  │    │  point() ~ns   │           │    └─ stdout / file   │
  │    │                │           │                       │
  │    ├─ counter.fetch_ │          │  metrics_loop()       │
  │    │  add(1) ~10ns   │          │    └─ Prometheus gauge│
  │    │                │           │       push (HTTP)     │
  │    └─ ring.push()   │           │                       │
  │       ~10ns          │           │  trace session        │
  └────────────────────┘           │    └─ lttng session    │
                                     │       daemon write    │
                                     └──────────────────────┘
```

**关键隔离**：实时路径只做原子操作和无锁 push，所有 I/O 和序列化在后台线程完成。DDS 故障、磁盘满、Prometheus 拉取超时——都不影响 callback 延迟。

---

## 四、实现计划

### M7.1：基础设施

- [ ] `RingBuffer<T>` — 无锁 SPSC，支持多个 producer 一个 consumer
- [ ] `Tracer` — LTTng tracepoint 封装，`TRACE_SCOPE("fusion::process")` 宏
- [ ] `MetricsRegistry` — 基于 `std::atomic` 的 counter/gauge/histogram
- [ ] `LogWorker` — 后台线程，从 ring buffer 取数据，序列化 JSON，写 stdout

### M7.2：Traces — LTTng 全链路

- [ ] 每个 Node callback 加 span tracepoint
- [ ] Fusion → Decision → MotorCtrl 父子 span 关联（利用 LTTng 进程内上下文）
- [ ] 跨进程 trace 通过 LTTng session ID 自动关联

### M7.3：Metrics — Prometheus 指标

- [ ] `amr_sensor_rate_hz` (Gauge) — 传感器实际频率
- [ ] `amr_fusion_latency_us` (Histogram) — 融合耗时分布
- [ ] `amr_e2e_latency_us` (Histogram) — 端到端延迟
- [ ] `amr_degradation_level` (Gauge) — 当前降级等级
- [ ] `amr_degradation_events_total` (Counter) — 降级事件累计

### M7.4：Logs — 结构化日志

- [ ] 热路径：`LOG_EVENT("fusion_cycle", {latency, object_count})` → ring buffer push
- [ ] 后台线程：ring buffer pop → JSON serialize → stdout
- [ ] JSON 格式包含 trace_id 用于关联

### M7.5：集成验证

- [ ] 启动仿真 → 运行 5 分钟 → `ros2 trace` 有完整调用链
- [ ] Prometheus 端点暴露 Histogram 延迟数据
- [ ] Ctrl+C 后 ring buffer 不丢数据
