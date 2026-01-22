# mechano_perception

Perception nodes for the Mechano robot (camera and sensors).

## Nodes

### distance_sensor_node

Reads from a 1D distance sensor (e.g., VL53L0X) and publishes `sensor_msgs/Range`.

**Published Topics:**
- `distance` (`sensor_msgs/Range`) - Distance measurement

**Parameters:**
- `frame_id` - TF frame for the sensor
- `device` - Serial port device
- `min_range` - Minimum detectable range (m)
- `max_range` - Maximum detectable range (m)
- `field_of_view` - Sensor field of view (rad)
- `publish_rate` - Publishing frequency (Hz)

## Launch Files

### perception.launch.py

Launch all perception nodes:
```bash
ros2 launch mechano_perception perception.launch.py
```

### stereo_camera.launch.py

Launch the stereo camera driver:
```bash
ros2 launch mechano_perception stereo_camera.launch.py
```

Note: You need to configure this for your specific camera (OAK-D, RealSense, etc.)

### distance_sensor.launch.py

Launch the distance sensor node:
```bash
ros2 launch mechano_perception distance_sensor.launch.py
```

### depth_to_scan.launch.py

Convert depth image to laser scan for navigation:
```bash
ros2 launch mechano_perception depth_to_scan.launch.py
```

## Configuration

| File | Description |
|------|-------------|
| `stereo_camera.yaml` | Camera driver parameters |
| `distance_sensor.yaml` | Distance sensor parameters |
| `depth_to_scan.yaml` | Depth to laserscan conversion |

## Supported Cameras

The camera launch file is a template. Configure for your camera:

### OAK-D (Luxonis)
```bash
sudo apt install ros-${ROS_DISTRO}-depthai-ros
```

### Intel RealSense
```bash
sudo apt install ros-${ROS_DISTRO}-realsense2-camera
```

### Orbbec
```bash
sudo apt install ros-${ROS_DISTRO}-orbbec-camera
```

## Adding New Sensors

1. Create a new node in `src/`
2. Add header to `include/mechano_perception/`
3. Add configuration in `config/`
4. Create launch file in `launch/`
5. Update `perception.launch.py` to include it
