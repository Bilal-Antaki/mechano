#!/bin/bash
# Installation script for Mechano robot (development PC - simulation only)

set -e

echo "=========================================="
echo "Mechano Desktop Installation Script"
echo "=========================================="

# Check Ubuntu version
UBUNTU_VERSION=$(lsb_release -rs)
echo "Detected Ubuntu $UBUNTU_VERSION"

# Determine ROS2 distribution
if [[ "$UBUNTU_VERSION" == "22.04" ]]; then
    ROS_DISTRO="humble"
elif [[ "$UBUNTU_VERSION" == "24.04" ]]; then
    ROS_DISTRO="jazzy"
else
    echo "Error: Unsupported Ubuntu version. Please use 22.04 or 24.04"
    exit 1
fi

echo "Using ROS2 $ROS_DISTRO"

# Install ROS2 desktop (includes RViz, Gazebo, etc.)
echo "Installing ROS2 desktop..."
sudo apt update
sudo apt install -y \
    ros-${ROS_DISTRO}-desktop \
    ros-${ROS_DISTRO}-ros2-control \
    ros-${ROS_DISTRO}-ros2-controllers \
    ros-${ROS_DISTRO}-gazebo-ros-pkgs \
    ros-${ROS_DISTRO}-gazebo-ros2-control \
    ros-${ROS_DISTRO}-xacro \
    ros-${ROS_DISTRO}-robot-state-publisher \
    ros-${ROS_DISTRO}-joint-state-publisher \
    ros-${ROS_DISTRO}-joint-state-publisher-gui \
    ros-${ROS_DISTRO}-robot-localization \
    ros-${ROS_DISTRO}-nav2-bringup \
    ros-${ROS_DISTRO}-slam-toolbox \
    ros-${ROS_DISTRO}-twist-mux \
    ros-${ROS_DISTRO}-teleop-twist-keyboard \
    ros-${ROS_DISTRO}-teleop-twist-joy \
    ros-${ROS_DISTRO}-joy \
    ros-${ROS_DISTRO}-depthimage-to-laserscan \
    ros-${ROS_DISTRO}-rqt* \
    python3-colcon-common-extensions \
    python3-rosdep

# Initialize rosdep if not already done
if [ ! -f /etc/ros/rosdep/sources.list.d/20-default.list ]; then
    sudo rosdep init
fi
rosdep update

# Source ROS2
source /opt/ros/${ROS_DISTRO}/setup.bash

# Install workspace dependencies
echo "Installing workspace dependencies..."
cd "$(dirname "$0")"
rosdep install --from-paths src --ignore-src -r -y

# Build the workspace
echo "Building workspace..."
colcon build --symlink-install

echo "=========================================="
echo "Installation complete!"
echo ""
echo "Add this to your ~/.bashrc:"
echo "  source /opt/ros/${ROS_DISTRO}/setup.bash"
echo "  source $(pwd)/install/setup.bash"
echo ""
echo "Then run: source ~/.bashrc"
echo ""
echo "To launch simulation:"
echo "  ros2 launch mechano_gazebo gazebo.launch.py"
echo "=========================================="
