#pragma once
/// @file   metrics_registry.hpp
/// @brief  Atomic-based metrics for real-time performance tracking.
///
/// Counter/Gauge use std::atomic — safe for concurrent read/write without locks.
/// Histogram uses lock-free bucket increments, approximations acceptable in hot path.
/// All metrics exposed via existing Prometheus HTTP endpoint (ADR-4).
///
/// Usage in callback:
///   MetricsRegistry::instance().imu_rate.store(actual_hz, memory_order_relaxed);
///   MetricsRegistry::instance().fusion_latency.record(latency_us);

#include <atomic>
#include <cstdint>

namespace amr::observability {

// POD struct for single Histogram bucket. Fixed 64 buckets, log-scale (1us → ~65ms).
struct alignas(64) Histogram {
    static constexpr int kBucketCount = 64;
    static constexpr int64_t kBaseUs = 2;      // bucket[i] covers [base^i, base^(i+1)) μs
    static constexpr int64_t kMaxUs = 1LL << 34; // overflow bucket

    std::atomic<int64_t> buckets[kBucketCount]{};
    std::atomic<int64_t> total_count{0};
    std::atomic<int64_t> total_sum_us{0};

    void record(int64_t latency_us) noexcept {
        total_count.fetch_add(1, std::memory_order_relaxed);
        total_sum_us.fetch_add(latency_us, std::memory_order_relaxed);

        int idx = 0;
        int64_t bound = kBaseUs;
        while (idx < kBucketCount - 1 && latency_us >= bound) {
            ++idx;
            bound *= kBaseUs;
        }
        buckets[idx].fetch_add(1, std::memory_order_relaxed);
    }

    /// Snapshot for Prometheus exposition. Not real-time safe (allocates vector).
    struct Snapshot {
        int64_t p50_us = 0;
        int64_t p99_us = 0;
        int64_t avg_us = 0;
        int64_t max_us = 0;
    };
};

// Singleton registry — one instance per process (compute_container or standalone node).
class MetricsRegistry {
public:
    static MetricsRegistry &instance() {
        static MetricsRegistry reg;
        return reg;
    }

    // === Sensor Rates (Gauge, Hz * 10 to preserve one decimal) ===
    std::atomic<int32_t> lidar_rate_ds{0};   // deci-Hz (e.g. 100 = 10.0Hz)
    std::atomic<int32_t> imu_rate_ds{0};
    std::atomic<int32_t> camera_rate_ds{0};

    // === Latency Histograms (μs) ===
    Histogram fusion_latency;
    Histogram decision_latency;
    Histogram motor_latency;
    Histogram e2e_latency;  // sensor-in → cmd-out

    // === State Gauges ===
    std::atomic<int32_t> degradation_level{0};      // 0=normal .. 4=critical
    std::atomic<int64_t> degradation_events{0};     // monotonic counter
    std::atomic<int64_t> recovery_events{0};
    std::atomic<int32_t> object_count{0};           // current tracked objects
    std::atomic<int64_t> fusion_cycle_count{0};

private:
    MetricsRegistry() = default;
};

} // namespace amr::observability
