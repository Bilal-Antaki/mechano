import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, SetEnvironmentVariable
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, LaunchConfiguration, PathJoinSubstitution, PythonExpression
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Get package directories
    gazebo_pkg = FindPackageShare('mechano_gazebo')
    description_pkg = FindPackageShare('mechano_description')
    bringup_pkg = FindPackageShare('mechano_bringup')
    gazebo_ros_pkg = FindPackageShare('gazebo_ros')

    # Declare launch arguments
    world_arg = DeclareLaunchArgument(
        'world',
        default_value='empty',
        description='World to load (empty, house, warehouse)'
    )

    use_rviz_arg = DeclareLaunchArgument(
        'use_rviz',
        default_value='true',
        description='Launch RViz'
    )

    # Set Gazebo model path
    gazebo_models_path = PathJoinSubstitution([gazebo_pkg, 'models'])
    set_gazebo_model_path = SetEnvironmentVariable(
        'GAZEBO_MODEL_PATH',
        gazebo_models_path
    )

    # World file path
    world_file = PathJoinSubstitution([
        gazebo_pkg, 'worlds',
        PythonExpression(["'", LaunchConfiguration('world'), ".world'"])
    ])

    # Start Gazebo
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([gazebo_ros_pkg, 'launch', 'gazebo.launch.py'])
        ]),
        launch_arguments={
            'world': world_file,
            'verbose': 'true',
        }.items()
    )

    # Robot description with simulation mode
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

    # Spawn robot in Gazebo
    spawn_robot = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=[
            '-topic', 'robot_description',
            '-entity', 'mechano',
            '-x', '0',
            '-y', '0',
            '-z', '0.1'
        ],
        output='screen'
    )

    # Controller configuration
    controller_config = PathJoinSubstitution([bringup_pkg, 'config', 'mechano_controllers.yaml'])

    # Spawn diff_drive_controller
    diff_drive_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['diff_drive_controller', '--controller-manager', '/controller_manager'],
        output='screen',
        parameters=[{'use_sim_time': True}]
    )

    # Spawn joint_state_broadcaster
    joint_state_broadcaster_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster', '--controller-manager', '/controller_manager'],
        output='screen',
        parameters=[{'use_sim_time': True}]
    )

    # RViz
    rviz_config = PathJoinSubstitution([
        FindPackageShare('mechano_viz'), 'rviz', 'full.rviz'
    ])
    rviz = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', rviz_config],
        output='screen',
        parameters=[{'use_sim_time': True}],
        condition=LaunchConfiguration('use_rviz')
    )

    return LaunchDescription([
        world_arg,
        use_rviz_arg,
        set_gazebo_model_path,
        gazebo,
        robot_state_publisher,
        spawn_robot,
        diff_drive_spawner,
        joint_state_broadcaster_spawner,
    ])
