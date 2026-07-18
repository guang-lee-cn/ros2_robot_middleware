#pragma once
/// @file   ring_buffer.hpp
/// @brief  Lock-free SPSC ring buffer for real-time logging.
///
/// Hot-path push() uses only atomic store/load — no malloc, no syscall, no lock.
/// Background consumer thread pops and serializes to JSON.
///
/// Usage:
///   static RingBuffer<LogEvent, 4096> log_rb;
///   log_rb.try_push(LogEvent{...});  // in IMU callback (~10ns)
///   auto ev = log_rb.pop();          // in background thread

#include <array>
#include <atomic>
#include <cstddef>
#include <optional>

namespace amr::observability {

template <typename T, size_t Capacity>
class RingBuffer {
    static_assert((Capacity & (Capacity - 1)) == 0,
                  "Capacity must be power of 2 for mask-based modulo");

public:
    RingBuffer() : head_(0), tail_(0) {}

    // Non-copyable, non-movable — one instance per buffer
    RingBuffer(const RingBuffer &) = delete;
    RingBuffer &operator=(const RingBuffer &) = delete;

    /// Try to push one element. Returns false if buffer full (consumer too slow).
    /// Called from real-time callback. No blocking, no allocation.
    bool try_push(const T &item) {
        const size_t current_head = head_.load(std::memory_order_relaxed);
        const size_t next_head = (current_head + 1) & mask_;

        if (next_head == tail_.load(std::memory_order_acquire)) {
            return false; // full — consumer can't keep up
        }
        buffer_[current_head] = item;
        head_.store(next_head, std::memory_order_release);
        return true;
    }

    /// Pop one element. Returns nullopt if buffer empty.
    /// Called from background thread. Blocks user-space (not hot path).
    std::optional<T> pop() {
        const size_t current_tail = tail_.load(std::memory_order_relaxed);

        if (current_tail == head_.load(std::memory_order_acquire)) {
            return std::nullopt; // empty
        }
        T item = buffer_[current_tail];
        tail_.store((current_tail + 1) & mask_, std::memory_order_release);
        return item;
    }

    /// Number of elements currently in buffer. Approximate (racy).
    size_t size() const {
        const size_t h = head_.load(std::memory_order_acquire);
        const size_t t = tail_.load(std::memory_order_acquire);
        return (h >= t) ? (h - t) : (Capacity + h - t);
    }

    bool empty() const { return size() == 0; }
    bool full() const { return size() >= Capacity; }

private:
    static constexpr size_t mask_ = Capacity - 1;
    std::array<T, Capacity> buffer_{};
    alignas(64) std::atomic<size_t> head_; // producer cursor — cache line isolation
    alignas(64) std::atomic<size_t> tail_; // consumer cursor — avoids false sharing
};

} // namespace amr::observability
