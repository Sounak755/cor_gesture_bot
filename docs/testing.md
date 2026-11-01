# COR Gesture Bot — Validation Plan

Record each result as PASS, FAIL, or NOT TESTED. Do not mark a hardware test as passed until it has been performed on the actual assembled robot.

## Software and communication

- [ ] Python environment installs from `requirements.txt`.
- [ ] Webcam opens and the hand-landmarker model loads.
- [ ] Closed fist maps to `B`.
- [ ] One extended finger maps to `F`.
- [ ] Two extended fingers map to `L`.
- [ ] Three extended fingers map to `R`.
- [ ] Four extended fingers/open palm maps to `S`.
- [ ] No-hand state requests `S`.
- [ ] ESP32 access point is visible.
- [ ] Root endpoint responds at `http://192.168.4.1/`.
- [ ] Valid HTTP commands are accepted and invalid commands rejected.

## Motor tests (wheels lifted)

- [ ] Driver standby is disabled at startup; motors remain stopped until a command arrives.
- [ ] `F` drives both wheels forward.
- [ ] `B` drives both wheels backward.
- [ ] `L` turns left.
- [ ] `R` turns right.
- [ ] `S` stops both motors.
- [ ] Motor supply and driver remain within rated current/temperature limits.
- [ ] Command timeout stops the motors when heartbeats cease.

## Integrated demonstration

- [ ] Laptop remains connected to the ESP32 access point.
- [ ] Gesture commands move the robot as intended.
- [ ] Stop gesture halts the robot.
- [ ] Wi-Fi disconnection / application exit produces a safe stop through the timeout.
- [ ] Floor test completed in a clear, controlled area.

## Test record

- Date:
- Board and motor-driver model:
- Motor voltage/current rating:
- Battery specification:
- Results and observations:
