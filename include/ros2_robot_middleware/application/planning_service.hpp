#ifndef ROS2_ROBOT_MIDDLEWARE_APPLICATION_PLANNING_SERVICE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_APPLICATION_PLANNING_SERVICE_HPP_

#include "ros2_robot_middleware/domain/planning/preempt_policy.hpp"
#include "ros2_robot_middleware/domain/planning/target_selector.hpp"

namespace amr {
namespace application {

// PlanningService — orchestrates target selection and preemption.
// Pure C++, zero ROS2 dependency.
class PlanningService {
public:
  using Goal     = amr::domain::planning::Goal;
  using Object   = amr::domain::planning::PerceivedObject;
  using Selector = amr::domain::planning::TargetSelector;
  using Preempt  = amr::domain::planning::PreemptPolicy;

  bool select_goal(const Object *objects, std::size_t count, Goal &out) {
    return selector_.select(objects, count, out);
  }

  bool should_preempt(bool has_active) const {
    return preempt_.should_preempt(has_active, 0, 0);
  }

  bool should_retry(int attempts) const { return selector_.should_retry(attempts); }

private:
  Selector selector_;
  Preempt  preempt_;
};

}  // namespace application
}  // namespace amr

#endif
