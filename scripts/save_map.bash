#!/bin/bash
# Save map using nav2_map_server

set -e

# Check if ROS2 is sourced
if [ -z "$ROS_DISTRO" ]; then
    echo "Error: ROS2 environment not sourced"
    echo "Please run: source /opt/ros/<distro>/setup.bash"
    exit 1
fi

# Default map name
MAP_NAME=${1:-"map"}
MAP_DIR=${2:-"$(dirname "$0")/../src/mechano_navigation/maps"}

# Create maps directory if it doesn't exist
mkdir -p "$MAP_DIR"

MAP_PATH="$MAP_DIR/$MAP_NAME"

echo "=========================================="
echo "Saving Map"
echo "=========================================="
echo "Map name: $MAP_NAME"
echo "Save path: $MAP_PATH"
echo ""

# Save the map
ros2 run nav2_map_server map_saver_cli -f "$MAP_PATH"

echo ""
echo "Map saved successfully!"
echo "Files created:"
echo "  $MAP_PATH.yaml"
echo "  $MAP_PATH.pgm"
echo ""
echo "To use this map for navigation:"
echo "  ros2 launch mechano_navigation navigation.launch.py map:=$MAP_PATH.yaml"
