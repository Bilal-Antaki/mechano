#!/bin/bash
set -e

# Source ROS2 installation
source /opt/ros/${ROS_DISTRO}/setup.bash

# Source workspace if it exists
if [ -f /ros2_ws/install/setup.bash ]; then
    source /ros2_ws/install/setup.bash
fi

# Execute the command passed to the container
exec "$@"
