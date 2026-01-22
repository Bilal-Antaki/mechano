# Distance sensor measurement

std_msgs/Header header

# Measured distance in meters
float32 distance_m

# Sensor range limits
float32 min_range  # meters
float32 max_range  # meters

# Field of view in radians
float32 fov

# Sensor ID for multiple sensors
uint8 sensor_id
