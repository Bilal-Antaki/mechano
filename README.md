# Mechano Robot

A ROS2 differential drive robot platform for autonomous navigation and research.

## Features

- Differential drive mobile base with encoder feedback
- Stereo camera for depth perception
- IMU for orientation estimation
- Distance sensors for obstacle detection
- Full Nav2 integration for autonomous navigation
- SLAM capability with slam_toolbox
- Gazebo simulation support
- ros2_control hardware interface

## Quick Start

### Prerequisites

- Ubuntu 22.04 or 24.04
- ROS2 Humble or Jazzy
- colcon build tools

### Installation

**On the robot computer:**
```bash
git clone https://github.com/yourusername/mechano_ws.git
cd mechano_ws
./install_mechano.bash
```

**On development PC (simulation only):**
```bash
git clone https://github.com/yourusername/mechano_ws.git
cd mechano_ws
./install_mechano_desktop.bash
```

### Building

```bash
cd mechano_ws
colcon build --symlink-install
source install/setup.bash
```

### Running

**Real robot:**
```bash
ros2 launch mechano_bringup bringup.launch.py
```

**Simulation:**
```bash
ros2 launch mechano_gazebo gazebo.launch.py
```

**Navigation:**
```bash
ros2 launch mechano_navigation navigation.launch.py map:=/path/to/map.yaml
```

**SLAM:**
```bash
ros2 launch mechano_navigation slam.launch.py
```

## Package Overview

| Package | Description |
|---------|-------------|
| `mechano` | Metapackage with dependencies on all other packages |
| `mechano_msgs` | Custom message and service definitions |
| `mechano_description` | URDF/Xacro robot model |
| `mechano_hardware` | ros2_control hardware interface |
| `mechano_bringup` | Launch files for real robot |
| `mechano_gazebo` | Gazebo simulation |
| `mechano_navigation` | Nav2 configuration and launch files |
| `mechano_perception` | Camera and sensor drivers |
| `mechano_node` | Main robot node with HMI and behaviors |
| `mechano_viz` | RViz configurations |

## Documentation

- [Hardware BOM](docs/hardware_bom.md)
- [Hardware Assembly](docs/hardware_assembly.md)
- [Software Architecture](docs/software_architecture.md)
- [ros2_control Overview](docs/ros2_control_overview.md)
- [Navigation Tuning](docs/navigation_tuning.md)
- [Troubleshooting](docs/troubleshooting.md)

## License

This project is licensed under the Apache 2.0 License - see the [LICENSE](LICENSE) file for details.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for contribution guidelines.
