# mechano

Metapackage for the Mechano differential drive robot.

## Description

This metapackage depends on all other Mechano packages, providing a convenient way to install the complete robot software stack.

## Included Packages

- **mechano_msgs**: Custom message and service definitions
- **mechano_description**: URDF/Xacro robot model
- **mechano_hardware**: ros2_control hardware interface
- **mechano_bringup**: Launch files and configurations
- **mechano_gazebo**: Gazebo simulation
- **mechano_navigation**: Nav2 configuration
- **mechano_perception**: Camera and sensor drivers
- **mechano_node**: Main robot node with HMI
- **mechano_viz**: RViz configurations

## Installation

To install all Mechano packages:

```bash
sudo apt install ros-${ROS_DISTRO}-mechano
```

Or build from source:

```bash
cd ~/ros2_ws
colcon build --packages-up-to mechano
```
