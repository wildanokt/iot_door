# Audio source placeholder

Place copies of the voice files here as `0001.mp3`, `0002.mp3`, and so on for project reference.

Copy the same file to the FAT32 microSD card using this structure:

```text
microSD/
`-- mp3/
    |-- 0001.mp3
    |-- 0002.mp3
    `-- 0003.mp3
```

Recommended test audio is a clear 5-10 second MP3 with little silence at the beginning or end.

Use consecutive numbers with no gaps. Each valid opening advances one track. A module-reported missing track restarts playback at `0001.mp3`, with `0002.mp3` tried on the following opening. With only `0001.mp3` present, the second opening tries the missing `0002.mp3` and should fall back to track 1. Resetting the ESP32 restarts the sequence at 1.
