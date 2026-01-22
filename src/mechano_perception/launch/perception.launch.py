from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
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

    # Include depth to scan launch
    depth_to_scan_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([perception_pkg, 'launch', 'depth_to_scan.launch.py'])
        ])
    )

    return LaunchDescription([
        camera_launch,
        distance_sensor_launch,
        depth_to_scan_launch,
    ])
