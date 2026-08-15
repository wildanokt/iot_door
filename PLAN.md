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

Phase 0 passed on 2026-08-15. Next, test raw GPIO27 input; add debounce only after raw input succeeds.

## Phase 2 - DFPlayer and speaker

Blocked until Phase 1 physically passes. Test audio independently from the door sensor.

## Phase 3 - Door/audio integration

Blocked until both sensor and audio subsystems pass independently.

## Phase 4 - V1 documentation and final physical validation

Blocked until integration passes.
