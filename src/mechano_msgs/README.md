# mechano_msgs

Custom message and service definitions for the Mechano robot.

## Messages

### RobotState.msg

Overall robot state information.

| Field | Type | Description |
|-------|------|-------------|
| header | std_msgs/Header | Timestamp and frame |
| mode | uint8 | Operating mode (IDLE, TELEOP, AUTONOMOUS, CHARGING) |
| battery_pct | float32 | Battery percentage (0-100) |
| estop_active | bool | Emergency stop state |
| error_code | uint16 | Error code (0 = no error) |
| error_message | string | Human-readable error message |

### WheelStates.msg

Wheel encoder feedback.

| Field | Type | Description |
|-------|------|-------------|
| header | std_msgs/Header | Timestamp and frame |
| left_encoder_ticks | int32 | Left wheel encoder count |
| left_velocity | float32 | Left wheel velocity (rad/s) |
| right_encoder_ticks | int32 | Right wheel encoder count |
| right_velocity | float32 | Right wheel velocity (rad/s) |

### DistanceSensor.msg

1D distance sensor measurement.

| Field | Type | Description |
|-------|------|-------------|
| header | std_msgs/Header | Timestamp and frame |
| distance_m | float32 | Measured distance (meters) |
| min_range | float32 | Minimum detectable range (meters) |
| max_range | float32 | Maximum detectable range (meters) |
| fov | float32 | Field of view (radians) |
| sensor_id | uint8 | Sensor identifier |

## Services

### SetMode.srv

Change the robot's operating mode.

**Request:**
- `mode` (uint8): Target mode (MODE_IDLE=0, MODE_TELEOP=1, MODE_AUTONOMOUS=2)

**Response:**
- `success` (bool): Whether mode change succeeded
- `message` (string): Status or error message

### EStop.srv

Activate or release the software emergency stop.

**Request:**
- `activate` (bool): true to activate E-stop, false to release

**Response:**
- `success` (bool): Whether operation succeeded
- `message` (string): Status or error message

## Usage

```cpp
#include "mechano_msgs/msg/robot_state.hpp"
#include "mechano_msgs/srv/set_mode.hpp"
```

```python
from mechano_msgs.msg import RobotState
from mechano_msgs.srv import SetMode
```
