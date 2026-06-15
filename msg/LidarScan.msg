# 2D lidar scan — range measurements in polar coordinates
std_msgs/Header header

float32[] ranges             # distance per angular bin (m)
float32   angle_min          # start angle (rad)
float32   angle_max          # end angle (rad)
float32   angle_increment    # angular step (rad)
