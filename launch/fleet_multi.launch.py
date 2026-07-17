"""多 AMR 集群启动 — 1 组完整 AMR 管线 + Fleet Manager

生产环境多 AMR 部署方式：
  Docker Compose 启动多个容器实例，每台 AMR 独立进程 + 独立 ROS_DOMAIN_ID。
  Fleet Manager 通过 Fast-DDS Partitions 或 Domain Bridge 跨域通信。
  本 launch 演示单进程内 Fleet Manager 的集成架构。

面试话术：
  "多 AMR 集群的核心是每台机器人独立运行自己的感知-决策-执行管线，
   通过 DDS domain 做网络级隔离。Fleet Manager 是跨 domain 的
   控制面节点——它订阅所有 AMR 的健康报告做全局集群状态决策。"
"""

from launch import LaunchDescription
from launch_ros.actions import LifecycleNode

BUSINESS_NODES = [
    ("lidar_node",          "lidar"),
    ("imu_node",            "imu"),
    ("camera_node",         "camera"),
    ("fusion_node",         "fusion"),
    ("decision_node",       "decision"),
    ("motor_ctrl_node",     "motor_ctrl"),
]


def generate_launch_description():
    desc = []

    # ── AMR-1 业务管线（全局 topic，通过 remapping 命名空间化） ────────
    for exec_name, node_name in BUSINESS_NODES:
        desc.append(
            LifecycleNode(
                package="ros2_robot_middleware",
                executable=exec_name,
                name=f"amr1_{node_name}",
                namespace="",
                output="screen",
            )
        )

    # ── AMR-1 health_monitor（remap 输出到 /amr1/health/report）─────────
    desc.append(
        LifecycleNode(
            package="ros2_robot_middleware",
            executable="health_monitor_node",
            name="amr1_health_monitor",
            namespace="",
            remappings=[
                ("/health/report", "/amr1/health/report"),
                ("/health/check",  "/amr1/health/check"),
            ],
            output="screen",
        )
    )

    # ── Fleet Manager ───────────────────────────────────────────────────
    desc.append(
        LifecycleNode(
            package="ros2_robot_middleware",
            executable="fleet_manager_node",
            name="fleet_manager",
            namespace="",
            output="screen",
        )
    )

    return LaunchDescription(desc)
