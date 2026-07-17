#ifndef ROS2_ROBOT_MIDDLEWARE_DOMAIN_CLUSTER_DETECTOR_HPP_
#define ROS2_ROBOT_MIDDLEWARE_DOMAIN_CLUSTER_DETECTOR_HPP_

#include <vector>
#include <cmath>
#include <string>

namespace amr {
namespace domain {
namespace perception {

// 1D scan-line clustering result
struct Cluster {
  float x = 0.0F;
  float y = 0.0F;
  float z = 0.0F;
  std::string id;
};

// Extract objects from LaserScan ranges using 1D scan-line clustering.
// Pure domain logic — zero ROS2/DDS dependency, fully unit-testable.
//
// Algorithm: scans 360 points from angle_min to angle_max.
//   hit:  range ∈ (min_range, max_range)
//   gap:   contiguous hits form a cluster if width > min_width
//   center: midpoint polar→cartesian conversion
class ClusterDetector {
public:
  struct Params {
    int   num_points   = 360;
    int   max_objects  = 5;
    float max_range    = 3.0F;
    float min_range    = 0.1F;
    int   min_cluster_width = 5;
  };

  ClusterDetector() {}
  explicit ClusterDetector(const Params &p) : params_(p) {}

  void set_params(const Params &p) { params_ = p; }

  // Core algorithm — takes range array + angle metadata, returns clusters.
  // Pure function: no side effects, no ROS2.
  std::vector<Cluster> detect(
    const float ranges[],
    float angle_min,
    float angle_increment) const
  {
    std::vector<Cluster> clusters;
    clusters.reserve(params_.max_objects);

    int start = -1;
    for (int i = 0;
         i < params_.num_points &&
         static_cast<int>(clusters.size()) < params_.max_objects;
         ++i) {
      bool hit = (ranges[i] > params_.min_range &&
                  ranges[i] < params_.max_range);

      if (hit && start < 0) {
        start = i;
      } else if (!hit && start >= 0) {
        if (i - start > params_.min_cluster_width) {
          int mid     = (start + i) / 2;
          float angle = angle_min + mid * angle_increment;
          float range = ranges[mid];

          Cluster c;
          c.id = "obj_" + std::to_string(clusters.size());
          c.x  = range * std::cos(angle);
          c.y  = range * std::sin(angle);
          c.z  = 0.0F;
          clusters.push_back(c);
          start = -1;
        } else {
          start = -1;
        }
      }
    }
    return clusters;
  }

private:
  Params params_;
};

}  // namespace perception
}  // namespace domain
}  // namespace amr

#endif
