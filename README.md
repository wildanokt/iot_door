# Door Voice IoT

A beginner-friendly ESP32 project that plays numbered MP3 files through a DFPlayer Mini when an MC-38 sensor detects a door opening.

The project is currently at **Phase 3**: door-trigger integration prepared for physical validation. ESP32 Serial, the MC-38 sensor, and repeated standalone DFPlayer playback have passed independent tests.

The integrated firmware uses GPIO27 with an internal pull-up and 50 ms debounce. Each valid CLOSED-to-OPEN transition requests the next track at volume 16: `mp3/0001.mp3`, then `0002.mp3`, then `0003.mp3`, and so on. Closing re-arms the trigger. Booting while OPEN does not play audio. Openings within 1000 ms of the last playback command are suppressed without advancing the sequence and require a new close/open cycle; there is no delayed retry. A new valid opening after cooldown sends a new playback command even if the previous track is still playing.

Use consecutive four-digit filenames from `0001.mp3` through at most `9999.mp3` in the card's `mp3` folder. When the module reports a missing/out-of-range file (error 5 or 6), a failed request above track 1 falls back to `0001.mp3` once during that opening, then the next opening tries `0002.mp3`. A gap ends the sequence: if `0002.mp3` is missing, `0003.mp3` will not be reached. If track 1 also fails, the firmware logs an error and waits for another valid opening instead of automatically retrying. Restarting the ESP32 resets the next track to 1. Playback completion alone does not advance or play a track.

Play commands use the DFRobot 0x12 protocol with feedback requested, sent directly over HardwareSerial without waiting for an ACK. The official library parses responses asynchronously. Wraparound depends on the compatible DFPlayer module reporting a missing-file error. Timeouts or other module errors are logged without assuming a file is missing; silent clone behavior still requires physical verification.

DFPlayer initialization errors remain visible; if initialization fails, sensor diagnostics continue and audio triggers are skipped. An initial sensor input that fails to remain stable for 50 ms within 2 seconds disables door triggering until a reset. The standalone file-count query is no longer performed in the integration firmware; its earlier warning remains recorded in `docs/testing.md`.

## Firmware

The PlatformIO project is in `firmware/` and targets an ESP32 DevKit using the Arduino framework.

Build from PowerShell:

```powershell
cd firmware
C:\Users\Zaghi\.platformio\penv\Scripts\platformio.exe run
```

Do not set an upload port until Windows has detected the board and the actual COM number is known.

For the project-local build cache prepared in this session, run `$env:PLATFORMIO_CORE_DIR = Join-Path (Get-Location) '.pio-core'` from `firmware/` before the PlatformIO command. Otherwise use the normal installed PlatformIO core above. Current confirmed port is COM8; close Serial Monitor before upload.

See `docs/wiring.md` before connecting the board and `docs/testing.md` for the acceptance checklist.
