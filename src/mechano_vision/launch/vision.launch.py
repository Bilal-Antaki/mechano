from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='mechano_vision',
            executable='camera_processor',
            name='camera_processor',
            output='screen',
            remappings=[
                ('/camera/image_raw', '/camera/image_raw'),
            ],
        ),
    ])
