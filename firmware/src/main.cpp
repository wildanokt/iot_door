#include <Arduino.h>

constexpr int DOOR_SENSOR_PIN = 27;
constexpr unsigned long DOOR_DEBOUNCE_MS = 50;

int previousRawDoorState = HIGH;
int stableDoorState = HIGH;
unsigned long lastRawChangeMs = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);

  previousRawDoorState = digitalRead(DOOR_SENSOR_PIN);
  stableDoorState = previousRawDoorState;
  lastRawChangeMs = millis();

  Serial.println("Door Voice IoT Phase 1 debounce test");
  if (stableDoorState == LOW) {
    Serial.println("Initial door state: CLOSED");
  } else {
    Serial.println("Initial door state: OPEN");
  }
}

void loop() {
  const int currentRawDoorState = digitalRead(DOOR_SENSOR_PIN);
  const unsigned long now = millis();

  if (currentRawDoorState != previousRawDoorState) {
    previousRawDoorState = currentRawDoorState;
    lastRawChangeMs = now;
  }

  if (currentRawDoorState != stableDoorState &&
      now - lastRawChangeMs >= DOOR_DEBOUNCE_MS) {
    stableDoorState = currentRawDoorState;

    if (stableDoorState == LOW) {
      Serial.println("Door CLOSED");
    } else {
      Serial.println("Door OPEN");
    }
  }
}
