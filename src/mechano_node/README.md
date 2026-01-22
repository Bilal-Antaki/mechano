# mechano_node

Main robot node for the Mechano robot, handling HMI (Human-Machine Interface) and behaviors.

## Features

- Robot state management (IDLE, TELEOP, AUTONOMOUS)
- E-Stop functionality
- Battery monitoring with low/critical warnings
- Safety monitoring (obstacle detection)
- LED status indication
- Button input handling

## Node: mechano_node

### Published Topics

- `robot_state` (`mechano_msgs/RobotState`) - Current robot state

### Services

- `set_mode` (`mechano_msgs/SetMode`) - Change robot operating mode
- `estop` (`mechano_msgs/EStop`) - Activate/deactivate E-Stop

### Subscribed Topics

- `battery_state` (`sensor_msgs/BatteryState`) - Battery information
- `distance` (`sensor_msgs/Range`) - Distance sensor for safety

### Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `battery.low_threshold` | float | 20.0 | Low battery warning (%) |
| `battery.critical_threshold` | float | 10.0 | Critical battery level (%) |
| `safety.min_distance` | float | 0.2 | Minimum safe distance (m) |

## Usage

```bash
ros2 launch mechano_node mechano_node.launch.py
```

## LED Status Indicators

| Color | Pattern | Meaning |
|-------|---------|---------|
| Green | Solid | Idle, ready |
| Green | Pulse | Autonomous mode |
| Blue | Solid | Teleop mode |
| Yellow | Slow blink | Low battery |
| Red | Fast blink | Critical battery / Safety trigger |
| Red | Solid | E-Stop active |

## Services

### Set Mode

```bash
# Switch to teleop mode
ros2 service call /set_mode mechano_msgs/srv/SetMode "{mode: 1}"

# Switch to autonomous mode
ros2 service call /set_mode mechano_msgs/srv/SetMode "{mode: 2}"

# Switch to idle
ros2 service call /set_mode mechano_msgs/srv/SetMode "{mode: 0}"
```

### E-Stop

```bash
# Activate E-Stop
ros2 service call /estop mechano_msgs/srv/EStop "{activate: true}"

# Release E-Stop
ros2 service call /estop mechano_msgs/srv/EStop "{activate: false}"
```

## Customization

### Button Functions

Edit `config/mechano_node.yaml`:

```yaml
buttons:
  button_1: "toggle_mode"
  button_2: "estop"
  button_3: "horn"
```

### Adding Behaviors

Extend the `MechanoNode` class in `src/mechano_node.cpp` to add custom behaviors.
