# mechano_viz

RViz configurations for the Mechano robot.

## Launch Files

### view_model.launch.py

View robot model and TF tree:
```bash
ros2 launch mechano_viz view_model.launch.py
```

### view_robot.launch.py

View robot with sensor data (camera, depth, distance):
```bash
ros2 launch mechano_viz view_robot.launch.py
```

### view_slam.launch.py

View during SLAM mapping (map, laser scan, odometry):
```bash
ros2 launch mechano_viz view_slam.launch.py
```

### view_navigation.launch.py

View during navigation (costmaps, paths, goals):
```bash
ros2 launch mechano_viz view_navigation.launch.py
```

### view_diagnostics.launch.py

Launch rqt_robot_monitor for diagnostics:
```bash
ros2 launch mechano_viz view_diagnostics.launch.py
```

## RViz Configurations

| File | Description |
|------|-------------|
| `model.rviz` | Robot model + TF |
| `sensors.rviz` | Camera, depth, distance sensor |
| `slam.rviz` | SLAM mapping view |
| `navigation.rviz` | Navigation with costmaps and paths |
| `full.rviz` | Everything combined |

## Customization

RViz configs are saved in `rviz/`. To customize:

1. Launch RViz with a config
2. Modify displays as needed
3. File > Save Config As to save changes

## Tools Available

- **Move Camera** - Orbit, pan, zoom
- **Select** - Select objects
- **Set Initial Pose** - Initialize AMCL localization
- **Set Goal** - Send navigation goals
- **Measure** - Measure distances
