COR Gesture Bot 
A real-time hand-gesture-controlled robot powered by computer vision,
Python, and an ESP32.

COR Gesture Bot lets a user control a mobile robot using intuitive hand
gestures. A webcam captures the user's hand, a computer-vision pipeline
interprets the gesture, and the resulting movement command is
transmitted wirelessly to an ESP32. The ESP32 coordinates the robot's
movement through a dual motor driver.

The project combines computer vision, embedded systems, wireless
communication, and robotics in one end-to-end application.
Table of Contents
- Project Overview
- Key Features
- System Architecture
- Gesture Controls
- Hardware Requirements
- Software Stack
- Repository Structure
- How It Works
- Getting Started
- ESP32 Communication
- Motor Control Design
- Safety and Reliability
- Testing Checklist
- Future Enhancements
Project Overview
Traditional robot control often relies on a handheld remote, joystick,
or mobile application. COR Gesture Bot explores a more natural
interface: controlling a robot through hand gestures detected by a
regular webcam.

The computer runs Python, OpenCV, and MediaPipe to detect the hand and
map the gesture to a compact command. The ESP32 receives commands over a
local Wi-Fi network and translates them into motor-driver signals. This
modular design keeps the computer-vision workload on the laptop and the
real-time hardware interface on the microcontroller.
Key Features
- Real-time hand detection and landmark tracking using MediaPipe.
- Webcam capture and visual feedback through OpenCV.
- Five intuitive movement commands: forward, backward, left, right,
  and stop.
- Wireless laptop-to-ESP32 communication over Wi-Fi.
- Lightweight HTTP interface for movement commands.
- Dual-motor differential-drive movement design.
- Modular software and hardware architecture for debugging and
  extension.
- Designed with a stop command and communication-loss failsafe as core
  safety requirements.
System Architecture
               <img width="1752" height="897" alt="image" src="https://github.com/user-attachments/assets/ee8dc651-7531-4289-aeb8-4231857a2f1d" />

Gesture Controls
The application counts the extended fingers, ignoring the thumb, and
maps the count to a command.

  Hand gesture             Extended fingers  Command  Robot action
  Closed fist                             0    B    Backward
One finger raised                       1    F    Forward
Two fingers raised                      2    L    Turn left
Three fingers raised                    3    R    Turn right
Open palm                               4    S    Stop

No hand detected: the application sends a stop command as a safe
default.
Gesture recognition can be affected by lighting, camera angle, hand
orientation, and occlusion. Test gestures in a well-lit area with the
hand clearly visible.

Hardware Requirements
  Component                           Purpose
  ESP32 development board             Receives wireless commands and
controls motor outputs

  Laptop or computer with webcam      Runs the hand-tracking and
gesture-recognition application

  2WD robot chassis                   Mechanical platform for the robot

  Two geared DC motors and wheels     Differential-drive movement

  Caster wheel                        Supports the chassis

  Dual DC motor driver, such as       Drives the motors from ESP32
TB6612FNG                           control signals

  Suitable battery pack               Supplies motor power at the
required voltage and current

  USB cable                           Programming and development power
for the ESP32
Jumper wires, connectors, switch,   Wiring, power isolation, and
and fuse                            protection
Choose the motor driver and battery only after checking the motors'
rated voltage and stall current. A TB6612FNG is suitable only when the
motor current stays within the driver's limits.
Software Stack
  Technology                  Role
  Python                      Main application logic
OpenCV                      Webcam capture and image display
MediaPipe Hand Landmarker   Hand detection and landmark tracking
Requests                    HTTP command transmission
ESP32 Arduino framework     Wi-Fi server and motor-control firmware
Arduino IDE                 ESP32 firmware compilation and upload
Git and GitHub              Version control and project hosting
Repository Structure
cor_gesture_bot/
├── hand_tracking.py        # Webcam, hand tracking, gesture mapping, HTTP commands
├── hand_landmarker.task    # MediaPipe hand-landmarker model
├── requirements.txt        # Python dependencies
├── .gitignore              # Excludes virtual environments and local files
├── README.md               # Project documentation
└── esp32/
    └── gesture_robot.ino   # ESP32 firmware (add this sketch to the repository)
The esp32/gesture_robot.ino file is the intended location for the
Arduino firmware. Keep the firmware and Python application in sync if
the Wi-Fi settings, HTTP endpoint, command names, or motor pin
assignments change.
How It Works
1. Capture: OpenCV reads frames from the computer's webcam.
2. Detect: MediaPipe identifies the hand and estimates its
   landmarks.
3. Classify: The application counts extended fingers and assigns a
   gesture.
4. Map: The gesture is converted into a movement command (F, B,
   L, R, or S).
5. Transmit: Python sends the command to the ESP32 over the local
   Wi-Fi network using an HTTP request.
6. Interpret: The ESP32 validates the command and selects the
   required motor direction.
