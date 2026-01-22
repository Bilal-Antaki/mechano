# ros2_control Overview

This document explains how ros2_control is used in the Mechano robot.

## What is ros2_control?

ros2_control is a framework for real-time control of robots in ROS2. It provides:
- Abstraction between hardware and controllers
- Real-time safe data exchange
- Standardized controller interfaces

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                   Controller Manager                      │
│  ┌─────────────────────┐  ┌─────────────────────────┐   │
│  │ diff_drive_controller│  │ joint_state_broadcaster │   │
│  └──────────┬──────────┘  └────────────┬────────────┘   │
│             │                          │                  │
│             │     Command/State        │                  │
│             │      Interfaces          │                  │
│             ▼                          ▼                  │
│  ┌─────────────────────────────────────────────────┐    │
│  │              Hardware Interface                   │    │
│  │           (MechanoSystemHardware)                │    │
│  └──────────────────────┬──────────────────────────┘    │
└─────────────────────────┼───────────────────────────────┘
                          │
                          │ Serial Communication
                          ▼
                ┌─────────────────────┐
                │   Microcontroller   │
                │    (Motor Control)   │
                └─────────────────────┘
```

## Hardware Interface

The `MechanoSystemHardware` class in `mechano_hardware` implements:

### Interfaces Exported

**Command Interfaces:**
- `left_wheel_joint/velocity` - Left wheel velocity command (rad/s)
- `right_wheel_joint/velocity` - Right wheel velocity command (rad/s)

**State Interfaces:**
- `left_wheel_joint/position` - Left wheel position (rad)
- `left_wheel_joint/velocity` - Left wheel velocity (rad/s)
- `right_wheel_joint/position` - Right wheel position (rad)
- `right_wheel_joint/velocity` - Right wheel velocity (rad/s)

### Lifecycle

```cpp
on_init()       // Parse URDF parameters
on_configure()  // Open serial connection
on_activate()   // Start communication
read()          // Read encoder feedback (called at control rate)
write()         // Send velocity commands (called at control rate)
on_deactivate() // Stop motors
on_cleanup()    // Close serial connection
```

## Controllers

### diff_drive_controller

Converts `/cmd_vel` (Twist) to wheel velocities:

```yaml
diff_drive_controller:
  ros__parameters:
    left_wheel_names: ["left_wheel_joint"]
    right_wheel_names: ["right_wheel_joint"]
    wheel_separation: 0.30  # meters
    wheel_radius: 0.05      # meters
    publish_rate: 50.0      # Hz
```

### joint_state_broadcaster

Publishes joint states to `/joint_states`:

```yaml
joint_state_broadcaster:
  ros__parameters:
    type: joint_state_broadcaster/JointStateBroadcaster
```

## URDF Configuration

The hardware interface is defined in URDF:

```xml
<ros2_control name="MechanoSystem" type="system">
  <hardware>
    <plugin>mechano_hardware/MechanoSystemHardware</plugin>
    <param name="device">/dev/mechano_mcu</param>
    <param name="baud_rate">115200</param>
    <param name="timeout_ms">1000</param>
  </hardware>

  <joint name="left_wheel_joint">
    <command_interface name="velocity"/>
    <state_interface name="position"/>
    <state_interface name="velocity"/>
  </joint>

  <joint name="right_wheel_joint">
    <command_interface name="velocity"/>
    <state_interface name="position"/>
    <state_interface name="velocity"/>
  </joint>
</ros2_control>
```

## Serial Protocol

Communication with the microcontroller uses a simple ASCII protocol:

**Commands (to MCU):**
```
v <left_vel> <right_vel>\n   # Set wheel velocities (rad/s)
e\n                           # Request encoder values
```

**Responses (from MCU):**
```
<left_pos> <right_pos> <left_vel> <right_vel>\n  # Encoder feedback
```

## Launching Controllers

Controllers are spawned via launch file:

```python
# Spawn controller manager
controller_manager = Node(
    package='controller_manager',
    executable='ros2_control_node',
    parameters=[robot_controllers],
)

# Spawn diff_drive_controller
diff_drive_spawner = Node(
    package='controller_manager',
    executable='spawner',
    arguments=['diff_drive_controller'],
)

# Spawn joint_state_broadcaster
jsb_spawner = Node(
    package='controller_manager',
    executable='spawner',
    arguments=['joint_state_broadcaster'],
)
```

## Debugging

### Check controller status
```bash
ros2 control list_controllers
```

### Check hardware interfaces
```bash
ros2 control list_hardware_interfaces
```

### Monitor commands
```bash
ros2 topic echo /diff_drive_controller/cmd_vel_unstamped
```

## See Also

- [ros2_control documentation](https://control.ros.org/)
- [diff_drive_controller](https://control.ros.org/master/doc/ros2_controllers/diff_drive_controller/doc/userdoc.html)
