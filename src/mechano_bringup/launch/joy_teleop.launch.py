from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Get package directory
    bringup_pkg = FindPackageShare('mechano_bringup')

    # Joy teleop config
    joy_teleop_config = PathJoinSubstitution([bringup_pkg, 'config', 'joy_teleop.yaml'])

    # Joy node (joystick driver)
    joy_node = Node(
        package='joy',
        executable='joy_node',
        name='joy_node',
        output='screen',
        parameters=[{
            'deadzone': 0.1,
            'autorepeat_rate': 20.0,
        }]
    )

    # Teleop twist joy node
    teleop_joy_node = Node(
        package='teleop_twist_joy',
        executable='teleop_node',
        name='teleop_twist_joy_node',
        output='screen',
        parameters=[joy_teleop_config],
        remappings=[
            ('cmd_vel', 'cmd_vel_teleop')
        ]
    )

    return LaunchDescription([
        joy_node,
        teleop_joy_node,
    ])
