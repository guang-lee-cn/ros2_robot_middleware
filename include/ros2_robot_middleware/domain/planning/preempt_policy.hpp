#ifndef ROS2_ROBOT_MIDDLEWARE_DOMAIN_PREEMPT_POLICY_HPP_
#define ROS2_ROBOT_MIDDLEWARE_DOMAIN_PREEMPT_POLICY_HPP_

namespace amr {
namespace domain {
namespace planning {

// PreemptPolicy: decides when to cancel an in-flight goal.
// Pure domain logic.
class PreemptPolicy {
public:
  // Always preempt — new perception data always replaces old goal.
  // Production variants: distance-based (only preempt if >0.5m deviation),
  // time-based (don't preempt within 2s of goal completion), etc.
  bool should_preempt(bool has_active_goal, float /* new_x */, float /* new_y */) const {
    return has_active_goal;
  }
};

}  // namespace planning
}  // namespace domain
}  // namespace amr

#endif
