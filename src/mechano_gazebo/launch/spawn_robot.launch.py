from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import Command, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Get package directory
    description_pkg = FindPackageShare('mechano_description')

    # Declare launch arguments
    x_arg = DeclareLaunchArgument('x', default_value='0.0', description='X position')
    y_arg = DeclareLaunchArgument('y', default_value='0.0', description='Y position')
    z_arg = DeclareLaunchArgument('z', default_value='0.1', description='Z position')
    yaw_arg = DeclareLaunchArgument('yaw', default_value='0.0', description='Yaw angle')

    # Robot description
    robot_description = Command([
        'xacro ',
        PathJoinSubstitution([description_pkg, 'urdf', 'mechano.urdf.xacro']),
        ' use_sim:=true use_fake_hardware:=false'
    ])

    # Robot state publisher
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{
            'robot_description': robot_description,
            'use_sim_time': True
        }]
    )

    # Spawn robot
    spawn_robot = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=[
            '-topic', 'robot_description',
            '-entity', 'mechano',
            '-x', LaunchConfiguration('x'),
            '-y', LaunchConfiguration('y'),
            '-z', LaunchConfiguration('z'),
            '-Y', LaunchConfiguration('yaw')
        ],
        output='screen'
    )

    return LaunchDescription([
        x_arg,
        y_arg,
        z_arg,
        yaw_arg,
        robot_state_publisher,
        spawn_robot,
    ])
