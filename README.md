# Door Voice IoT

A beginner-friendly ESP32 project that will eventually play `0001.mp3` through a DFPlayer Mini when an MC-38 sensor detects a door opening.

The project is currently at **Phase 3**: door-trigger integration prepared for physical validation. ESP32 Serial, the MC-38 sensor, and repeated standalone DFPlayer playback have passed independent tests.

The integrated firmware uses GPIO27 with an internal pull-up and 50 ms debounce. One stable CLOSED-to-OPEN transition plays `mp3/0001.mp3` at volume 16 (raised from 12 at the user's request). Closing re-arms the trigger. Booting while OPEN does not play audio. Openings within 1000 ms of the last playback command are suppressed and require a new close/open cycle; there is no delayed retry. A new valid opening after cooldown sends a new playback command even if the previous track is still playing.

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
