from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Get package directory
    perception_pkg = FindPackageShare('mechano_perception')

    # Include camera launch
    camera_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([perception_pkg, 'launch', 'stereo_camera.launch.py'])
        ])
    )

    # Include distance sensor launch
    distance_sensor_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([perception_pkg, 'launch', 'distance_sensor.launch.py'])
        ])
    )

    return LaunchDescription([
        camera_launch,
        distance_sensor_launch,
    ])
