#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

constexpr int DFPLAYER_RX_PIN = 16;
constexpr int DFPLAYER_TX_PIN = 17;

constexpr int AUDIO_TRACK = 1;
constexpr int AUDIO_VOLUME = 12;

HardwareSerial dfPlayerSerial(2);
DFRobotDFPlayerMini audioPlayer;

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
  Serial.println("Door Voice IoT Phase 2 communication test");
  Serial.println("Speaker is intentionally disconnected");

  if (!initAudioPlayer()) return;

  setAudioVolume(AUDIO_VOLUME);
  delay(300);

  const int fileCount = audioPlayer.readFileCounts();
  if (fileCount >= 0) {
    Serial.print("microSD file count: ");
    Serial.println(fileCount);
  } else {
    Serial.println("WARNING: microSD file-count query failed");
  }

  playTrack(AUDIO_TRACK);
  Serial.println("Communication test commands complete");
  Serial.println("Audio playback is NOT validated without a speaker");
}

void loop() {
  if (audioPlayer.available()) {
    const uint8_t type = audioPlayer.readType();
    const int value = audioPlayer.read();
    printAudioEvent(type, value);
  }
}
