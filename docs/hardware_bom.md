# Hardware Bill of Materials

This document lists all components needed to build the Mechano robot.

## Main Components

| Component | Quantity | Description | Approx. Price |
|-----------|----------|-------------|---------------|
| Compute Board | 1 | Raspberry Pi 4 (4GB+) or Jetson Nano | $55-150 |
| Microcontroller | 1 | Arduino Mega 2560 | $20 |
| Motor Driver | 1 | L298N Dual H-Bridge or BTS7960 | $10-25 |
| DC Motors | 2 | 12V DC Geared Motors with Encoders (100-200 RPM) | $30-50 |
| Wheels | 2 | 100mm diameter rubber wheels | $15 |
| Caster Wheel | 1 | Ball caster or swivel caster | $5 |
| Battery | 1 | 12V LiPo (3S) or 12V SLA | $30-50 |

## Sensors

| Component | Quantity | Description | Approx. Price |
|-----------|----------|-------------|---------------|
| Stereo Camera | 1 | OAK-D Lite or Intel RealSense D435 | $100-200 |
| IMU | 1 | BNO055 or MPU6050 | $15-30 |
| Distance Sensor | 1-4 | VL53L0X ToF sensors | $10 each |

## Electronics

| Component | Quantity | Description | Approx. Price |
|-----------|----------|-------------|---------------|
| Voltage Regulator | 1 | 12V to 5V DC-DC (5A) | $10 |
| Fuse Holder | 1 | With 10A fuse | $5 |
| Power Switch | 1 | Toggle or rocker switch (10A) | $3 |
| E-Stop Button | 1 | Emergency stop mushroom button | $5 |
| LEDs | 3-5 | Status indicator LEDs | $2 |
| Push Buttons | 2-3 | User interface buttons | $3 |
| Wires & Connectors | - | Various gauge wire, connectors | $20 |

## Mechanical

| Component | Quantity | Description | Approx. Price |
|-----------|----------|-------------|---------------|
| Chassis | 1 | Aluminum or 3D printed frame | $30-50 |
| Motor Mounts | 2 | Aluminum brackets | $10 |
| Standoffs/Spacers | - | M3 standoffs for mounting boards | $10 |
| Screws/Nuts | - | M3, M4 hardware kit | $10 |

## Optional

| Component | Quantity | Description | Approx. Price |
|-----------|----------|-------------|---------------|
| WiFi Antenna | 1 | External antenna for better range | $10 |
| Battery Charger | 1 | For LiPo or SLA battery | $20-40 |
| Display | 1 | Small OLED for status | $10 |

## Total Estimated Cost

| Configuration | Approx. Cost |
|--------------|--------------|
| Basic (RPi4 + basic sensors) | $300-400 |
| Standard (RPi4 + OAK-D) | $450-550 |
| Advanced (Jetson + full sensors) | $600-800 |

## Recommended Suppliers

- Motors & mechanical: Amazon, Pololu, ServoCity
- Electronics: Adafruit, SparkFun, DigiKey
- Cameras: Luxonis (OAK-D), Intel (RealSense)
- 3D printing: Local service or Shapeways

## Notes

- Prices are approximate and may vary by region
- Consider buying extras of small components
- Quality motors with encoders are important for accurate odometry
