# Troubleshooting Guide

Common issues and solutions for the Mechano robot.

## Hardware Issues

### Robot doesn't power on

1. Check battery charge level
2. Verify E-Stop is not engaged
3. Check fuse
4. Test power switch
5. Measure voltage at key points with multimeter

### Motors don't move

1. Check motor driver connections
2. Verify motor driver is receiving power
3. Check control signal wiring to microcontroller
4. Test motors directly with 12V (briefly)
5. Check serial communication:
   ```bash
   ros2 topic echo /joint_states
   ```

### Encoders not reading

1. Verify encoder power (usually 5V)
2. Check encoder signal wiring
3. Ensure interrupt pins are used on microcontroller
4. Test with `ros2 topic hz /joint_states`

### Camera not detected

1. Check USB connection
2. Verify USB power is sufficient
3. Try different USB port
4. Check with:
   ```bash
   lsusb | grep -i depth
   ```

## Software Issues

### Build fails

1. Ensure ROS2 is sourced:
   ```bash
   source /opt/ros/humble/setup.bash
   ```
2. Install missing dependencies:
   ```bash
   rosdep install --from-paths src --ignore-src -r -y
   ```
3. Clean and rebuild:
   ```bash
   rm -rf build install log
   colcon build
   ```

### Cannot connect to robot topics

1. Check ROS_DOMAIN_ID matches:
   ```bash
   echo $ROS_DOMAIN_ID
   ```
2. Verify network connectivity
3. Check firewall rules
4. Try with multicast disabled:
   ```bash
   export ROS_LOCALHOST_ONLY=1
   ```

### Hardware interface fails to start

1. Check serial port exists:
   ```bash
   ls -la /dev/mechano_mcu
   ```
2. Verify permissions:
   ```bash
   sudo chmod 666 /dev/ttyUSB0
   ```
3. Run udev rules setup:
   ```bash
   ./scripts/setup_udev_rules.bash
   ```

### TF errors / "Could not find transform"

1. Check TF tree:
   ```bash
   ros2 run tf2_tools view_frames
   ```
2. Verify robot_state_publisher is running
3. Check URDF is being published correctly
4. Ensure all nodes publishing TF are running

## Navigation Issues

### Robot doesn't move to goal

1. Verify localization:
   ```bash
   ros2 topic echo /amcl_pose
   ```
2. Check costmaps in RViz
3. Verify global and local plans are being generated
4. Check cmd_vel is being published:
   ```bash
   ros2 topic echo /cmd_vel
   ```

### Robot oscillates or moves erratically

1. Check odometry quality
2. Reduce controller velocity limits
3. Increase goal tolerance
4. Check for sensor noise in costmap

### Robot hits obstacles

1. Increase inflation radius
2. Check sensor data is being received
3. Verify obstacle layer is enabled
4. Check robot footprint size

### SLAM map is poor quality

1. Drive slowly during mapping
2. Ensure good lighting (for camera)
3. Check for wheel slippage
4. Revisit areas to close loops

## Simulation Issues

### Gazebo crashes

1. Check GPU drivers are up to date
2. Try software rendering:
   ```bash
   export LIBGL_ALWAYS_SOFTWARE=1
   ```
3. Reduce simulation complexity

### Robot falls through ground in Gazebo

1. Check collision geometry in URDF
2. Verify inertia values are reasonable
3. Reduce physics step size

### Sensors don't work in simulation

1. Ensure gazebo plugins are loaded
2. Check plugin topic names match expected
3. Verify sensor_msgs types are correct

## Diagnostic Commands

### Check system status
```bash
./scripts/check_hardware.bash
```

### View all topics
```bash
ros2 topic list
```

### Check node status
```bash
ros2 node list
```

### Monitor CPU/memory
```bash
htop
```

### View ROS2 logs
```bash
ros2 run rqt_console rqt_console
```

### Record data for analysis
```bash
ros2 bag record -a
```

## Getting Help

If you're still stuck:
1. Check the [GitHub Issues](https://github.com/yourusername/mechano_ws/issues)
2. Search ROS Answers and ROS Discourse
3. Create a new issue with:
   - Description of the problem
   - Steps to reproduce
   - Relevant log output
   - System information (ROS version, Ubuntu version)
