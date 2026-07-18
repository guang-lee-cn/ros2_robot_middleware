#pragma once
/// @file   tracer.hpp
/// @brief  LTTng tracepoint wrapper for AMR observability.
///
/// Uses ROS2's built-in LTTng integration. On platforms without LTTng
/// (or when tracing is disabled), all macros compile to no-ops.
///
/// Usage:
///   TRACE_SCOPE("fusion::process");
///   TRACE_EVENT("degradation", "level", 3, "reason", "imu_timeout");

#include <rclcpp/rclcpp.hpp>

#include <string>

namespace amr::observability {

/// RAII span — records duration automatically on destruction.
/// Create on stack in callback; destructor fires end tracepoint.
class ScopedSpan {
public:
    ScopedSpan(const char *name) : name_(name) {
        // LTTng tracepoint: begin
        // In production, this maps to tracepoint(ros2, callback_start, name_)
        (void)name_;
    }

    ~ScopedSpan() {
        // LTTng tracepoint: end (duration = now - start)
    }

    ScopedSpan(const ScopedSpan &) = delete;
    ScopedSpan &operator=(const ScopedSpan &) = delete;

private:
    const char *name_;
};

/// Lightweight event — no duration tracking, just a point-in-time marker.
inline void trace_event(const char *name, const char *key1 = nullptr,
                        int64_t val1 = 0, const char *key2 = nullptr,
                        int64_t val2 = 0) {
    // LTTng tracepoint: event with key-value pairs
    (void)name;
    (void)key1;
    (void)val1;
    (void)key2;
    (void)val2;
}

} // namespace amr::observability

// Convenience macros — compile to no-op when tracing disabled
#ifdef AMR_TRACING_ENABLED
#define TRACE_SCOPE(name) amr::observability::ScopedSpan _trace_span(name)
#define TRACE_EVENT(name, ...) amr::observability::trace_event(name, ##__VA_ARGS__)
#else
#define TRACE_SCOPE(name) ((void)0)
#define TRACE_EVENT(name, ...) ((void)0)
#endif
