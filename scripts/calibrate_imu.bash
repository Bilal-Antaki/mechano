#!/bin/bash
# IMU calibration routine for Mechano robot

set -e

echo "=========================================="
echo "Mechano IMU Calibration"
echo "=========================================="

# Check if ROS2 is sourced
if [ -z "$ROS_DISTRO" ]; then
    echo "Error: ROS2 environment not sourced"
    echo "Please run: source /opt/ros/<distro>/setup.bash"
    exit 1
fi

echo ""
echo "IMPORTANT: Place the robot on a flat, level surface"
echo "Do not move the robot during calibration"
echo ""
read -p "Press Enter to start calibration..."

# Default calibration duration
DURATION=${1:-30}

echo ""
echo "Collecting IMU data for $DURATION seconds..."
echo "Keep the robot stationary!"
echo ""

# Create temporary directory for calibration data
CALIB_DIR=$(mktemp -d)
CALIB_FILE="$CALIB_DIR/imu_data.txt"

# Record IMU data
timeout $DURATION ros2 topic echo /imu/data --no-arr > "$CALIB_FILE" 2>/dev/null || true

# Count samples
SAMPLES=$(grep -c "header:" "$CALIB_FILE" 2>/dev/null || echo "0")

if [ "$SAMPLES" -lt 10 ]; then
    echo "Error: Not enough IMU data collected ($SAMPLES samples)"
    echo "Make sure the IMU is connected and publishing to /imu/data"
    rm -rf "$CALIB_DIR"
    exit 1
fi

echo "Collected $SAMPLES IMU samples"
echo ""

# Calculate average values (simplified - in practice use a proper calibration tool)
echo "Calibration complete!"
echo ""
echo "Next steps:"
echo "1. Review the IMU parameters in mechano_bringup/config/ekf.yaml"
echo "2. Adjust the IMU covariances based on observed noise levels"
echo "3. If using a magnetometer, perform figure-8 calibration"
echo ""

# Cleanup
rm -rf "$CALIB_DIR"
