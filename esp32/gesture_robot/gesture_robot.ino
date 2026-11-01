/*
  COR Gesture Bot - ESP32 motor-control firmware
  Target: classic ESP32 Dev Module / ESP32-WROOM
  Driver: TB6612FNG dual DC motor driver
  Network: ESP32 SoftAP + HTTP command server

  IMPORTANT:
  - Confirm pin mapping and motor-driver voltage/current ratings before wiring.
  - This sketch targets Arduino-ESP32 core 2.x (e.g. 2.0.17) and uses ledcSetup/ledcAttachPin.
  - A command timeout stops the motors. The Python controller must periodically resend
    the active command (heartbeat) while a gesture remains active.
*/

#include <WiFi.h>
#include <WebServer.h>

const char* WIFI_SSID = "GestureRobot";
const char* WIFI_PASSWORD = "robot123";

WebServer server(80);

// TB6612FNG pin mapping — verify against your wiring.
constexpr uint8_t PIN_PWMA = 25;
constexpr uint8_t PIN_AIN1 = 26;
constexpr uint8_t PIN_AIN2 = 27;

constexpr uint8_t PIN_PWMB = 14;
constexpr uint8_t PIN_BIN1 = 32;
constexpr uint8_t PIN_BIN2 = 33;

constexpr uint8_t PIN_STBY = 13;

// PWM setup for Arduino-ESP32 core 2.x.
constexpr uint8_t PWM_CHANNEL_A = 0;
constexpr uint8_t PWM_CHANNEL_B = 1;
constexpr uint32_t PWM_FREQUENCY = 15000;
constexpr uint8_t PWM_RESOLUTION_BITS = 8; // duty: 0..255

// Start conservatively; tune after testing with wheels lifted.
constexpr uint8_t MOTOR_SPEED = 170;
constexpr unsigned long COMMAND_TIMEOUT_MS = 2000;

unsigned long lastValidCommandMs = 0;
char currentCommand = 'S';

void setMotorA(int direction, uint8_t speedValue) {
  // direction: 1 forward, -1 reverse, 0 stop/coast
  if (direction > 0) {
    digitalWrite(PIN_AIN1, HIGH);
    digitalWrite(PIN_AIN2, LOW);
  } else if (direction < 0) {
    digitalWrite(PIN_AIN1, LOW);
    digitalWrite(PIN_AIN2, HIGH);
  } else {
    digitalWrite(PIN_AIN1, LOW);
    digitalWrite(PIN_AIN2, LOW);
    speedValue = 0;
  }
  ledcWrite(PWM_CHANNEL_A, speedValue);
}

void setMotorB(int direction, uint8_t speedValue) {
  if (direction > 0) {
    digitalWrite(PIN_BIN1, HIGH);
    digitalWrite(PIN_BIN2, LOW);
  } else if (direction < 0) {
    digitalWrite(PIN_BIN1, LOW);
    digitalWrite(PIN_BIN2, HIGH);
  } else {
    digitalWrite(PIN_BIN1, LOW);
    digitalWrite(PIN_BIN2, LOW);
    speedValue = 0;
  }
  ledcWrite(PWM_CHANNEL_B, speedValue);
}

void stopMotors() {
  setMotorA(0, 0);
  setMotorB(0, 0);
  currentCommand = 'S';
}

void applyCommand(char command) {
  switch (command) {
    case 'F':
      setMotorA(1, MOTOR_SPEED);
      setMotorB(1, MOTOR_SPEED);
      break;
    case 'B':
      setMotorA(-1, MOTOR_SPEED);
      setMotorB(-1, MOTOR_SPEED);
      break;
    case 'L':
      // Pivot left. Swap motor direction if the physical motor orientation differs.
      setMotorA(-1, MOTOR_SPEED);
      setMotorB(1, MOTOR_SPEED);
      break;
    case 'R':
      // Pivot right.
      setMotorA(1, MOTOR_SPEED);
      setMotorB(-1, MOTOR_SPEED);
      break;
    case 'S':
    default:
      stopMotors();
      return;
  }
  currentCommand = command;
}

bool isValidCommand(const String& value) {
  return value.length() == 1 &&
         (value[0] == 'F' || value[0] == 'B' ||
          value[0] == 'L' || value[0] == 'R' ||
          value[0] == 'S');
}

void handleRoot() {
  server.send(200, "text/plain",
              "COR Gesture Bot ESP32 is online. Use /cmd?cmd=F|B|L|R|S");
}

void handleCommand() {
  if (!server.hasArg("cmd")) {
    server.send(400, "text/plain", "Missing cmd parameter");
    return;
  }

  String value = server.arg("cmd");
  value.trim();
  if (!isValidCommand(value)) {
    server.send(400, "text/plain", "Invalid command. Use F, B, L, R, or S.");
    return;
  }

  const char command = value[0];
  lastValidCommandMs = millis();

  if (command == 'S') {
    stopMotors();
  } else {
    applyCommand(command);
  }

  Serial.print("Received command: ");
  Serial.println(command);
  server.send(200, "text/plain", String("OK: ") + command);
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_AIN1, OUTPUT);
  pinMode(PIN_AIN2, OUTPUT);
  pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_BIN2, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);

  digitalWrite(PIN_STBY, LOW);
  ledcSetup(PWM_CHANNEL_A, PWM_FREQUENCY, PWM_RESOLUTION_BITS);
  ledcSetup(PWM_CHANNEL_B, PWM_FREQUENCY, PWM_RESOLUTION_BITS);
  ledcAttachPin(PIN_PWMA, PWM_CHANNEL_A);
  ledcAttachPin(PIN_PWMB, PWM_CHANNEL_B);
  stopMotors();

  WiFi.mode(WIFI_AP);
  if (WiFi.softAP(WIFI_SSID, WIFI_PASSWORD)) {
    Serial.println("Wi-Fi access point started.");
    Serial.print("SSID: ");
    Serial.println(WIFI_SSID);
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Failed to start Wi-Fi access point.");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/cmd", HTTP_GET, handleCommand);
  server.begin();

  lastValidCommandMs = millis();
  Serial.println("COR Gesture Bot command server ready.");
}

void loop() {
  server.handleClient();

  if (currentCommand != 'S' &&
      millis() - lastValidCommandMs > COMMAND_TIMEOUT_MS) {
    Serial.println("Command timeout: stopping motors.");
    stopMotors();
  }
}
