from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='uuv_imu_controller',
            executable='imu_controller',
            name='imu_controller',
            output='screen',
        ),
    ])
