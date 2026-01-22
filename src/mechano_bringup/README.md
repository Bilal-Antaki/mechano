# mechano_bringup

Launch files and configurations for bringing up the Mechano robot.

## Launch Files

### bringup.launch.py

Main launch file for the real robot. Starts:
- Robot description (URDF)
- ros2_control controller manager
- diff_drive_controller
- joint_state_broadcaster
- EKF sensor fusion
- Twist multiplexer

```bash
ros2 launch mechano_bringup bringup.launch.py
```

**Arguments:**
- `use_sim` (default: false) - Use simulation mode
- `use_fake_hardware` (default: false) - Use mock hardware interface

### controllers.launch.py

Launches the ros2_control stack:
- Controller manager
- Differential drive controller
- Joint state broadcaster

```bash
ros2 launch mechano_bringup controllers.launch.py
```

### sensors.launch.py

Launches sensor drivers:
- Stereo camera
- Distance sensors

```bash
ros2 launch mechano_bringup sensors.launch.py
```

### teleop.launch.py

Keyboard teleoperation:

```bash
ros2 launch mechano_bringup teleop.launch.py
```

### joy_teleop.launch.py

Joystick teleoperation:

```bash
ros2 launch mechano_bringup joy_teleop.launch.py
```

## Configuration Files

| File | Description |
|------|-------------|
| `mechano_controllers.yaml` | ros2_control configuration |
| `ekf.yaml` | robot_localization EKF parameters |
| `twist_mux.yaml` | Velocity command priority settings |
| `joy_teleop.yaml` | Joystick button/axis mappings |
| `robot_params.yaml` | General robot parameters |

## Topics

### Velocity Commands

The twist_mux prioritizes velocity commands:

| Priority | Topic | Source |
|----------|-------|--------|
| 255 | e_stop | Emergency stop (locks all motion) |
| 100 | cmd_vel_teleop | Teleop (keyboard/joystick) |
| 50 | cmd_vel_recovery | Recovery behaviors |
| 10 | cmd_vel_nav | Navigation stack |

Output goes to `diff_drive_controller/cmd_vel_unstamped`.

### Odometry

| Topic | Description |
|-------|-------------|
| `/diff_drive_controller/odom` | Raw wheel odometry |
| `/odom` | Filtered odometry (EKF output) |

## Customization

### Changing velocity limits

Edit `config/mechano_controllers.yaml`:

```yaml
linear.x.max_velocity: 0.5  # m/s
angular.z.max_velocity: 1.5  # rad/s
```

### Joystick configuration

Edit `config/joy_teleop.yaml` to match your controller:

```yaml
axis_linear:
  x: 1  # Adjust axis numbers
axis_angular:
  yaw: 0
enable_button: 4  # Adjust button numbers
```
