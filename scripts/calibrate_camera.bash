#!/bin/bash
# Stereo camera calibration script for Mechano robot

set -e

echo "=========================================="
echo "Mechano Camera Calibration"
echo "=========================================="

# Check if ROS2 is sourced
if [ -z "$ROS_DISTRO" ]; then
    echo "Error: ROS2 environment not sourced"
    echo "Please run: source /opt/ros/<distro>/setup.bash"
    exit 1
fi

# Default parameters
BOARD_SIZE=${1:-"8x6"}
SQUARE_SIZE=${2:-"0.025"}

echo ""
echo "Camera Calibration Settings:"
echo "  Checkerboard size: $BOARD_SIZE (inner corners)"
echo "  Square size: ${SQUARE_SIZE}m"
echo ""
echo "Instructions:"
echo "1. Print a checkerboard calibration pattern"
echo "2. Hold the pattern in front of the camera"
echo "3. Move the pattern to cover all areas of the image"
echo "4. Tilt the pattern at various angles"
echo "5. Collect at least 20-30 images"
echo ""
read -p "Press Enter to start calibration..."

# Launch camera calibration
echo "Starting camera calibration..."
echo "A window will open - follow the on-screen instructions"
echo ""

ros2 run camera_calibration cameracalibrator \
    --size $BOARD_SIZE \
    --square $SQUARE_SIZE \
    --ros-args \
    --remap image:=/camera/left/image_raw \
    --remap camera:=/camera/left

echo ""
echo "Calibration complete!"
echo "The calibration file has been saved."
echo ""
echo "Copy the calibration to the config directory:"
echo "  cp /tmp/calibrationdata.tar.gz src/mechano_perception/config/"
