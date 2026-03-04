import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition, UnlessCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Get package directories
    bringup_pkg = FindPackageShare('mechano_bringup')
    description_pkg = FindPackageShare('mechano_description')

    # Declare launch arguments
    use_sim_arg = DeclareLaunchArgument(
        'use_sim',
        default_value='false',
        description='Use simulation mode'
    )

    use_fake_hardware_arg = DeclareLaunchArgument(
        'use_fake_hardware',
        default_value='false',
        description='Use fake hardware interface'
    )

    # Include robot description
    description_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([description_pkg, 'launch', 'description.launch.py'])
        ]),
        launch_arguments={
            'use_sim': LaunchConfiguration('use_sim'),
            'use_fake_hardware': LaunchConfiguration('use_fake_hardware'),
        }.items()
    )

    # Include controllers
    controllers_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([bringup_pkg, 'launch', 'controllers.launch.py'])
        ]),
        condition=UnlessCondition(LaunchConfiguration('use_sim'))
    )

    use_sim_time = LaunchConfiguration('use_sim')

    # EKF for sensor fusion
    ekf_config = PathJoinSubstitution([bringup_pkg, 'config', 'ekf.yaml'])
    ekf_node = Node(
        package='robot_localization',
        executable='ekf_node',
        name='ekf_filter_node',
        output='screen',
        parameters=[ekf_config, {'use_sim_time': use_sim_time}],
        remappings=[
            ('odometry/filtered', 'odom')
        ]
    )

    # Twist mux for velocity command priority
    twist_mux_config = PathJoinSubstitution([bringup_pkg, 'config', 'twist_mux.yaml'])
    twist_mux_node = Node(
        package='twist_mux',
        executable='twist_mux',
        name='twist_mux',
        output='screen',
        parameters=[twist_mux_config, {'use_sim_time': use_sim_time}],
        remappings=[
            ('cmd_vel_out', 'diff_drive_controller/cmd_vel_unstamped')
        ]
    )

    return LaunchDescription([
        use_sim_arg,
        use_fake_hardware_arg,
        description_launch,
        controllers_launch,
        ekf_node,
        twist_mux_node,
    ])
