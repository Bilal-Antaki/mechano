from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    perception_pkg = FindPackageShare('mechano_perception')

    # Camera config
    camera_config = PathJoinSubstitution([perception_pkg, 'config', 'stereo_camera.yaml'])

    # Declare arguments
    use_sim_time_arg = DeclareLaunchArgument(
        'use_sim_time',
        default_value='false',
        description='Use simulation time'
    )

    # Note: This is a placeholder. Replace with your actual camera driver.
    # For OAK-D cameras, use depthai_ros_driver
    # For RealSense, use realsense2_camera
    # For Orbbec, use orbbec_camera

    # Placeholder camera node (replace with actual driver)
    # camera_node = Node(
    #     package='depthai_ros_driver',
    #     executable='camera_node',
    #     name='camera',
    #     output='screen',
    #     parameters=[
    #         camera_config,
    #         {'use_sim_time': LaunchConfiguration('use_sim_time')}
    #     ]
    # )

    return LaunchDescription([
        use_sim_time_arg,
        # camera_node,  # Uncomment and configure for your camera
    ])
