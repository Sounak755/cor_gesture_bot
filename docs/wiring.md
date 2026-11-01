# COR Gesture Bot — Wiring Guide

This guide assumes a **classic ESP32-WROOM development board** and a **TB6612FNG dual motor driver**. Confirm your actual board and driver labels before wiring. If your driver is an L298N or a different module, do not use this pin table unchanged.

## ESP32 to TB6612FNG

| TB6612FNG pin | ESP32 pin | Purpose |
|---|---:|---|
| PWMA | GPIO 25 | Left motor PWM/speed |
| AIN1 | GPIO 26 | Left motor direction |
| AIN2 | GPIO 27 | Left motor direction |
| PWMB | GPIO 14 | Right motor PWM/speed |
| BIN1 | GPIO 32 | Right motor direction |
| BIN2 | GPIO 33 | Right motor direction |
| STBY | GPIO 13 | Driver standby enable |
| VCC | 3V3 | Driver logic supply |
| GND | GND | Common logic ground |

## Motors and power

- Connect the left motor to `A01` and `A02` (or the equivalent Motor A output terminals).
- Connect the right motor to `B01` and `B02` (or the equivalent Motor B output terminals).
- Connect the battery positive to the driver's motor supply input (`VM`); connect battery negative to driver `GND`.
- Connect ESP32 `GND`, driver `GND`, and battery negative together as required by the module's wiring design.
- Keep motor current out of the ESP32 3V3 pin and GPIO pins.
- Do not feed the motor battery voltage into the ESP32 3V3 pin. Power the ESP32 via USB during initial tests or use a suitable regulated supply.
- Use a battery with voltage appropriate for the motors and driver. Check the motor stall current against the driver's per-channel and total current limits.
- Add a suitable fuse and accessible power switch in the motor supply path.

## Direction calibration

Motor wires and mounting orientation vary. If the robot moves backward on `F`, swap that motor's output wires or invert that motor's direction in firmware. If left and right are reversed, swap the Motor A/B assignments or adjust the command logic.

## First power-up checklist

1. Keep the wheels lifted off the ground.
2. Check for shorts and confirm all power polarity.
3. Power the ESP32 over USB and the motors from their correctly rated supply.
4. Connect to Wi-Fi `GestureRobot` (development password: `robot123`).
5. Open `http://192.168.4.1/` to check the server.
6. Test `S` first, then brief `F`, `B`, `L`, and `R` commands.
7. Verify the timeout stops the motors if command heartbeats stop.

Never connect DC motors directly to ESP32 GPIO pins.
