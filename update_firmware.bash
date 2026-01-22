#!/bin/bash
# Script to flash microcontroller firmware

set -e

echo "=========================================="
echo "Mechano Firmware Update Script"
echo "=========================================="

# Default values
FIRMWARE_DIR="firmware"
BOARD="arduino:avr:mega"
PORT="/dev/ttyUSB0"

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --port)
            PORT="$2"
            shift 2
            ;;
        --board)
            BOARD="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Check if arduino-cli is installed
if ! command -v arduino-cli &> /dev/null; then
    echo "arduino-cli not found. Installing..."
    curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
    export PATH="$PATH:$HOME/bin"
fi

# Check if firmware directory exists
if [ ! -d "$FIRMWARE_DIR" ]; then
    echo "Error: Firmware directory not found at $FIRMWARE_DIR"
    echo "Please download the firmware from the project repository."
    exit 1
fi

# Detect port if not specified
if [ ! -e "$PORT" ]; then
    echo "Looking for Arduino..."
    DETECTED_PORT=$(arduino-cli board list | grep -E "ttyUSB|ttyACM" | head -n1 | awk '{print $1}')
    if [ -n "$DETECTED_PORT" ]; then
        PORT="$DETECTED_PORT"
        echo "Found Arduino at $PORT"
    else
        echo "Error: No Arduino found. Please connect the microcontroller."
        exit 1
    fi
fi

echo "Compiling firmware..."
arduino-cli compile --fqbn "$BOARD" "$FIRMWARE_DIR"

echo "Uploading firmware to $PORT..."
arduino-cli upload -p "$PORT" --fqbn "$BOARD" "$FIRMWARE_DIR"

echo "=========================================="
echo "Firmware update complete!"
echo "=========================================="
