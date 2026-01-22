# mechano_description

URDF/Xacro robot model for the Mechano differential drive robot.

## Contents

- `urdf/` - Robot model files (xacro format)
- `meshes/` - 3D mesh files (STL/DAE)
- `rviz/` - RViz configuration files
- `launch/` - Launch files
- `config/` - Configuration files

## URDF Structure

The robot model is split into multiple xacro files for modularity:

| File | Description |
|------|-------------|
| `mechano.urdf.xacro` | Main file that includes all others |
| `common_properties.xacro` | Dimensions, masses, inertia macros |
| `materials.xacro` | Color definitions |
| `base.xacro` | Base link and caster |
| `wheels.xacro` | Wheel macro and joints |
| `sensors.xacro` | IMU, camera, distance sensor |
| `ros2_control.xacro` | Hardware interface configuration |
| `gazebo.xacro` | Gazebo plugins (simulation only) |

## Coordinate Frame

The robot follows REP-105:
- **X**: Forward
- **Y**: Left
- **Z**: Up

TF tree:
```
base_footprint
    └── base_link
        ├── left_wheel
        ├── right_wheel
        ├── caster_link
        ├── imu_link
        ├── camera_link
        │   ├── camera_depth_frame
        │   └── camera_color_frame
        └── distance_sensor_link
```

## Usage

### View in RViz (with joint_state_publisher_gui)

```bash
ros2 launch mechano_description description.launch.py use_rviz:=true use_jsp_gui:=true
```

### Include in another launch file

```python
from launch_ros.actions import Node
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

robot_description = Command([
    'xacro ',
    PathJoinSubstitution([
        FindPackageShare('mechano_description'),
        'urdf',
        'mechano.urdf.xacro'
    ]),
    ' use_sim:=false',
])

robot_state_publisher = Node(
    package='robot_state_publisher',
    executable='robot_state_publisher',
    parameters=[{'robot_description': robot_description}]
)
```

## Customization

### Changing robot dimensions

Edit `urdf/common_properties.xacro`:

```xml
<xacro:property name="base_length" value="0.30"/>
<xacro:property name="base_width" value="0.25"/>
<xacro:property name="wheel_radius" value="0.05"/>
<xacro:property name="wheel_separation" value="0.30"/>
```

### Adding custom meshes

1. Place STL files in `meshes/`
2. Reference in xacro:

```xml
<visual>
  <geometry>
    <mesh filename="package://mechano_description/meshes/base_link.stl" scale="0.001 0.001 0.001"/>
  </geometry>
</visual>
```

### Adding sensors

Edit `urdf/sensors.xacro` and add a new macro following the existing patterns.
