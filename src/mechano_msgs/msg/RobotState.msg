# Robot state message

std_msgs/Header header

# Operating mode
uint8 MODE_IDLE = 0
uint8 MODE_TELEOP = 1
uint8 MODE_AUTONOMOUS = 2
uint8 MODE_CHARGING = 3
uint8 mode

# Battery percentage (0-100)
float32 battery_pct

# E-stop state
bool estop_active

# Error code (0 = no error)
uint16 error_code

# Error message (empty if no error)
string error_message
