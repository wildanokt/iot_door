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
