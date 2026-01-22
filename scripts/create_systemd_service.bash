#!/bin/bash
# Create systemd service for Mechano robot auto-start

set -e

echo "=========================================="
echo "Mechano Systemd Service Setup"
echo "=========================================="

# Get the workspace path
WORKSPACE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
ROS_DISTRO=${ROS_DISTRO:-humble}

# Service file content
SERVICE_FILE="/etc/systemd/system/mechano.service"

echo "Creating systemd service..."
echo "Workspace: $WORKSPACE_DIR"
echo "ROS Distro: $ROS_DISTRO"
echo ""

sudo tee $SERVICE_FILE > /dev/null << EOF
[Unit]
Description=Mechano Robot ROS2 Bringup
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
User=$USER
Group=$USER
Environment="HOME=$HOME"
Environment="ROS_DOMAIN_ID=0"
ExecStart=/bin/bash -c 'source /opt/ros/$ROS_DISTRO/setup.bash && source $WORKSPACE_DIR/install/setup.bash && ros2 launch mechano_bringup bringup.launch.py'
Restart=on-failure
RestartSec=10

[Install]
WantedBy=multi-user.target
EOF

echo "Service file created at $SERVICE_FILE"
echo ""

# Reload systemd
sudo systemctl daemon-reload

echo "=========================================="
echo "Service created successfully!"
echo ""
echo "To enable auto-start on boot:"
echo "  sudo systemctl enable mechano.service"
echo ""
echo "To start the service now:"
echo "  sudo systemctl start mechano.service"
echo ""
echo "To check status:"
echo "  sudo systemctl status mechano.service"
echo ""
echo "To view logs:"
echo "  journalctl -u mechano.service -f"
echo "=========================================="
