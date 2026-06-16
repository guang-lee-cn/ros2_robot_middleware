# 2D lidar scan — range measurements in polar coordinates
std_msgs/Header header

float32[] ranges             # distance per angular bin (m)
float32[] intensities        # 与 ranges 一一对应, 0=max 反射
float32   time_increment     # 相邻测距点的时间间隔 (s)
float32   angle_min          # start angle (rad)
float32   angle_max          # end angle (rad)
float32   angle_increment    # angular step (rad)
