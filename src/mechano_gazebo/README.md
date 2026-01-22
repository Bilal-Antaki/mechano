# mechano_gazebo

Gazebo simulation for the Mechano robot.

## Launch Files

### gazebo.launch.py

Main simulation launch file. Starts:
- Gazebo with specified world
- Robot model spawning
- ros2_control controllers
- Optionally RViz

```bash
ros2 launch mechano_gazebo gazebo.launch.py
```

**Arguments:**
- `world` (default: empty) - World to load: `empty`, `house`, `warehouse`
- `use_rviz` (default: true) - Launch RViz

**Examples:**
```bash
# Empty world
ros2 launch mechano_gazebo gazebo.launch.py world:=empty

# House environment
ros2 launch mechano_gazebo gazebo.launch.py world:=house

# Warehouse environment
ros2 launch mechano_gazebo gazebo.launch.py world:=warehouse
```

### spawn_robot.launch.py

Spawn the robot into a running Gazebo instance:

```bash
ros2 launch mechano_gazebo spawn_robot.launch.py x:=1.0 y:=2.0
```

**Arguments:**
- `x`, `y`, `z` - Spawn position
- `yaw` - Initial orientation

## Worlds

| World | Description |
|-------|-------------|
| `empty.world` | Empty world for basic testing |
| `house.world` | Indoor house with rooms and furniture |
| `warehouse.world` | Warehouse with shelving and obstacles |

## Adding Custom Worlds

1. Create a new `.world` file in `worlds/`
2. Follow the SDF format
3. Use the `world` argument to load it

Example:
```xml
<?xml version="1.0" ?>
<sdf version="1.6">
  <world name="my_world">
    <include><uri>model://sun</uri></include>
    <include><uri>model://ground_plane</uri></include>
    <!-- Add models here -->
  </world>
</sdf>
```

## Adding Custom Models

1. Create model directory in `models/`
2. Add `model.config` and `model.sdf`
3. Reference in world file

```
models/
  my_model/
    model.config
    model.sdf
    meshes/
      model.dae
```

## Simulation Tips

- Use `use_sim_time:=true` for all nodes
- The simulation runs at real-time by default
- Check physics settings in world files for stability

## Driving in Simulation

After launching, use teleop:
```bash
ros2 launch mechano_bringup teleop.launch.py
```

Or send commands directly:
```bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.2}, angular: {z: 0.0}}"
```
