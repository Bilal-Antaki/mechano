from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    pkg = FindPackageShare('mechano_node')

    # Config file
    config = PathJoinSubstitution([pkg, 'config', 'mechano_node.yaml'])

    # Declare arguments
    use_sim_time_arg = DeclareLaunchArgument(
        'use_sim_time',
        default_value='false',
        description='Use simulation time'
    )

    # Mechano node
    mechano_node = Node(
        package='mechano_node',
        executable='mechano_node',
        name='mechano_node',
        output='screen',
        parameters=[
            config,
            {'use_sim_time': LaunchConfiguration('use_sim_time')}
        ]
    )

    return LaunchDescription([
        use_sim_time_arg,
        mechano_node,
    ])
