# Door Voice IoT implementation plan

Work must proceed one tested phase at a time. A later phase may start only after the user confirms the current phase's physical acceptance tests.

## Phase 0 - ESP32 baseline

- [x] Create the PlatformIO `esp32dev` project.
- [x] Add minimal 115200-baud heartbeat firmware.
- [x] Confirm the firmware builds locally.
- [x] Confirm Windows detects the ESP32 serial interface as CP210x on COM10.
- [x] Upload firmware to the ESP32 on COM10.
- [x] Confirm heartbeat output at 115200 baud.
- [x] User confirms the board remains cool and the heartbeat stays stable for at least 60 seconds.

## Phase 1 - MC-38 sensor

- [x] Add raw GPIO27 input firmware using `INPUT_PULLUP`.
- [x] Connect the MC-38 between GPIO27 and GND with USB power disconnected.
- [x] Confirm magnet near produces `RAW=0`.
- [x] Confirm magnet away produces `RAW=1`.
- [x] Add 50 ms debounce after the raw input test passes.
- [x] Pass the debounced open/closed transition test per user confirmation.
- [ ] Secure the temporary physical wiring before the full-device test.

## Phase 2 - DFPlayer and speaker

Phase 1 sensor behavior passed on 2026-08-15. Prepare and test audio independently from the door sensor.

- [x] User reports the FAT32 microSD and `mp3/0001.mp3` are prepared and inserted.
- [x] User reports DFPlayer power and crossed UART are connected.
- [x] Connect the speaker and confirm first audible playback (2026-10-09).
- [ ] Visually inspect all connections before applying power.
- [x] Add standalone HardwareSerial communication-test firmware.
- [x] Confirm DFPlayer initialization on current port COM8 (2026-10-09).
- [ ] Resolve the file-count query warning; later feedback reported value 1.
- [x] Confirm at least five clear playbacks without unexpected resets or distortion (user report, 2026-10-09).
- [ ] Verify adjustable volume during the standalone audio test.

## Phase 3 - Door/audio integration

Sensor and repeated audible playback passed independently. User requested integration on 2026-10-09; the file-count warning and separate volume-adjustment check remain follow-ups.

- [x] Combine the accepted sensor debounce and DFPlayer command.
- [x] Seed a stable initial door state without startup playback.
- [x] Play once on a debounced CLOSED-to-OPEN transition, then require closing to re-arm.
- [x] Add a 1000 ms cooldown and keep servicing the sensor during playback.
- [x] Build the integrated firmware at volume 16 (2026-10-09).
- [x] Upload the integrated firmware to COM8 and capture System ready at volume 16 (2026-10-09).
- [x] User confirms the basic integrated door trigger works (2026-10-09).
- [ ] Verify closed/open startup, 15-second hold, re-arm, repeated cycles, and power cycling physically.

## Phase 4 - V1 documentation and final physical validation

Blocked until integration passes.
