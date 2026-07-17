#ifndef ROS2_ROBOT_MIDDLEWARE_DOMAIN_INTERPOLATOR_HPP_
#define ROS2_ROBOT_MIDDLEWARE_DOMAIN_INTERPOLATOR_HPP_

#include <cmath>

namespace amr {
namespace domain {
namespace execution {

// 2D linear interpolation toward a target at fixed step size.
// Pure domain logic, extracted from motor_ctrl_node's execute().
class Interpolator {
public:
  struct State {
    float x = 0.0F;
    float y = 0.0F;
  };

  struct Feedback {
    float current_x;
    float current_y;
    float distance_remaining;
    float percent_complete;
  };

  explicit Interpolator(float step_size = 0.05F) : step_size_(step_size) {}

  void set_step_size(float s) { step_size_ = s; }
  float step_size() const { return step_size_; }

  // Step toward target. Returns true if reached.
  bool step(const State &target, State &current, Feedback *fb = nullptr) {
    float dx   = target.x - current.x;
    float dy   = target.y - current.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < step_size_) {
      current.x = target.x;
      current.y = target.y;
      if (fb) {
        fb->current_x         = current.x;
        fb->current_y         = current.y;
        fb->distance_remaining = 0.0F;
        fb->percent_complete   = 100.0F;
      }
      return true;
    }

    // Normalize and scale by step_size
    float ratio = step_size_ / dist;
    current.x += dx * ratio;
    current.y += dy * ratio;

    if (fb) {
      fb->current_x          = current.x;
      fb->current_y          = current.y;
      fb->distance_remaining  = dist - step_size_;
      float total_dist = std::sqrt(target.x * target.x + target.y * target.y);
      fb->percent_complete = total_dist > 0.0F
        ? (1.0F - dist / total_dist) * 100.0F
        : 100.0F;
    }
    return false;
  }

private:
  float step_size_;
};

}  // namespace execution
}  // namespace domain
}  // namespace amr

#endif
