#ifndef ROS2_ROBOT_MIDDLEWARE_DOMAIN_TARGET_SELECTOR_HPP_
#define ROS2_ROBOT_MIDDLEWARE_DOMAIN_TARGET_SELECTOR_HPP_

#include <cstddef>
#include <cstdint>

namespace amr {
namespace domain {
namespace planning {

// TargetSelector: maps perceived objects → navigation goal.
// Pure domain logic, zero ROS2 dependency.
//
// Current strategy: pick the first (closest) object.
// Future: LLM-based selection via replaceable strategy interface.

struct Goal {
  float x = 0.0F;
  float y = 0.0F;
  float theta = 0.0F;
  float max_speed = 0.5F;
};

struct PerceivedObject {
  float x;
  float y;
  const char *id;
};

class TargetSelector {
public:
  static constexpr int kMaxRetries = 3;

  // Returns a goal for the nearest detected object.
  // Returns false if no objects available.
  bool select(const PerceivedObject *objects, std::size_t count, Goal &out) const {
    if (count == 0 || objects == nullptr) return false;
    out.x = objects[0].x;
    out.y = objects[0].y;
    out.theta = 0.0F;
    out.max_speed = 0.5F;
    return true;
  }

  // Retry policy: should we retry after a rejection?
  bool should_retry(int attempts) const {
    return attempts < kMaxRetries;
  }
};

}  // namespace planning
}  // namespace domain
}  // namespace amr

#endif
