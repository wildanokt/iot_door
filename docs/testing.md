# Hardware test record

## Phase 0: ESP32, USB, upload, and Serial

| Test | Expected result | Actual result |
| --- | --- | --- |
| ESP32 physical inspection | Pins are straight, board is stable, no obvious short | Passed by user on 2026-08-15 |
| USB power | Board powers without heat, smoke, or unstable connection | Passed by user on 2026-08-15 |
| Windows USB detection | CP210x or USB Serial device appears with a COM port | Passed on 2026-08-15; Silicon Labs CP210x detected on COM10 |
| Firmware build | PlatformIO reports `SUCCESS` | Passed locally on 2026-08-14; PlatformIO completed successfully |
| Firmware upload | Upload completes and ESP32 resets | Passed on 2026-08-15 using COM10; flash hashes verified and hard reset completed |
| Serial Monitor | Readable boot message and one heartbeat per second at 115200 baud | Passed automated capture on 2026-08-15; Heartbeat 16 through 27 arrived once per second |
| Stability | Heartbeat continues without random resets | Passed by user on 2026-08-15 for more than 60 heartbeats; no heat issue reported |

Do not begin Phase 1 until every Phase 0 test has passed.

## Phase 1A-B: raw MC-38 input

| Test | Expected result | Actual result |
| --- | --- | --- |
| No sensor connected | Internal pull-up makes GPIO27 report `RAW=1` | Passed on 2026-08-15 after build and upload to COM10 |
| Magnet near MC-38 | Closed switch connects GPIO27 to GND and reports `RAW=0` | Passed per user report on 2026-08-15 |
| Magnet away from MC-38 | Open switch lets the pull-up report `RAW=1` | Passed per user report on 2026-08-15 |
| Repeat movement | Raw state follows magnet position consistently | Function confirmed by user; temporary wiring issue remains unresolved |

Do not add software debounce until the raw readings pass.

## Phase 1C: 50 ms debounce

| Test | Expected result | Actual result |
| --- | --- | --- |
| Firmware build and upload | PlatformIO succeeds and ESP32 resets normally | Passed on 2026-08-15 using COM10 |
| Boot with magnet near | `Initial door state: CLOSED` | Passed per user confirmation on 2026-08-15 |
| Move magnet away | Exactly one `Door OPEN` | Passed per user confirmation on 2026-08-15 |
| Keep magnet away for 10 seconds | No additional `Door OPEN` events | Passed per user confirmation on 2026-08-15 |
| Return magnet | Exactly one `Door CLOSED` | Passed per user confirmation on 2026-08-15 |
| Repeat transitions | One stable event per transition, with no false triggers | Passed per user confirmation on 2026-08-15 |

Known hardware follow-up: secure the temporary MC-38 wiring before the full-device acceptance test.

## Phase 2A-B: DFPlayer preparation and wiring

| Test | Expected result | Actual result |
| --- | --- | --- |
| microSD preparation | FAT32 card contains `mp3/0001.mp3` | Pending user preparation |
| Power wiring | ESP32 VIN/5V to VCC and common GND | Pending user wiring |
| UART wiring | GPIO17 through about 1k ohm to DFPlayer RX; DFPlayer TX to GPIO16 | Pending user wiring |
| Speaker wiring | Speaker is connected only across SPK1 and SPK2 | Pending user wiring |
| Visual inspection | No shorts, reversed power, or speaker-to-GND connection | Pending user inspection |

## Phase 2C: communication test without speaker

| Test | Expected result | Actual result |
| --- | --- | --- |
| Firmware build and upload | PlatformIO succeeds on COM10 | Pending local test |
| DFPlayer initialization | Module responds over UART | Pending local test |
| microSD query | Module returns a non-negative file count | Pending local test |
| Track command | Command for `mp3/0001.mp3` is issued | Pending local test |
| Audible playback | Clear audio from speaker | Intentionally not tested; speaker disconnected |

## 2026-10-09: rewiring and first audible playback

- Current USB interface: Silicon Labs CP210x on COM8.
- After wiring corrections, user-provided Serial output shows normal ESP32 boot and `DFPlayer initialized`.
- Track command for `mp3/0001.mp3` was sent at volume 12.
- The synchronous microSD file-count query still reports a warning. Later feedback type 11 reports value 1; this likely represents a delayed file-count reply, but the log does not include its command identifier.
- User confirms audio is now audible through the connected speaker. Clarity and repeated playback stability remain to be confirmed.
- Pending Phase 2 acceptance: at least five successful, understandable playbacks without unexpected ESP32 resets or distortion, and adjustable volume.
- No Phase 3 integration acceptance has been performed.

### Repeated standalone playback accepted

On 2026-10-09, the user confirms all five complete playbacks passed the requested clarity and stability check and asks to proceed to the door trigger. Record this as the repeated-playback milestone. The synchronous file-count warning and a separate volume-adjustment test remain follow-ups; neither has been marked passed.
