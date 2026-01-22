#!/bin/bash
# Create udev rules for Mechano robot hardware

set -e

RULES_FILE="/etc/udev/rules.d/99-mechano.rules"

echo "Creating udev rules for Mechano robot..."

# Create rules file
sudo tee $RULES_FILE > /dev/null << 'EOF'
# Mechano Robot udev rules
# This file provides consistent device names for robot hardware

# Arduino/Microcontroller (typical FTDI or CH340 chips)
# Creates symlink /dev/mechano_mcu
SUBSYSTEM=="tty", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0043", SYMLINK+="mechano_mcu", MODE="0666"
SUBSYSTEM=="tty", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", SYMLINK+="mechano_mcu", MODE="0666"
SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", SYMLINK+="mechano_mcu", MODE="0666"

# IMU (typical USB IMU devices)
# Creates symlink /dev/mechano_imu
SUBSYSTEM=="tty", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", SYMLINK+="mechano_imu", MODE="0666"

# Stereo Camera (OAK-D or similar)
# Allow access without root
SUBSYSTEM=="usb", ATTRS{idVendor}=="03e7", MODE="0666"

# Joystick/Gamepad
# Allow access without root
KERNEL=="js*", MODE="0666"
KERNEL=="event*", SUBSYSTEM=="input", MODE="0666"
EOF

echo "Udev rules created at $RULES_FILE"
echo ""
echo "To apply the rules, run:"
echo "  sudo udevadm control --reload-rules"
echo "  sudo udevadm trigger"
echo ""
echo "Then reconnect your devices."
