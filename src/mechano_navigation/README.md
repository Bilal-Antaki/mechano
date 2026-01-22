# mechano_navigation

Navigation configuration for the Mechano robot using Nav2.

## Launch Files

### navigation.launch.py

Full Nav2 navigation stack for autonomous navigation with a known map.

```bash
ros2 launch mechano_navigation navigation.launch.py map:=/path/to/map.yaml
```

**Arguments:**
- `map` - Path to map YAML file (required)
- `use_sim_time` (default: false) - Use simulation time
- `params_file` - Custom Nav2 parameters file

### slam.launch.py

SLAM (Simultaneous Localization and Mapping) using slam_toolbox.

```bash
ros2 launch mechano_navigation slam.launch.py
```

**Arguments:**
- `use_sim_time` (default: false) - Use simulation time
- `slam_params_file` - Custom SLAM parameters file

### localization.launch.py

Localization only using AMCL (when you already have a map).

```bash
ros2 launch mechano_navigation localization.launch.py map:=/path/to/map.yaml
```

## Creating Maps

1. Start the robot:
   ```bash
   ros2 launch mechano_bringup bringup.launch.py
   ```

2. Start SLAM:
   ```bash
   ros2 launch mechano_navigation slam.launch.py
   ```

3. Drive around to map the environment:
   ```bash
   ros2 launch mechano_bringup teleop.launch.py
   ```

4. Save the map:
   ```bash
   ./scripts/save_map.bash my_map
   ```

## Navigation

1. Start the robot:
   ```bash
   ros2 launch mechano_bringup bringup.launch.py
   ```

2. Start navigation:
   ```bash
   ros2 launch mechano_navigation navigation.launch.py map:=/path/to/map.yaml
   ```

3. Use RViz to set goals:
   ```bash
   ros2 launch mechano_viz view_navigation.launch.py
   ```

## Configuration Files

| File | Description |
|------|-------------|
| `nav2_params.yaml` | Complete Nav2 stack configuration |
| `slam_params.yaml` | SLAM Toolbox parameters |

## Behavior Trees

| File | Description |
|------|-------------|
| `navigate_to_pose.xml` | Navigate to a single goal |
| `navigate_through_poses.xml` | Navigate through waypoints |

## Tuning

See [docs/navigation_tuning.md](../../docs/navigation_tuning.md) for tuning guidance.

### Key Parameters

**Costmaps:**
- `robot_radius` - Robot footprint radius
- `inflation_radius` - Safety margin around obstacles
- `resolution` - Costmap cell size

**Controller:**
- `desired_linear_vel` - Target forward speed
- `lookahead_dist` - How far ahead to look for path following
- `rotate_to_heading_angular_vel` - Rotation speed

**Planner:**
- `tolerance` - Goal position tolerance
- `use_astar` - Use A* (faster) or Dijkstra
