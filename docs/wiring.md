# Phase 0 wiring

For Phase 0, connect only these two ends:

```text
Laptop USB port -> USB-C data cable -> ESP32 USB-C connector
```

Do not connect the MC-38, DFPlayer Mini, speaker, or any other wires yet.

Before applying power, confirm the ESP32 header pins are straight, the board is stable in the breadboard, and there is no obvious short circuit. Do not force the board farther into the breadboard if it is already stable.

## Phase 1 raw MC-38 test

Disconnect the USB cable before making these connections:

```text
ESP32 GPIO27 -> MC-38 wire/terminal 1
MC-38 wire/terminal 2 -> ESP32 GND
```

The MC-38 is only a switch in this test, so its two wires do not have positive and negative polarity. Do not connect either MC-38 wire to 3.3V or 5V.

If the ESP32 is mounted across the breadboard center gap, place each jumper in a hole that shares a connected row with the intended ESP32 pin. Do not place both MC-38 connections in the same connected group of breadboard holes.

The DFPlayer Mini and speaker must remain disconnected during this test.

## Phase 2 DFPlayer and speaker

Disconnect the USB cable before making or changing any of these connections:

```text
ESP32 VIN/5V -> DFPlayer VCC
ESP32 GND -> DFPlayer GND

ESP32 GPIO17/TX -> approximately 1k ohm resistor -> DFPlayer RX
DFPlayer TX -> ESP32 GPIO16/RX

DFPlayer SPK1 -> speaker terminal 1
DFPlayer SPK2 -> speaker terminal 2
```

ESP32 and DFPlayer must share GND. UART is crossed: ESP32 TX goes to DFPlayer RX, and DFPlayer TX goes to ESP32 RX.

Never connect either speaker terminal to an ESP32 pin or GND. The speaker must be connected only across DFPlayer `SPK1` and `SPK2`.

Use the ESP32 pin marked `VIN` or `5V` for DFPlayer power, not `3V3`. Insert the prepared microSD card before applying power.
