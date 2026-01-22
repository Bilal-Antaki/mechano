# Robot Installation Guide

This guide walks you through setting up the Mechano robot hardware.

## Prerequisites

- Assembled Mechano robot (see [Hardware Assembly](docs/hardware_assembly.md))
- Ubuntu 22.04 or 24.04 installed on robot computer
- Network connection (WiFi or Ethernet)

## Step 1: Install ROS2

```bash
# Add ROS2 repository
sudo apt update && sudo apt install -y software-properties-common
sudo add-apt-repository universe
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# Install ROS2 (Humble for 22.04, Jazzy for 24.04)
sudo apt update
sudo apt install -y ros-humble-desktop  # or ros-jazzy-desktop
```

## Step 2: Clone and Build Workspace

```bash
mkdir -p ~/ros
cd ~/ros
git clone https://github.com/yourusername/mechano_ws.git
cd mechano_ws
./install_mechano.bash
```

## Step 3: Set Up Hardware

### USB Permissions

```bash
./scripts/setup_udev_rules.bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

### Flash Microcontroller Firmware

```bash
./update_firmware.bash
```

### Calibrate Sensors

```bash
# IMU calibration
./scripts/calibrate_imu.bash

# Camera calibration (optional)
./scripts/calibrate_camera.bash
```

## Step 4: Verify Hardware

```bash
./scripts/check_hardware.bash
```

This script will check:
- Microcontroller connection
- IMU data stream
- Camera connectivity
- Motor encoder feedback

## Step 5: Auto-Start on Boot (Optional)

```bash
./scripts/create_systemd_service.bash
sudo systemctl enable mechano.service
```

## Step 6: Test the Robot

```bash
source install/setup.bash
ros2 launch mechano_bringup bringup.launch.py
```

In a new terminal:
```bash
source install/setup.bash
ros2 launch mechano_bringup teleop.launch.py
```

Use the keyboard to drive the robot and verify everything works.

## Troubleshooting

See [docs/troubleshooting.md](docs/troubleshooting.md) for common issues.
