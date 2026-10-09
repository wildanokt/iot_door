#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

constexpr int DOOR_SENSOR_PIN = 27;
constexpr int DFPLAYER_RX_PIN = 16;
constexpr int DFPLAYER_TX_PIN = 17;

constexpr int AUDIO_TRACK = 1;
constexpr int AUDIO_VOLUME = 16;
constexpr unsigned long DOOR_DEBOUNCE_MS = 50;
constexpr unsigned long TRIGGER_COOLDOWN_MS = 1000;

HardwareSerial dfPlayerSerial(2);
DFRobotDFPlayerMini audioPlayer;

bool audioReady = false;
bool doorReady = false;
bool armed = false;
bool hasPlayed = false;
int previousRawDoorState = HIGH;
int stableDoorState = HIGH;
unsigned long lastRawChangeMs = 0;
unsigned long lastTriggerMs = 0;

bool initDoorSensor() {
  previousRawDoorState = digitalRead(DOOR_SENSOR_PIN);
  lastRawChangeMs = millis();
  const unsigned long startMs = lastRawChangeMs;

  // Require a stable starting reading; it is not an opening event.
  while (millis() - startMs < 2000) {
    const int rawState = digitalRead(DOOR_SENSOR_PIN);
    const unsigned long now = millis();
    if (rawState != previousRawDoorState) {
      previousRawDoorState = rawState;
      lastRawChangeMs = now;
    }
    if (now - lastRawChangeMs >= DOOR_DEBOUNCE_MS) {
      stableDoorState = rawState;
      armed = stableDoorState == LOW;
      Serial.print("Initial door state: ");
      Serial.println(armed ? "CLOSED" : "OPEN");
      return true;
    }
    delay(1); // Only during initial sensor validation.
  }

  Serial.println("ERROR: door input unstable; check GPIO27/GND wiring and reset");
  return false;
}

bool initAudioPlayer() {
  Serial.println("Initializing DFPlayer...");
  Serial.println("UART2: RX=GPIO16, TX=GPIO17, baud=9600");

  dfPlayerSerial.begin(9600, SERIAL_8N1, DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);
  delay(1000);

  if (!audioPlayer.begin(dfPlayerSerial, true, true)) {
    Serial.println("ERROR: DFPlayer initialization failed");
    Serial.println("Check 5V/VCC, common GND, crossed TX/RX, and microSD");
    return false;
  }

  audioPlayer.setTimeOut(1000);
  Serial.println("DFPlayer initialized");
  return true;
}

void setAudioVolume(uint8_t volume) {
  audioPlayer.volume(volume);
  Serial.print("Volume command: ");
  Serial.println(volume);
}

void playTrack(uint16_t track) {
  Serial.print("Playing track ");
  Serial.println(track);
  audioPlayer.playMp3Folder(track);
  Serial.print("Play command sent for mp3/");
  if (track < 1000) Serial.print('0');
  if (track < 100) Serial.print('0');
  if (track < 10) Serial.print('0');
  Serial.print(track);
  Serial.println(".mp3");
}

void stopAudio() {
  audioPlayer.stop();
  Serial.println("Stop command sent");
}

void printAudioEvent(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println("DFPlayer event: timeout");
      break;
    case WrongStack:
      Serial.println("DFPlayer event: invalid response");
      break;
    case DFPlayerCardInserted:
      Serial.println("DFPlayer event: microSD inserted");
      break;
    case DFPlayerCardRemoved:
      Serial.println("DFPlayer event: microSD removed");
      break;
    case DFPlayerCardOnline:
      Serial.println("DFPlayer event: microSD online");
      break;
    case DFPlayerPlayFinished:
      Serial.print("DFPlayer event: track finished, number ");
      Serial.println(value);
      break;
    case DFPlayerError:
      Serial.print("DFPlayer event: error code ");
      Serial.println(value);
      break;
    default:
      Serial.print("DFPlayer event type ");
      Serial.print(type);
      Serial.print(", value ");
      Serial.println(value);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Door Voice IoT Phase 3 door trigger");
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);

  audioReady = initAudioPlayer();
  if (audioReady) {
    // Initialization still requires a real reply. Later commands do not
    // wait for ACKs, so a missing ACK cannot stall sensor servicing.
    audioPlayer.begin(dfPlayerSerial, false, false);
    setAudioVolume(AUDIO_VOLUME);
    stopAudio();
  }

  doorReady = initDoorSensor();
  if (audioReady && doorReady) {
    Serial.println("System ready");
  } else {
    Serial.println("System NOT ready; see errors above");
  }
}

void loop() {
  if (doorReady) {
    const int rawState = digitalRead(DOOR_SENSOR_PIN);
    const unsigned long now = millis();

    if (rawState != previousRawDoorState) {
      previousRawDoorState = rawState;
      lastRawChangeMs = now;
    }

    if (rawState != stableDoorState &&
        now - lastRawChangeMs >= DOOR_DEBOUNCE_MS) {
      stableDoorState = rawState;

      if (stableDoorState == LOW) {
        Serial.println("Door CLOSED");
        armed = true;
        Serial.println("Trigger re-armed");
      } else {
        Serial.println("Door OPEN");
        const bool wasArmed = armed;
        armed = false;

        if (wasArmed) {
          if (!audioReady) {
            Serial.println("Trigger skipped: DFPlayer unavailable");
          } else if (hasPlayed && now - lastTriggerMs < TRIGGER_COOLDOWN_MS) {
            Serial.println("Trigger skipped: 1000 ms cooldown");
          } else {
            lastTriggerMs = now;
            hasPlayed = true;
            playTrack(AUDIO_TRACK);
          }
        }
      }
    }
  }

  if (audioReady && audioPlayer.available()) {
    const uint8_t type = audioPlayer.readType();
    const int value = audioPlayer.read();
    printAudioEvent(type, value);
  }
}
