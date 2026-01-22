import os
from launch import LaunchDescription
from launch.actions import RegisterEventHandler, TimerAction
from launch.event_handlers import OnProcessStart
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Get package directories
    bringup_pkg = FindPackageShare('mechano_bringup')
    description_pkg = FindPackageShare('mechano_description')

    # Controller configuration
    controller_config = PathJoinSubstitution([bringup_pkg, 'config', 'mechano_controllers.yaml'])

    # Robot description
    robot_description = Command([
        'xacro ',
        PathJoinSubstitution([description_pkg, 'urdf', 'mechano.urdf.xacro']),
        ' use_sim:=false use_fake_hardware:=false'
    ])

    # Controller manager node
    controller_manager = Node(
        package='controller_manager',
        executable='ros2_control_node',
        parameters=[
            {'robot_description': robot_description},
            controller_config
        ],
        output='screen',
    )

    # Spawn diff_drive_controller
    diff_drive_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['diff_drive_controller', '--controller-manager', '/controller_manager'],
        output='screen',
    )

    # Spawn joint_state_broadcaster
    joint_state_broadcaster_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster', '--controller-manager', '/controller_manager'],
        output='screen',
    )

    # Delay controller spawning until controller_manager is ready
    delayed_diff_drive_spawner = RegisterEventHandler(
        event_handler=OnProcessStart(
            target_action=controller_manager,
            on_start=[
                TimerAction(
                    period=3.0,
                    actions=[diff_drive_spawner],
                ),
            ],
        )
    )

    delayed_joint_state_broadcaster_spawner = RegisterEventHandler(
        event_handler=OnProcessStart(
            target_action=controller_manager,
            on_start=[
                TimerAction(
                    period=3.0,
                    actions=[joint_state_broadcaster_spawner],
                ),
            ],
        )
    )

    return LaunchDescription([
        controller_manager,
        delayed_diff_drive_spawner,
        delayed_joint_state_broadcaster_spawner,
    ])
