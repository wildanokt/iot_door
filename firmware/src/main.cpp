#include <Arduino.h>

unsigned long heartbeatCount = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Door Voice IoT booting...");
}

void loop() {
  Serial.print("Heartbeat ");
  Serial.println(heartbeatCount);
  heartbeatCount++;
  delay(1000);
}
