# Hardware Assembly Guide

This guide walks through assembling the Mechano robot.

## Prerequisites

- All components from the [Bill of Materials](hardware_bom.md)
- Basic tools: screwdrivers, soldering iron, wire strippers
- Multimeter for testing

## Safety First

- Disconnect power when making connections
- Double-check polarity before connecting batteries
- Use appropriate wire gauges for current requirements

## Assembly Steps

### Step 1: Prepare the Chassis

1. If 3D printing, print all chassis parts
2. Clean up any print artifacts
3. Test fit all components before final assembly

### Step 2: Mount the Motors

1. Attach motor mounts to the chassis
2. Secure motors to the mounts
3. Ensure wheels spin freely without rubbing

### Step 3: Install the Caster Wheel

1. Mount the caster at the front (or rear) of the chassis
2. Adjust height so the robot sits level
3. Ensure smooth rotation

### Step 4: Wire the Power System

```
Battery (+) ─── Fuse ─── E-Stop ─── Power Switch ─┬─── Motor Driver (12V)
                                                   │
                                                   └─── DC-DC Converter ─── 5V Rail

Battery (-) ─────────────────────────────────────────── Common Ground
```

1. Install the fuse holder near the battery
2. Wire the E-Stop in series with the main power
3. Connect the DC-DC converter for 5V supply
4. Test voltage outputs before connecting anything else

### Step 5: Install the Motor Driver

1. Mount the motor driver on the chassis
2. Connect motor power from the 12V rail
3. Connect motor outputs to the DC motors
4. Connect control pins to the microcontroller:
   - PWM pins for speed control
   - Direction pins for motor direction

### Step 6: Set Up the Microcontroller

1. Mount the Arduino on standoffs
2. Connect to motor driver control pins
3. Connect encoder signals from motors:
   - Channel A and B for each motor
   - Use interrupt-capable pins
4. Connect I2C for IMU (SDA, SCL)
5. Connect serial to the main computer

### Step 7: Mount the Compute Board

1. Install standoffs for the Raspberry Pi or Jetson
2. Mount the board securely
3. Connect power from the 5V rail
4. Connect USB to the microcontroller
5. Connect USB to the camera

### Step 8: Install Sensors

#### IMU
1. Mount the IMU firmly to the chassis
2. Align with robot coordinate frame (X forward, Y left, Z up)
3. Connect I2C to the microcontroller

#### Camera
1. Mount the stereo camera facing forward
2. Ensure stable mounting to prevent vibration
3. Connect USB to the compute board

#### Distance Sensors (optional)
1. Mount sensors at desired locations
2. Connect I2C to the microcontroller

### Step 9: Final Wiring

1. Organize all wires with cable ties
2. Ensure no wires can contact moving parts
3. Label connections for easier maintenance

### Step 10: Testing

1. Double-check all connections
2. Power on with E-Stop ready
3. Check voltages at all power rails
4. Verify motor direction
5. Test encoder feedback
6. Run the hardware check script:
   ```bash
   ./scripts/check_hardware.bash
   ```

## Wiring Diagram

See `docs/images/wiring_diagram.png` for the complete wiring schematic.

## Coordinate Frame

The robot uses the REP-105 coordinate convention:
- **X**: Forward
- **Y**: Left
- **Z**: Up

Ensure all sensors are mounted aligned with this convention.

## Next Steps

After assembly:
1. [Install software](../ROBOT_INSTALLATION.md)
2. [Calibrate sensors](../scripts/calibrate_imu.bash)
3. [Test the robot](../README.md#running)
