"""Launch all 7 nodes of the perception-to-actuation pipeline with lifecycle management.

Each node is a rclcpp_lifecycle::LifecycleNode that self-transitions through
configure() → activate() in its main() entrypoint. For external lifecycle
control (e.g. managed by a lifecycle manager), comment out the self-activation
in main() and use:

    ros2 lifecycle set /lidar configure
    ros2 lifecycle set /lidar activate
"""

from launch import LaunchDescription
from launch_ros.actions import LifecycleNode


def generate_launch_description():
    return LaunchDescription([
        # ── Sensor Layer ──
        LifecycleNode(
            package='ros2_robot_middleware',
            executable='lidar_node',
            name='lidar',
            namespace='',
            output='screen',
        ),
        LifecycleNode(
            package='ros2_robot_middleware',
            executable='imu_node',
            name='imu',
            namespace='',
            output='screen',
        ),
        LifecycleNode(
            package='ros2_robot_middleware',
            executable='camera_node',
            name='camera',
            namespace='',
            output='screen',
        ),
        # ── Fusion Layer ──
        LifecycleNode(
            package='ros2_robot_middleware',
            executable='fusion_node',
            name='fusion',
            namespace='',
            output='screen',
        ),
        # ── Decision Layer ──
        LifecycleNode(
            package='ros2_robot_middleware',
            executable='decision_node',
            name='decision',
            namespace='',
            output='screen',
        ),
        # ── Actuation Layer ──
        LifecycleNode(
            package='ros2_robot_middleware',
            executable='motor_ctrl_node',
            name='motor_ctrl',
            namespace='',
            output='screen',
        ),
        # ── Infrastructure Layer ──
        LifecycleNode(
            package='ros2_robot_middleware',
            executable='health_monitor_node',
            name='health_monitor',
            namespace='',
            output='screen',
        ),
    ])
