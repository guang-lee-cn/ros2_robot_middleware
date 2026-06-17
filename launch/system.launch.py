"""Launch all 6 nodes of the perception-to-actuation pipeline."""

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        # ── Sensor Layer ──
        Node(
            package='ros2_robot_middleware',
            executable='lidar_node',
            name='lidar',
            output='screen',
        ),
        Node(
            package='ros2_robot_middleware',
            executable='imu_node',
            name='imu',
            output='screen',
        ),
        Node(
            package='ros2_robot_middleware',
            executable='camera_node',
            name='camera',
            output='screen',
        ),
        # ── Fusion Layer ──
        Node(
            package='ros2_robot_middleware',
            executable='fusion_node',
            name='fusion',
            output='screen',
        ),
        # ── Decision Layer ──
        Node(
            package='ros2_robot_middleware',
            executable='decision_node',
            name='decision',
            output='screen',
        ),
        # ── Actuation Layer ──
        Node(
            package='ros2_robot_middleware',
            executable='motor_ctrl_node',
            name='motor_ctrl',
            output='screen',
        ),
    ])
