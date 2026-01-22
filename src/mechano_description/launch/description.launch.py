import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import Command, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Get package share directory
    pkg_share = FindPackageShare('mechano_description')

    # Declare launch arguments
    use_sim_arg = DeclareLaunchArgument(
        'use_sim',
        default_value='false',
        description='Use simulation (Gazebo) mode'
    )

    use_fake_hardware_arg = DeclareLaunchArgument(
        'use_fake_hardware',
        default_value='false',
        description='Use fake hardware interface'
    )

    use_rviz_arg = DeclareLaunchArgument(
        'use_rviz',
        default_value='false',
        description='Launch RViz'
    )

    use_jsp_gui_arg = DeclareLaunchArgument(
        'use_jsp_gui',
        default_value='false',
        description='Use joint_state_publisher_gui'
    )

    # Get URDF via xacro
    robot_description_content = Command([
        'xacro ',
        PathJoinSubstitution([pkg_share, 'urdf', 'mechano.urdf.xacro']),
        ' use_sim:=', LaunchConfiguration('use_sim'),
        ' use_fake_hardware:=', LaunchConfiguration('use_fake_hardware'),
    ])

    robot_description = {'robot_description': robot_description_content}

    # Robot state publisher
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[robot_description]
    )

    # Joint state publisher (for visualization without hardware)
    joint_state_publisher_node = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        condition=IfCondition(LaunchConfiguration('use_jsp_gui')),
    )

    # Joint state publisher GUI
    joint_state_publisher_gui_node = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        condition=IfCondition(LaunchConfiguration('use_jsp_gui')),
    )

    # RViz
    rviz_config_file = PathJoinSubstitution([pkg_share, 'rviz', 'model.rviz'])
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', rviz_config_file],
        condition=IfCondition(LaunchConfiguration('use_rviz'))
    )

    return LaunchDescription([
        use_sim_arg,
        use_fake_hardware_arg,
        use_rviz_arg,
        use_jsp_gui_arg,
        robot_state_publisher_node,
        joint_state_publisher_node,
        joint_state_publisher_gui_node,
        rviz_node,
    ])
