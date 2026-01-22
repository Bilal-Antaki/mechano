# Navigation Tuning Guide

This guide helps you tune the Nav2 navigation stack for optimal performance.

## Overview

Navigation performance depends on:
1. Accurate odometry and localization
2. Proper costmap configuration
3. Well-tuned planner and controller

## Step 1: Verify Odometry

Before tuning navigation, ensure odometry is accurate.

### Test Wheel Odometry

1. Mark the robot's starting position
2. Drive forward 1 meter using teleop
3. Check the reported odometry matches actual movement:
   ```bash
   ros2 topic echo /odom
   ```

### Common Issues

- **Drift**: Check wheel radius and separation in URDF
- **Slipping**: Reduce acceleration limits
- **Noise**: Tune encoder filtering

## Step 2: Tune the EKF

The EKF fuses wheel odometry with IMU data.

```yaml
# ekf.yaml
ekf_filter_node:
  ros__parameters:
    frequency: 50.0

    odom0: odom
    odom0_config: [false, false, false,  # x, y, z
                   false, false, false,  # roll, pitch, yaw
                   true, true, false,    # vx, vy, vz
                   false, false, true,   # vroll, vpitch, vyaw
                   false, false, false]  # ax, ay, az

    imu0: imu/data
    imu0_config: [false, false, false,  # x, y, z
                  true, true, true,     # roll, pitch, yaw
                  false, false, false,  # vx, vy, vz
                  true, true, true,     # vroll, vpitch, vyaw
                  true, true, true]     # ax, ay, az
```

### EKF Tips

- Use odometry for velocity, IMU for orientation
- Increase process noise if tracking is slow
- Decrease process noise if output is noisy

## Step 3: Configure Costmaps

### Global Costmap

For long-range planning:

```yaml
global_costmap:
  ros__parameters:
    update_frequency: 1.0
    publish_frequency: 1.0
    resolution: 0.05        # 5cm cells
    robot_radius: 0.20      # Must clear obstacles by this amount

    plugins: ["static_layer", "obstacle_layer", "inflation_layer"]

    inflation_layer:
      cost_scaling_factor: 3.0   # Higher = costs drop faster from obstacles
      inflation_radius: 0.55     # Robot radius + safety margin
```

### Local Costmap

For reactive obstacle avoidance:

```yaml
local_costmap:
  ros__parameters:
    update_frequency: 5.0
    publish_frequency: 2.0
    resolution: 0.05
    width: 3.0              # Local window size
    height: 3.0

    plugins: ["voxel_layer", "inflation_layer"]

    voxel_layer:
      observation_sources: scan
      scan:
        topic: /scan
        max_obstacle_height: 2.0
        clearing: true
        marking: true
```

### Costmap Tips

- Start with conservative (larger) inflation radius
- Reduce resolution for faster updates, increase for tight spaces
- Use rolling window for local costmap

## Step 4: Tune the Planner

### NavFn (Dijkstra/A*)

Good default choice:

```yaml
planner_server:
  ros__parameters:
    planner_plugins: ["GridBased"]
    GridBased:
      plugin: "nav2_navfn_planner/NavfnPlanner"
      tolerance: 0.5
      use_astar: true       # A* is faster than Dijkstra
      allow_unknown: true
```

### SmacPlanner (Hybrid A*)

Better for non-holonomic robots:

```yaml
planner_server:
  ros__parameters:
    planner_plugins: ["GridBased"]
    GridBased:
      plugin: "nav2_smac_planner/SmacPlannerHybrid"
      minimum_turning_radius: 0.40  # Based on robot geometry
      motion_model_for_search: "DUBIN"
```

## Step 5: Tune the Controller

### DWB (Dynamic Window)

Flexible but requires tuning:

```yaml
controller_server:
  ros__parameters:
    controller_plugins: ["FollowPath"]
    FollowPath:
      plugin: "dwb_core::DWBLocalPlanner"

      min_vel_x: 0.0
      max_vel_x: 0.5
      max_vel_theta: 1.0
      min_speed_xy: 0.0
      max_speed_xy: 0.5

      acc_lim_x: 2.5
      acc_lim_theta: 3.2
      decel_lim_x: -2.5

      vx_samples: 20
      vy_samples: 0          # 0 for differential drive
      vtheta_samples: 20

      critics: ["RotateToGoal", "Oscillation", "BaseObstacle", "GoalAlign", "PathAlign", "PathDist", "GoalDist"]
```

### Regulated Pure Pursuit

Simpler and often works well:

```yaml
controller_server:
  ros__parameters:
    controller_plugins: ["FollowPath"]
    FollowPath:
      plugin: "nav2_regulated_pure_pursuit_controller::RegulatedPurePursuitController"

      desired_linear_vel: 0.5
      lookahead_dist: 0.6
      min_lookahead_dist: 0.3
      max_lookahead_dist: 0.9

      rotate_to_heading_angular_vel: 1.8
      use_velocity_scaled_lookahead_dist: true
```

## Step 6: Test and Iterate

### Test Procedure

1. Start with low speeds and conservative parameters
2. Send goals in open areas first
3. Gradually increase complexity
4. Test recovery behaviors

### Common Problems and Solutions

| Problem | Possible Cause | Solution |
|---------|----------------|----------|
| Robot oscillates | Controller gains too high | Reduce velocity/acceleration limits |
| Hits obstacles | Inflation radius too small | Increase inflation radius |
| Gets stuck | Recovery behaviors not working | Check recovery server config |
| Slow planning | Resolution too high | Reduce costmap resolution |
| Path cuts corners | Lookahead too large | Reduce lookahead distance |

### Visualization

Use RViz to visualize:
- Global and local costmaps
- Planned paths
- Controller trajectories
- TF tree

```bash
ros2 launch mechano_viz view_navigation.launch.py
```

## Reference Parameters

See the full configuration files in:
- `mechano_navigation/config/nav2_params.yaml`
- `mechano_navigation/config/controller_params.yaml`
- `mechano_navigation/config/planner_params.yaml`
