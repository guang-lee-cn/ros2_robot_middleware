#ifndef ROS2_ROBOT_MIDDLEWARE_KALMAN_FILTER_HPP_
#define ROS2_ROBOT_MIDDLEWARE_KALMAN_FILTER_HPP_

#include <cmath>

// 2D 线性卡尔曼滤波器 — 恒加速度模型
//
// 状态向量 (4D): [x, y, vx, vy]
//   位置 + 速度，位置由 LiDAR 观测，速度由 IMU 加速度驱动
//
// 面试要点：
// - 为什么是 KF 而不是 EKF？2D 恒加速度模型的状态转移是线性的（速度×时间）
//   不需要 EKF。只有涉及角度（cos/sin）的模型才需要 EKF/UKF。
// - 预测步：x = F·x + B·u (状态转移 + 控制输入，u 来自 IMU)
// - 更新步：K = P·H^T / (H·P·H^T + R), x = x + K(z - H·x)

class KalmanFilter2D {
public:
  // 4 维状态: [x, y, vx, vy]
  static constexpr int kDim  = 4;
  // 2 维测量: [x, y] (来自 LiDAR 聚类中心)
  static constexpr int kMeas = 2;

  KalmanFilter2D() { reset(); }

  void reset() {
    for (double &v : state_) v = 0.0;
    for (double &v : cov_) v = 0.0;
    for (int i = 0; i < kDim; ++i) cov_[i * kDim + i] = 1.0;
  }

  // 预测步: dt 为秒, ax/ay 为 IMU 加速度 (m/s²)
  void predict(double dt, double ax, double ay) {
    // 状态转移矩阵 F: [1, 0, dt,  0 ]
    //                  [0, 1,  0, dt ]
    //                  [0, 0,  1,  0 ]
    //                  [0, 0,  0,  1 ]
    const double f[16] = {
      1.0, 0.0, dt,  0.0,
      0.0, 1.0, 0.0, dt,
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0,
    };

    // 控制矩阵 B: [0.5*dt², 0      ]
    //             [0      , 0.5*dt²]
    //             [dt     , 0      ]
    //             [0      , dt     ]
    // x = F·x + B·u
    double x_new[4] = {};
    for (int i = 0; i < kDim; ++i) {
      for (int j = 0; j < kDim; ++j) { x_new[i] += f[i * kDim + j] * state_[j]; }
      x_new[i] += (i == 0 || i == 1 ? 0.5 * dt * dt : dt) * (i < 2 ? (i == 0 ? ax : ay) : 0.0);
    }
    x_new[2] += dt * ax;
    x_new[3] += dt * ay;

    // P = F·P·F^T + Q
    // Step 1: FP = F * P
    double fp[16] = {};
    for (int i = 0; i < kDim; ++i) {
      for (int j = 0; j < kDim; ++j) {
        for (int k = 0; k < kDim; ++k) { fp[i * kDim + j] += f[i * kDim + k] * cov_[k * kDim + j]; }
      }
    }
    // Step 2: P_new = FP * F^T
    double p_new[16] = {};
    for (int i = 0; i < kDim; ++i) {
      for (int j = 0; j < kDim; ++j) {
        for (int k = 0; k < kDim; ++k) { p_new[i * kDim + j] += fp[i * kDim + k] * f[j * kDim + k]; }
      }
    }

    // Step 3: + Q
    for (int i = 0; i < kDim * kDim; ++i) cov_[i] = p_new[i];
    for (int i = 0; i < kDim; ++i) cov_[i * kDim + i] += kQdiag[i];

    for (int i = 0; i < kDim; ++i) state_[i] = x_new[i];
  }

  // 更新步: zx/zy 为 LiDAR 聚类中心的 (x,y) 坐标
  // R 为测量噪声协方差（越大 = 越不信任测量）
  void update(double zx, double zy, double measurement_noise = 0.1) {
    // 测量矩阵 H: [1, 0, 0, 0]
    //             [0, 1, 0, 0]
    // 卡尔曼增益 K = P·H^T / (H·P·H^T + R)
    // 简化: 二维独立测量，分开算

    for (int meas = 0; meas < kMeas; ++meas) {
      const int idx = meas;  // measurement maps to state[meas]

      // S = P[idx][idx] + R
      double S = cov_[idx * kDim + idx] + measurement_noise;

      // K = P[:,idx] / S (只取一列)
      double K[4] = {};
      for (int i = 0; i < kDim; ++i) { K[i] = cov_[i * kDim + idx] / S; }

      // 残差 y = z - H·x
      double y = (meas == 0 ? zx : zy) - state_[idx];

      // x = x + K·y
      for (int i = 0; i < kDim; ++i) { state_[i] += K[i] * y; }

      // P = (I - K·H)·P
      for (int i = 0; i < kDim; ++i) {
        for (int j = 0; j < kDim; ++j) {
          cov_[i * kDim + j] -= K[i] * cov_[idx * kDim + j];
        }
      }
    }
  }

  double x()  const { return state_[0]; }
  double y()  const { return state_[1]; }
  double vx() const { return state_[2]; }
  double vy() const { return state_[3]; }

private:
  double state_[kDim] = {};
  double cov_[kDim * kDim] = {};

  // 过程噪声 Q 对角元素（越小 = 越信任模型预测）
  // x,y 噪声 0.01, vx,vy 噪声 0.1
  static constexpr double kQdiag[4] = {0.01, 0.01, 0.1, 0.1};
};

#endif
