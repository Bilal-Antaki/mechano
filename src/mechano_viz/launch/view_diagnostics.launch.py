from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    # RQT Robot Monitor
    rqt_robot_monitor = Node(
        package='rqt_robot_monitor',
        executable='rqt_robot_monitor',
        name='rqt_robot_monitor',
        output='screen'
    )

    return LaunchDescription([
        rqt_robot_monitor,
    ])
