# ReadingEcowitt ESP32 Firmware

## Overview

This firmware configures an ESP32 DevKit V1 as an HTTP server to receive weather data from an Ecowitt weather station gateway over a local Wi-Fi network.

The received weather parameters are converted into a compact JSON packet and forwarded to a RAK4631 module through UART communication.

## Hardware Requirements

- ESP32 DevKit V1
- RAK19007 WisBlock Base Board
- RAK4631 LoRa module
- Ecowitt weather station and gateway

## Software Requirements

- Arduino IDE
- ESP32 board package

## Installation

1. Download and extract the ESP32 Arduino firmware.
2. Open `ReadingEcowitt_ESP32.ino` in Arduino IDE.
3. Copy `config.h.example` and rename the copy to `config.h`.
4. Enter your local Wi-Fi SSID and password in `config.h`.
5. Select ESP32 Dev Module under Tools → Board.
6. Select the appropriate COM port.
7. Compile and upload the firmware.

## Wi-Fi Configuration

After uploading, open Arduino Serial Monitor at 115200 baud.

The ESP32 will display its assigned local IP address after connecting to Wi-Fi.

## Ecowitt Gateway Configuration

Configure the Ecowitt gateway custom server settings:

- Protocol: HTTP
- Server IP: ESP32 IP address
- Port: 80
- Upload path: `/weatherstation/updateweatherstation.php`

The Ecowitt gateway and ESP32 must be connected to the same local Wi-Fi network.

## UART Connections

Connect ESP32 DevKit V1 to RAK19007:

| ESP32 DevKit V1 | RAK19007 J10 |
|---|---|
| TX2 (GPIO17) | RX1 (Pin 4) |
| RX2 (GPIO16) | TX1 (Pin 3) |
| GND | GND (Pin 2) |

UART baud rate: 115200 bps.