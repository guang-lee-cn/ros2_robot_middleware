#ifndef ROS2_ROBOT_MIDDLEWARE_APPLICATION_MONITORING_SERVICE_HPP_
#define ROS2_ROBOT_MIDDLEWARE_APPLICATION_MONITORING_SERVICE_HPP_

#include "ros2_robot_middleware/domain/monitoring/heartbeat_analyzer.hpp"
#include "ros2_robot_middleware/domain/monitoring/recovery_policy.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace amr {
namespace application {

// MonitoringService — orchestrates heartbeat analysis + recovery policy.
// Pure C++, zero ROS2 dependency.
class MonitoringService {
public:
  using NodeStatus    = amr::domain::monitoring::NodeStatus;
  using NodeHeartbeat = amr::domain::monitoring::NodeHeartbeat;
  using Analyzer      = amr::domain::monitoring::HeartbeatAnalyzer;
  using Recovery      = amr::domain::monitoring::RecoveryPolicy;
  using FleetSummary  = amr::domain::monitoring::FleetSummary;

  // Register a node to monitor.
  void register_node(const std::string &name, double timeout_s) {
    NodeHeartbeat hb;
    hb.node_name = name;
    hb.timeout_s  = timeout_s;
    hb.last_seen_s = -1.0;
    heartbeats_[name] = hb;
  }

  // Update heartbeat timestamp for a node.
  void heartbeat_received(const std::string &name, double age_s = 0.0) {
    auto it = heartbeats_.find(name);
    if (it != heartbeats_.end()) it->second.last_seen_s = age_s;
  }

  // Age all heartbeats by dt seconds.
  void tick(double dt) {
    for (auto &[name, hb] : heartbeats_) {
      if (hb.last_seen_s >= 0) hb.last_seen_s += dt;
    }
  }

  // Evaluate status for a single node.
  NodeStatus evaluate(const std::string &name) const {
    auto it = heartbeats_.find(name);
    if (it == heartbeats_.end()) return NodeStatus::STALE;
    return analyzer_.evaluate(it->second);
  }

  // Recovery decision for a node.
  bool should_recover(const std::string &name) {
    auto status = evaluate(name);
    auto &rec = recovery_[name];
    bool ok = recovery_policy_.should_recover(status, rec);
    if (ok) rec.attempts++;
    return ok;
  }

  // Reset recovery counter when node recovers.
  void on_recovered(const std::string &name) {
    recovery_[name].attempts = 0;
  }

  NodeStatus escalated_status(const std::string &name) {
    auto status = evaluate(name);
    return recovery_policy_.escalate(status, recovery_[name]);
  }

  // Get all heartbeat snapshots.
  std::vector<NodeHeartbeat> snapshot() const {
    std::vector<NodeHeartbeat> result;
    result.reserve(heartbeats_.size());
    for (const auto &[_, hb] : heartbeats_) result.push_back(hb);
    return result;
  }

  Analyzer::Summary summary() const {
    return analyzer_.summarize(snapshot());
  }

  const auto &heartbeats() const { return heartbeats_; }

  // Fleet-level aggregation.
  static FleetSummary fleet_summary(
    const std::vector<Analyzer::Summary> &per_amr) {
    return amr::domain::monitoring::fleet_summarize(per_amr);
  }

private:
  Analyzer analyzer_;
  Recovery recovery_policy_;
  std::unordered_map<std::string, NodeHeartbeat> heartbeats_;
  std::unordered_map<std::string, Recovery::RecoveryState> recovery_;
};

}  // namespace application
}  // namespace amr

#endif
