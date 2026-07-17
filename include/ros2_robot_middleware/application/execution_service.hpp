#ifndef ROS2_ROBOT_MIDDLEWARE_APPLICATION_EXECUTION_SERVICE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_APPLICATION_EXECUTION_SERVICE_HPP_

#include "ros2_robot_middleware/domain/execution/interpolator.hpp"

namespace amr {
namespace application {

// ExecutionService — thin wrapper around motion interpolator.
// Pure C++, zero ROS2 dependency.
class ExecutionService {
public:
  using State    = amr::domain::execution::Interpolator::State;
  using Feedback = amr::domain::execution::Interpolator::Feedback;

  explicit ExecutionService(float step_size = 0.05F)
    : interpolator_(step_size) {}

  void set_step_size(float s) { interpolator_.set_step_size(s); }

  bool step(const State &target, State &current, Feedback *fb = nullptr) {
    return interpolator_.step(target, current, fb);
  }

private:
  amr::domain::execution::Interpolator interpolator_;
};

}  // namespace application
}  // namespace amr

#endif
