#!/bin/bash
# Diagnostic script to verify all Mechano robot hardware

echo "=========================================="
echo "Mechano Hardware Diagnostic"
echo "=========================================="
echo ""

ERRORS=0

# Function to check device
check_device() {
    local device=$1
    local name=$2

    if [ -e "$device" ]; then
        echo "[OK] $name found at $device"
        return 0
    else
        echo "[FAIL] $name NOT found at $device"
        return 1
    fi
}

# Function to check USB device by vendor:product ID
check_usb() {
    local vid_pid=$1
    local name=$2

    if lsusb | grep -q "$vid_pid"; then
        echo "[OK] $name detected (USB $vid_pid)"
        return 0
    else
        echo "[FAIL] $name NOT detected (USB $vid_pid)"
        return 1
    fi
}

echo "=== Serial Devices ==="

# Check for microcontroller
if [ -e "/dev/mechano_mcu" ]; then
    echo "[OK] Microcontroller found at /dev/mechano_mcu"
elif ls /dev/ttyUSB* 2>/dev/null | head -1 > /dev/null; then
    echo "[WARN] Microcontroller may be at $(ls /dev/ttyUSB* | head -1)"
    echo "       Run setup_udev_rules.bash for consistent naming"
elif ls /dev/ttyACM* 2>/dev/null | head -1 > /dev/null; then
    echo "[WARN] Microcontroller may be at $(ls /dev/ttyACM* | head -1)"
    echo "       Run setup_udev_rules.bash for consistent naming"
else
    echo "[FAIL] No microcontroller found"
    ((ERRORS++))
fi

echo ""
echo "=== USB Devices ==="

# Check for common camera devices
if lsusb | grep -q "03e7:"; then
    echo "[OK] Intel/Luxonis depth camera detected"
elif lsusb | grep -q "8086:"; then
    echo "[OK] Intel RealSense camera may be present"
elif lsusb | grep -q "2bc5:"; then
    echo "[OK] Orbbec camera detected"
else
    echo "[WARN] No known depth camera detected"
fi

# Check for joystick
if ls /dev/input/js* 2>/dev/null | head -1 > /dev/null; then
    echo "[OK] Joystick found at $(ls /dev/input/js* | head -1)"
else
    echo "[INFO] No joystick detected (optional)"
fi

echo ""
echo "=== ROS2 Environment ==="

if [ -n "$ROS_DISTRO" ]; then
    echo "[OK] ROS2 $ROS_DISTRO is sourced"
else
    echo "[FAIL] ROS2 environment not sourced"
    ((ERRORS++))
fi

if [ -n "$AMENT_PREFIX_PATH" ] && echo "$AMENT_PREFIX_PATH" | grep -q "mechano"; then
    echo "[OK] Mechano workspace is sourced"
else
    echo "[WARN] Mechano workspace may not be sourced"
fi

echo ""
echo "=== Network ==="

# Check ROS_DOMAIN_ID
echo "[INFO] ROS_DOMAIN_ID=${ROS_DOMAIN_ID:-0}"

# Check for multicast
if ping -c 1 -W 1 224.0.0.1 &>/dev/null; then
    echo "[OK] Multicast appears to be working"
else
    echo "[WARN] Multicast may not be available"
fi

echo ""
echo "=========================================="
if [ $ERRORS -eq 0 ]; then
    echo "Hardware check passed!"
else
    echo "Hardware check completed with $ERRORS error(s)"
fi
echo "=========================================="

exit $ERRORS
