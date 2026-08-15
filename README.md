# Door Voice IoT

A beginner-friendly ESP32 project that will eventually play `0001.mp3` through a DFPlayer Mini when an MC-38 sensor detects a door opening.

The project is currently limited to **Phase 0**: verifying ESP32 power, USB communication, firmware build/upload, and Serial output. The MC-38, DFPlayer Mini, speaker, networking, and all other later features are intentionally absent.

## Phase 0 firmware

The PlatformIO project is in `firmware/` and targets an ESP32 DevKit using the Arduino framework.

Build from PowerShell:

```powershell
cd firmware
C:\Users\Zaghi\.platformio\penv\Scripts\platformio.exe run
```

Do not set an upload port until Windows has detected the board and the actual COM number is known.

See `docs/wiring.md` before connecting the board and `docs/testing.md` for the acceptance checklist.
