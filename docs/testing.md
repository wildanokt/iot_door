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

## Phase 3: integrated door trigger

| Test | Expected result | Actual result |
| --- | --- | --- |
| Firmware build | ESP32 integrated firmware builds | Passed on 2026-10-09; RAM 21,592 bytes, flash 273,241 bytes |
| Firmware upload | Integrated firmware uploads to COM8 | Passed on 2026-10-09; all flash hashes verified and ESP32 reset normally |
| Boot CLOSED | Initial state CLOSED, System ready, no playback | Pending physical test |
| Boot OPEN | Initial state OPEN, no playback until a future close/open cycle | Pending physical test |
| Normal opening | One Door OPEN event and one track-1 playback command | Pending physical test |
| Hold OPEN for 15 seconds | No further opening event or playback | Pending physical test |
| Close and re-open | Closing is silent and re-arms; opening plays once after cooldown | Pending physical test |
| Ten opening cycles | One playback per valid opening, no missed or duplicate triggers | Pending physical test |
| Slow and fast magnet movement | Stable debounce; suppressed short openings/cooldown reported predictably | Pending physical test |
| USB power cycle | Returns to System ready with no startup playback | Pending physical test |

Volume is raised from the tested 12 to 16 on user request (approximately 30 percent more in the numerical setting, not a guaranteed acoustic percentage). Audio clarity and power stability at 16 remain to be checked physically. The integration no longer sends the standalone file-count query or automatically starts a track during setup. Audio commands use no ACK wait after the initialization handshake so sensor servicing does not wait for playback completion. A rapid re-opening after cooldown may restart a currently playing track.

The restricted session cannot create locks in the global PlatformIO installation. Build succeeded using a copy of its installed platform and packages in the git-ignored `firmware/.pio-core` directory, with `PLATFORMIO_CORE_DIR` set to that directory. Source dependencies and board configuration are unchanged.

### Integrated startup capture

On 2026-10-09, a 20-second COM8 capture at 115200 baud shows `DFPlayer initialized`, `Volume command: 16`, `Stop command sent`, `Initial door state: OPEN`, and `System ready`. No playback command, repeated reset, or flash-read error was observed. This verifies startup diagnostics and the initial OPEN state handling, but does not verify physical MC-38 wiring, audible silence, or a closed/open cycle. Serial was closed after the capture so the user can open their monitor.

After the user reports connecting the MC-38 and reconnecting USB, a fresh 60-second COM8 capture again shows initialization at volume 16, initial OPEN, and System ready. No subsequent door transitions, playback commands, or reboot messages were observed. Magnet movement during this capture has not been confirmed, so this does not establish a sensor fault or an integration pass. Serial was closed when capture completed.

On 2026-10-09, the user subsequently confirms that the integrated door trigger works. This accepts the basic door-trigger milestone at volume 16; the full ten-cycle, slow/fast movement, both startup-state, and power-cycle checklist above remains pending unless separately confirmed.

## Sequential MP3 extension (2026-10-09)

| Test | Expected result | Actual result |
| --- | --- | --- |
| Firmware build | ESP32 firmware builds with sequential tracks and bounded fallback | Passed on 2026-10-09; flash 273,781 bytes |
| Upload/startup | Upload succeeds; System ready and sequence start at 1 are logged | Passed on COM8; flash hashes verified, startup OPEN at volume 16, no startup playback command |
| Consecutive files | Eligible openings request 1, 2, 3 in order | Requests 1 then 2 captured; audible multiple-file sequence still pending because track 2 is absent |
| Missing next number | Error 5 or 6 causes one fallback command for track 1; next opening requests 2 | Passed on the connected module: four track-2 errors (code 6), each followed by one track-1 command |
| Missing track 1 | Error logged with no repeated automatic playback commands | Pending physical test |
| Held-open/cooldown | No extra command or sequence advancement without an eligible opening | Held-open behavior passed in capture; sub-1000 ms opening remains untested |
| Reset while open/closed | No startup playback and sequence restarts at 1 | Pending physical test |

Wraparound uses module error responses, not the unreliable standalone file-count query. If a compatible clone stays silent on a missing file, missing-track fallback has not passed and needs further diagnosis. No new sensor wiring is required.

### Sequential firmware Serial evidence

After upload on 2026-10-09, the first capture shows a CLOSED transition at 7.99 seconds, an OPEN transition requesting track 1 at 10.61 seconds, and a playback-finished response at 25.00 seconds. There is no duplicate playback command through the end of that 60-second capture.

A second 45-second capture continues without resetting the sequence. At 17.39 seconds an opening requests track 2; at 17.47 seconds the module returns error 6, and at 17.48 seconds the firmware sends exactly one fallback command for track 1. Later close/open cycles repeat that same behavior at 26.03, 28.52, and 32.62 seconds. This validates feedback-requested play packets, real missing-file responses, wraparound, and re-arming on this module. No repeated boot messages were observed. Acoustic quality and multiple distinct tracks still require the user's listening test. Serial was closed after capture.
