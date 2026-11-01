# Integration Notes

## Firmware assumptions

The supplied sketch targets a classic ESP32 development board, Arduino-ESP32 core 2.x, and TB6612FNG driver. The selected GPIO assignments are examples that must be checked against the actual board and wiring.

## Important heartbeat requirement

The ESP32 firmware stops the motors if it receives no valid command for 2 seconds. The existing Python application was previously configured to send a command only when the recognized command changes. **That behavior must be updated to periodically resend the active command while a gesture remains held**, otherwise the timeout will stop the robot after two seconds even if the user keeps holding the same gesture.

The no-hand state and application exit should send `S` immediately. For safe operation, the ESP32 timeout remains a separate fallback.

## Before the physical test

Confirm the actual motor driver model. If the supplied module is not TB6612FNG, use the correct pin mapping and driver-specific wiring. Confirm motor voltage and stall current before choosing the motor supply. Test with wheels raised first.
