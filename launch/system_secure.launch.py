"""SROS2 安全启动 — 所有节点加载 DDS-Security 密钥

前置条件（一次性）：
  cd config/sros2
  ros2 security create_keystore .
  for n in /lidar /imu /camera /fusion /decision /motor_ctrl /health_monitor; do
    ros2 security create_enclave . $n
    ros2 security create_permission . $n config/sros2/permissions_${n#/}.xml
  done
  # 手动编辑 governance.p7s（已提供模板）

启动：
  ros2 launch ros2_robot_middleware system_secure.launch.py

验证：
  ros2 topic echo /sensor/lidar    # 未授权节点无法订阅
  抓包：sudo tcpdump -i lo -A | grep "lidar"   # 数据已加密
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import LifecycleNode


def generate_launch_description():
    pkg_dir = get_package_share_directory("ros2_robot_middleware")
    keystore = os.path.join(pkg_dir, "config", "sros2")

    env = {
        "ROS_SECURITY_KEYSTORE": keystore,
        "ROS_SECURITY_ENABLE": "true",
        "ROS_SECURITY_STRATEGY": "Enforce",
    }

    nodes = [
        ("lidar_node",          "/lidar"),
        ("imu_node",            "/imu"),
        ("camera_node",         "/camera"),
        ("fusion_node",         "/fusion"),
        ("decision_node",       "/decision"),
        ("motor_ctrl_node",     "/motor_ctrl"),
        ("health_monitor_node", "/health_monitor"),
    ]

    return LaunchDescription([
        LifecycleNode(
            package="ros2_robot_middleware",
            executable=exec_name,
            name=node_name,
            namespace="",
            additional_env=env,
            output="screen",
        )
        for exec_name, node_name in nodes
    ])
