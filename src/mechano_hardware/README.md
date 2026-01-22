# mechano_hardware

ros2_control hardware interface for the Mechano robot.

## Overview

This package implements a `SystemInterface` for ros2_control that communicates with an Arduino microcontroller to control the robot's motors and read encoder feedback.

## Hardware Interface

### Exported Interfaces

**Command Interfaces:**
- `left_wheel_joint/velocity` - Left wheel velocity command (rad/s)
- `right_wheel_joint/velocity` - Right wheel velocity command (rad/s)

**State Interfaces:**
- `left_wheel_joint/position` - Left wheel position (rad)
- `left_wheel_joint/velocity` - Left wheel velocity (rad/s)
- `right_wheel_joint/position` - Right wheel position (rad)
- `right_wheel_joint/velocity` - Right wheel velocity (rad/s)

## Serial Protocol

The hardware interface communicates with the Arduino using a simple ASCII protocol:

### Commands (sent to Arduino)

| Command | Format | Description |
|---------|--------|-------------|
| Read encoders | `e\n` | Request current encoder values |
| Set velocity | `v <left> <right>\n` | Set wheel velocities (rad/s) |

### Responses (from Arduino)

| Response | Format | Description |
|----------|--------|-------------|
| Encoder values | `<left> <right>\n` | Encoder tick counts |

## Configuration

Configure in URDF:

```xml
<ros2_control name="MechanoSystem" type="system">
  <hardware>
    <plugin>mechano_hardware/MechanoSystemHardware</plugin>
    <param name="device">/dev/mechano_mcu</param>
    <param name="baud_rate">115200</param>
    <param name="timeout_ms">1000</param>
    <param name="enc_counts_per_rev">1440</param>
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

### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `device` | string | Serial port device path |
| `baud_rate` | int | Serial baud rate (default: 115200) |
| `timeout_ms` | int | Serial read timeout in milliseconds |
| `enc_counts_per_rev` | int | Encoder counts per wheel revolution |

## Arduino Firmware

The Arduino firmware should implement the serial protocol described above. Example code structure:

```cpp
void setup() {
  Serial.begin(115200);
  // Initialize motor drivers and encoders
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');

    if (cmd.startsWith("e")) {
      // Return encoder values
      Serial.print(leftEncoder);
      Serial.print(" ");
      Serial.println(rightEncoder);
    }
    else if (cmd.startsWith("v")) {
      // Parse and set velocities
      float leftVel, rightVel;
      sscanf(cmd.c_str(), "v %f %f", &leftVel, &rightVel);
      setMotorVelocities(leftVel, rightVel);
    }
  }
}
```

## Building

```bash
cd ~/ros2_ws
colcon build --packages-select mechano_hardware
```

## Testing

Test with fake hardware:

```bash
ros2 launch mechano_bringup bringup.launch.py use_fake_hardware:=true
```
