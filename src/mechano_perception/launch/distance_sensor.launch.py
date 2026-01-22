from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    perception_pkg = FindPackageShare('mechano_perception')

    # Distance sensor config
    distance_config = PathJoinSubstitution([perception_pkg, 'config', 'distance_sensor.yaml'])

    # Declare arguments
    use_sim_time_arg = DeclareLaunchArgument(
        'use_sim_time',
        default_value='false',
        description='Use simulation time'
    )

    # Distance sensor node
    distance_sensor_node = Node(
        package='mechano_perception',
        executable='distance_sensor_node',
        name='distance_sensor_node',
        output='screen',
        parameters=[
            distance_config,
            {'use_sim_time': LaunchConfiguration('use_sim_time')}
        ]
    )

    return LaunchDescription([
        use_sim_time_arg,
        distance_sensor_node,
    ])
