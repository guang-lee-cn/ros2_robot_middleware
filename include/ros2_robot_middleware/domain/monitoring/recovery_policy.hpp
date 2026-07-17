#ifndef ROS2_ROBOT_MIDDLEWARE_DOMAIN_RECOVERY_POLICY_HPP_
#define ROS2_ROBOT_MIDDLEWARE_DOMAIN_RECOVERY_POLICY_HPP_

#include "heartbeat_analyzer.hpp"

namespace amr {
namespace domain {
namespace monitoring {

// Watchdog recovery policy — pure domain logic.
class RecoveryPolicy {
public:
  static constexpr int kMaxRetries = 3;

  struct RecoveryState {
    int attempts = 0;
  };

  // Should we attempt recovery? Returns true if node is in ERROR and
  // hasn't exceeded the retry limit.
  bool should_recover(NodeStatus status, const RecoveryState &state) const {
    return status == NodeStatus::ERROR && state.attempts < kMaxRetries;
  }

  // Upgrade ERROR to FATAL after max attempts exhausted.
  NodeStatus escalate(NodeStatus status, const RecoveryState &state) const {
    if (status == NodeStatus::ERROR && state.attempts >= kMaxRetries) {
      return NodeStatus::FATAL;
    }
    return status;
  }

  // Return heartbeat status string for recovery.
  // OK = alive; ERROR/FATAL triggers recovery counter.
  bool reset_on_ok() const { return true; }   // reset counter when node recovers
};

}  // namespace monitoring
}  // namespace domain
}  // namespace amr

#endif
