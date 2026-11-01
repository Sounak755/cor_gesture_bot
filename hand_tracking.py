
import cv2
import mediapipe as mp
import requests
import time

from mediapipe.tasks import python
from mediapipe.tasks.python import vision


# -----------------------------------
# ESP32 Wi-Fi setup
# -----------------------------------

ESP32_URL = "http://192.168.4.1/cmd"
REQUEST_TIMEOUT = 0.3

session = requests.Session()
last_sent_command = None
last_send_time = 0
MIN_SEND_INTERVAL = 0.15


def send_command(command):
    """Send a command to the ESP32 over Wi-Fi."""
    global last_sent_command, last_send_time

    now = time.monotonic()

    # Avoid repeatedly sending the same command
    if command == last_sent_command:
        return

    # Small delay between command changes
    if now - last_send_time < MIN_SEND_INTERVAL:
        return

    try:
        response = session.get(
            ESP32_URL,
            params={"cmd": command},
            timeout=REQUEST_TIMEOUT
        )

        if response.ok:
            print(f"Sent {command}: {response.text}")
            last_sent_command = command
            last_send_time = now
        else:
            print(f"ESP32 returned HTTP {response.status_code}")

    except requests.RequestException as error:
        print(f"ESP32 Wi-Fi connection error: {error}")


# -----------------------------------
# MediaPipe setup
# -----------------------------------

BaseOptions = python.BaseOptions
HandLandmarker = vision.HandLandmarker
HandLandmarkerOptions = vision.HandLandmarkerOptions
VisionRunningMode = vision.RunningMode

options = HandLandmarkerOptions(
    base_options=BaseOptions(
        model_asset_path="hand_landmarker.task"
    ),
    running_mode=VisionRunningMode.IMAGE,
    num_hands=1
)

landmarker = HandLandmarker.create_from_options(options)


# -----------------------------------
# Gesture detection
# -----------------------------------

def get_gesture(hand):
    index_tip = hand[8]
    index_pip = hand[6]

    middle_tip = hand[12]
    middle_pip = hand[10]

    ring_tip = hand[16]
    ring_pip = hand[14]

    pinky_tip = hand[20]
    pinky_pip = hand[18]

    index_extended = index_tip.y < index_pip.y
    middle_extended = middle_tip.y < middle_pip.y
    ring_extended = ring_tip.y < ring_pip.y
    pinky_extended = pinky_tip.y < pinky_pip.y

    count = sum([
        index_extended,
        middle_extended,
        ring_extended,
        pinky_extended
    ])

    if count == 0:
        return "BACKWARD", "B"
    elif count == 1:
        return "FORWARD", "F"
    elif count == 2:
        return "LEFT", "L"
    elif count == 3:
        return "RIGHT", "R"
    elif count == 4:
        return "STOP", "S"

    return "UNKNOWN", "S"


# -----------------------------------
# Camera
# -----------------------------------

cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Could not access camera")
    landmarker.close()
    raise SystemExit


# -----------------------------------
# Main loop
# -----------------------------------

try:
    while True:
        ret, frame = cap.read()

        if not ret:
            print("Could not read camera")
            break

        frame = cv2.flip(frame, 1)

        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        mp_image = mp.Image(
            image_format=mp.ImageFormat.SRGB,
            data=rgb
        )

        result = landmarker.detect(mp_image)

        gesture = "NO HAND"
        command = "S"

        if result.hand_landmarks:
            hand = result.hand_landmarks[0]
            gesture, command = get_gesture(hand)

            h, w, _ = frame.shape

            for landmark in hand:
                x = int(landmark.x * w)
                y = int(landmark.y * h)

                cv2.circle(
                    frame,
                    (x, y),
                    5,
                    (0, 255, 0),
                    -1
                )

        # Send a command only when it changes.
        # No hand detected means STOP.
        send_command(command)

        cv2.putText(
            frame,
            f"Gesture: {gesture}",
            (20, 40),
            cv2.FONT_HERSHEY_SIMPLEX,
            1,
            (0, 255, 0),
            2
        )

        cv2.putText(
            frame,
            f"Command: {command}",
            (20, 80),
            cv2.FONT_HERSHEY_SIMPLEX,
            1,
            (0, 255, 0),
            2
        )

        cv2.imshow("Gesture Robot", frame)

        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

finally:
    # Try to stop the robot when exiting
    try:
        session.get(
            ESP32_URL,
            params={"cmd": "S"},
            timeout=1
        )
    except requests.RequestException:
        pass

    session.close()
    cap.release()
    cv2.destroyAllWindows()
    landmarker.close()
