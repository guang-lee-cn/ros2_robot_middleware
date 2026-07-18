#pragma once
/// @file   log_worker.hpp
/// @brief  Background thread: ring buffer consumer → JSON serialize → stdout/file.
///
/// Runs at low priority, decoupled from real-time callbacks.
/// If stdout is blocked or disk is full, ring buffer may overflow —
/// try_push() returns false gracefully, no data corruption.

#include "ring_buffer.hpp"
#include "log_event.hpp"

#include <atomic>
#include <fstream>
#include <iostream>
#include <thread>

namespace amr::observability {

template <size_t Capacity>
class LogWorker {
public:
    using Buffer = RingBuffer<LogEvent, Capacity>;

    explicit LogWorker(Buffer &buf) : buffer_(buf), running_(false) {}

    ~LogWorker() { stop(); }

    LogWorker(const LogWorker &) = delete;
    LogWorker &operator=(const LogWorker &) = delete;

    void start() {
        running_.store(true, std::memory_order_release);
        thread_ = std::thread(&LogWorker::run, this);
    }

    void stop() {
        running_.store(false, std::memory_order_release);
        if (thread_.joinable()) thread_.join();
    }

    /// Dropped events since last query. Indicates ring buffer overflow.
    uint64_t dropped_events() const {
        return dropped_.load(std::memory_order_relaxed);
    }

private:
    void run() {
        while (running_.load(std::memory_order_acquire)) {
            auto maybe_event = buffer_.pop();
            if (maybe_event) {
                write_json(*maybe_event);
            } else {
                // Buffer empty — yield to avoid busy-waiting
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
        // Drain remaining before exit
        drain_remaining();
    }

    void drain_remaining() {
        while (true) {
            auto maybe_event = buffer_.pop();
            if (!maybe_event) break;
            write_json(*maybe_event);
        }
    }

    void write_json(const LogEvent &ev) {
        // Minimal JSON — no heap, fixed buffer
        // Format: {"ts":1234567890,"lvl":"INFO","mod":"fusion","msg":"cycle done","lat_us":230,"extra":5}
        char buf[256];
        const char *level_str = level_to_string(ev.level);
        int n = snprintf(buf, sizeof(buf),
                         R"({"ts":%lu,"lvl":"%s","mod":"%s","msg":"%s","lat_us":%ld,"extra":%d})"
                         "\n",
                         static_cast<unsigned long>(ev.timestamp_ns),
                         level_str,
                         ev.module.data(),
                         ev.message.data(),
                         static_cast<long>(ev.latency_us),
                         ev.extra_value);
        if (n > 0 && static_cast<size_t>(n) < sizeof(buf)) {
            std::cout.write(buf, n);
        } else {
            // Truncated — count as dropped
            dropped_.fetch_add(1, std::memory_order_relaxed);
        }
    }

    static const char *level_to_string(LogLevel lvl) {
        switch (lvl) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default:              return "UNKNOWN";
        }
    }

    Buffer &buffer_;
    std::atomic<bool> running_;
    std::thread thread_;
    std::atomic<uint64_t> dropped_{0};
};

} // namespace amr::observability