7. Drive: The motor driver switches the left and right DC motors to
   create the requested movement.
Getting Started
Prerequisites
- Git installed on the computer.
- Python 3.11 recommended for the current development environment.
- Webcam access.
- Arduino IDE with ESP32 board support.
- An ESP32 development board and USB cable.
- For physical movement: assembled chassis, compatible motor driver,
  motors, and a correctly rated battery.
1. Clone the repository
git clone https://github.com/Sounak755/cor_gesture_bot.git
cd cor_gesture_bot
2. Create a virtual environment
py -3.11 -m venv venv
.\venv\Scripts\Activate.ps1
If PowerShell blocks environment activation, use a permitted activation
method for your system or run the environment's Python executable
directly.
3. Install Python dependencies
python -m pip install --upgrade pip
pip install -r requirements.txt
4. Configure the ESP32
Flash the ESP32 with the project's Arduino firmware. The development
network settings currently used by the Python application and initial
ESP32 server are:

  Setting                      Value
  Wi-Fi SSID                   GestureRobot
Wi-Fi password               robot123
ESP32 access-point address   192.168.4.1
HTTP command endpoint        /cmd
Command parameter            cmd

These are development defaults, not production security settings. Change
the Wi-Fi credentials before public deployment and keep the firmware and
Python endpoint configuration consistent.
5. Connect to the ESP32
Connect the computer to the GestureRobot Wi-Fi network. Since the
ESP32 provides the local access point, internet access may not be
available while connected.

Test the server in a browser:
http://192.168.4.1/
A command can be tested with:
http://192.168.4.1/cmd?cmd=F
The initial communication test server validates and logs commands to the
Arduino Serial Monitor. Physical movement requires the motor-control
firmware and hardware to be connected and tested.
6. Run the Python application
python hand_tracking.py
Allow webcam access if prompted. Present the supported gestures in front
of the camera and confirm the recognized commands in the application and
ESP32 Serial Monitor.
ESP32 Communication
The laptop sends HTTP GET requests to the ESP32. The command interface
uses the following values:

  Command   Meaning
  F       Forward
B       Backward
L       Left
R       Right
S       Stop

Example:
GET http://192.168.4.1/cmd?cmd=F
The ESP32 should validate the command before acting on it. Unknown or
missing commands should be rejected rather than passed to the
motor-control logic.
Motor Control Design
For a two-wheel differential-drive robot, the direction of each motor
determines the robot's movement. The final motor firmware should
implement the following behavior:

  Command   Left motor        Right motor
  F       Forward           Forward
B       Reverse           Reverse
L       Reverse or stop   Forward
R       Forward           Reverse or stop
S       Stop              Stop

The exact turning behavior can be tuned for the chassis. For example,
counter-rotating the wheels enables a tighter pivot turn, while stopping
one wheel produces a gentler turn. PWM can be added to adjust speed.
Safety and Reliability
- Stop on lost hand: the application should request S when no
  hand is detected.
- Communication timeout: the ESP32 should stop both motors if no
  valid command arrives within a short timeout.
- Startup state: initialize motor outputs to stop before accepting
  movement commands.
- Power separation: do not power motors directly from ESP32 GPIO
  pins. Use a suitable motor driver and a correctly rated motor
  supply.
- Common ground: connect ESP32 ground and motor-driver logic
  ground appropriately, following the driver's wiring requirements.
- Battery protection: verify polarity, voltage, current rating,
  and short-circuit protection.
- Safe testing: raise the wheels off the ground for the first
  motor tests and keep a physical power switch accessible.
Testing Checklist
Use this checklist during integration and demonstration:
- [ ] Webcam opens and displays a live image.
- [ ] Each of the five gestures is recognized consistently.
- [ ] The laptop connects to the ESP32 Wi-Fi access point.
- [ ] Each command reaches the ESP32 and is logged correctly.
- [ ] Motor-driver direction and enable pins are verified.
- [ ] Forward and backward movement work as expected.
- [ ] Left and right turns work as expected.
- [ ] Stop command halts both motors.
- [ ] Communication timeout stops the robot if Wi-Fi or command
  transmission is interrupted.
- [ ] Full system is tested on the floor in a clear, controlled area.
Future Enhancements
- Adjustable speed control using PWM.
- Gesture calibration and improved robustness under varying lighting.
- On-screen gesture and connection-status indicators.
- Configurable Wi-Fi and motor-pin settings.
- Battery-level monitoring and low-voltage warning.
- Optional obstacle detection and emergency-stop input.
Project Goal
COR Gesture Bot demonstrates how computer vision, embedded Wi-Fi
communication, and motor control can be combined to create an intuitive
human--robot interface. Its modular design makes it suitable for a
robotics demonstration and further development in embedded systems,
computer vision, and mobile robotics.
Author
Sounak Ghosh

GitHub repository:
Sounak755/cor_gesture_bot
