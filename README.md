#  COR Gesture Bot

**A Hand-Gesture-Controlled Robot Using Computer Vision and ESP32**

COR Gesture Bot is a gesture-controlled robotic vehicle that allows users to control robot movement using simple hand gestures. The system uses a laptop webcam, Python, OpenCV, and MediaPipe to recognize gestures and sends movement commands wirelessly to an ESP32 over Wi-Fi.

The ESP32 receives commands and interfaces with a motor driver to control the robot's movement.

##  Features

- Real-time hand tracking and gesture recognition
- Webcam-based control using OpenCV and MediaPipe
- Five gesture commands for robot navigation
- Wireless communication between laptop and ESP32
- HTTP-based command transmission over Wi-Fi
- ESP32-based embedded control architecture
- Designed for two-wheel differential-drive movement

##  Gesture Controls

| Hand Gesture | Command | Action |
|---|---|---|
| Closed fist | `B` | Move backward |
| One finger | `F` | Move forward |
| Two fingers | `L` | Turn left |
| Three fingers | `R` | Turn right |
| Open palm | `S` | Stop |

When no hand is detected, the application attempts to send the stop command.

##  System Architecture

<img width="1752" height="897" alt="image" src="https://github.com/user-attachments/assets/87c2eac4-9b6d-41e2-9036-acb76c2b10f2" />


The system follows a modular architecture with computer vision running on the laptop and wireless command handling on the ESP32.

**Processing pipeline:**

1. **Image Capture:** The laptop webcam captures live video.
2. **Hand Detection:** MediaPipe identifies hand landmarks.
3. **Gesture Recognition:** Python processes the landmarks and identifies the gesture.
4. **Command Mapping:** The recognized gesture is converted into a movement command.
5. **Wireless Transmission:** An HTTP request sends the command over Wi-Fi.
6. **Embedded Control:** The ESP32 receives and validates the command.
7. **Robot Movement:** The motor driver controls the left and right motors according to the command.

##  Hardware Requirements

- ESP32 development board
- Laptop or computer with webcam
- Two-wheel robot chassis
- Two geared DC motors and wheels
- Caster wheel
- Dual motor driver (TB6612FNG or another compatible driver)
- Suitable battery pack
- USB cable, jumper wires, connectors, and power switch

The motor driver and battery must be selected according to the motors' voltage and current requirements.

##  Software and Technologies

| Technology | Purpose |
|---|---|
| Python | Application logic |
| OpenCV | Webcam capture and image processing |
| MediaPipe | Hand landmark detection |
| Requests | HTTP communication |
| ESP32 Arduino framework | Embedded Wi-Fi and control firmware |
| Arduino IDE | ESP32 programming |
| Git and GitHub | Version control |

##  Project Structure

```text
cor_gesture_bot/
├── hand_tracking.py
├── hand_landmarker.task
├── requirements.txt
├── .gitignore
└── README.md
```

- `hand_tracking.py` — webcam processing, gesture recognition, and command transmission.
- `hand_landmarker.task` — MediaPipe hand-landmark model.
- `requirements.txt` — Python dependencies.
- `.gitignore` — excludes virtual environments and local development files.

##  Installation and Setup

### 1. Clone the repository

```bash
git clone https://github.com/Sounak755/cor_gesture_bot.git
cd cor_gesture_bot
```

### 2. Create a Python virtual environment

On Windows:

```powershell
py -3.11 -m venv venv
.\venv\Scripts\Activate.ps1
```

### 3. Install dependencies

```bash
python -m pip install --upgrade pip
pip install -r requirements.txt
```

### 4. Connect to the ESP32

The current development configuration uses the following Wi-Fi settings:

- **SSID:** `GestureRobot`
- **Password:** `robot123`
- **ESP32 IP:** `192.168.4.1`
- **Command endpoint:** `/cmd`

The laptop must be connected to the ESP32 access point for local command transmission. The ESP32 must be running compatible Wi-Fi server firmware.

### 5. Run the application

```bash
python hand_tracking.py
```

Allow webcam access if prompted, then show the supported gestures to the camera.

##  Communication Protocol

The laptop sends HTTP GET requests to the ESP32 using the following commands:

| Command | Meaning |
|---|---|
| `F` | Forward |
| `B` | Backward |
| `L` | Left |
| `R` | Right |
| `S` | Stop |

Example request:

```text
http://192.168.4.1/cmd?cmd=F
```

The ESP32 validates incoming commands before processing them. Motor movement requires compatible motor-control firmware and correctly connected hardware.

##  Motor Control Concept

The robot uses a differential-drive configuration with independently controlled left and right motors.

| Command | Left Motor | Right Motor |
|---|---|---|
| Forward | Forward | Forward |
| Backward | Reverse | Reverse |
| Left | Reverse or stop | Forward |
| Right | Forward | Reverse or stop |
| Stop | Stop | Stop |

The exact turning behavior can be adjusted according to the chassis and motor-driver configuration.

##  Safety Considerations

- Motors must be powered through a suitable motor driver, never directly from ESP32 GPIO pins.
- The battery voltage and current rating must match the motors and driver.
- Motor outputs should default to stop at startup.
- A communication timeout should stop the motors if commands are interrupted.
- Initial motor tests should be performed with the wheels lifted off the ground.
- Keep a physical power switch accessible during testing.

##  Validation Checklist

- [ ] Real-time webcam capture
- [ ] Recognition of all five gestures
- [ ] Wi-Fi connection between laptop and ESP32
- [ ] HTTP command transmission
- [ ] ESP32 command validation
- [ ] Motor-driver integration
- [ ] Forward and backward movement
- [ ] Left and right turning
- [ ] Stop command and communication-loss failsafe
- [ ] Full-system testing

##  Future Improvements

- PWM-based speed control
- Improved gesture classification under different lighting conditions
- On-screen connection and command status
- Obstacle detection
- Battery monitoring
- Emergency-stop input

##  Project Objective

The objective of COR Gesture Bot is to demonstrate an intuitive human–robot interface by combining computer vision, wireless communication, embedded systems, and mobile robotics.

The modular architecture provides a foundation for further development in robotics, embedded control, and computer vision.

##  Author

**Sounak Ghosh**

GitHub Repository: [Sounak755/cor_gesture_bot](https://github.com/Sounak755/cor_gesture_bot)
