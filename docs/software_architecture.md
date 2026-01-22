# Software Architecture

This document describes the software architecture of the Mechano robot.

## Overview

The Mechano robot software is built on ROS2 and follows standard ROS2 patterns and conventions.

## Package Structure

```
mechano_ws/src/
├── mechano/              # Metapackage
├── mechano_msgs/         # Custom messages and services
├── mechano_description/  # URDF robot model
├── mechano_hardware/     # ros2_control hardware interface
├── mechano_bringup/      # Launch files and configs
├── mechano_gazebo/       # Simulation
├── mechano_navigation/   # Nav2 configuration
├── mechano_perception/   # Camera and sensor drivers
├── mechano_node/         # Main robot node
└── mechano_viz/          # RViz configurations
```

## Node Graph

```
                                    ┌─────────────────┐
                                    │   Navigation    │
                                    │   (Nav2 Stack)  │
                                    └────────┬────────┘
                                             │
                                             │ /cmd_vel
                                             ▼
┌──────────────┐    /cmd_vel     ┌───────────────────┐
│  Teleop      │ ───────────────▶│    twist_mux      │
│  (joy/kbd)   │                 └─────────┬─────────┘
└──────────────┘                           │
                                           │ /cmd_vel_out
                                           ▼
                              ┌────────────────────────┐
                              │   Controller Manager   │
                              │  (diff_drive_controller)│
                              └────────────┬───────────┘
                                           │
                                           ▼
                              ┌────────────────────────┐
                              │   Hardware Interface   │
                              │  (mechano_hardware)    │
                              └────────────┬───────────┘
                                           │
                                           │ Serial
                                           ▼
                              ┌────────────────────────┐
                              │     Microcontroller    │
                              │      (Arduino)         │
                              └────────────────────────┘
```

## Topics

### Velocity Commands
| Topic | Type | Description |
|-------|------|-------------|
| `/cmd_vel` | `geometry_msgs/Twist` | Input to twist_mux |
| `/cmd_vel_out` | `geometry_msgs/Twist` | Output from twist_mux to controller |

### Odometry
| Topic | Type | Description |
|-------|------|-------------|
| `/odom` | `nav_msgs/Odometry` | Wheel odometry |
| `/odometry/filtered` | `nav_msgs/Odometry` | EKF-fused odometry |

### Sensors
| Topic | Type | Description |
|-------|------|-------------|
| `/imu/data` | `sensor_msgs/Imu` | IMU measurements |
| `/camera/depth/image_raw` | `sensor_msgs/Image` | Depth image |
| `/camera/color/image_raw` | `sensor_msgs/Image` | Color image |
| `/scan` | `sensor_msgs/LaserScan` | Laser scan (from depth) |

### Robot State
| Topic | Type | Description |
|-------|------|-------------|
| `/robot_state` | `mechano_msgs/RobotState` | Battery, mode, errors |
| `/joint_states` | `sensor_msgs/JointState` | Wheel positions/velocities |

## TF Tree

```
map
 └── odom
      └── base_footprint
           └── base_link
                ├── left_wheel
                ├── right_wheel
                ├── caster_link
                ├── imu_link
                ├── camera_link
                │    ├── camera_depth_frame
                │    └── camera_color_frame
                └── distance_sensor_link
```

## Services

| Service | Type | Description |
|---------|------|-------------|
| `/set_mode` | `mechano_msgs/SetMode` | Change robot mode |
| `/estop` | `mechano_msgs/EStop` | Activate/deactivate E-stop |

## Parameters

Key parameters are defined in:
- `mechano_bringup/config/mechano_controllers.yaml` - Controller parameters
- `mechano_bringup/config/ekf.yaml` - Sensor fusion parameters
- `mechano_navigation/config/nav2_params.yaml` - Navigation parameters

## Data Flow

### Odometry Flow
1. Motor encoders → Microcontroller
2. Microcontroller → Hardware Interface (serial)
3. Hardware Interface → Controller Manager
4. Controller Manager → `/odom` topic
5. EKF node fuses `/odom` + `/imu/data`
6. EKF publishes `/odometry/filtered` and `odom→base_footprint` TF

### Navigation Flow
1. Goal received on `/goal_pose`
2. Nav2 planner computes global path
3. Nav2 controller computes local commands
4. Commands sent to `/cmd_vel`
5. twist_mux forwards to controller
6. Controller converts to wheel commands
7. Hardware interface sends to motors

## See Also

- [ros2_control Overview](ros2_control_overview.md)
- [Navigation Tuning](navigation_tuning.md)
