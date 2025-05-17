# Entrance Gate Project

## Project Overview
The **Entrance Gate Project** is a robust monitoring system designed for tracking battery performance, environmental conditions, and gate entrance operations using an ESP32-S3 microcontroller module.

---

## Functional Requirements
The system must explicitly provide the following functionality:

### 1. Battery and Electrical Monitoring
- Real-time voltage and current measurements.
- Accurate current measurement using an ACS758LCB-050B Hall-effect sensor.
- Battery percentage estimation from voltage readings.

### 2. Environmental Monitoring
- Real-time temperature (°F) and humidity (%) monitoring using an AHT10 sensor.

### 3. Gate Status Monitoring
- Explicit detection of gate states: Open, Closed, Moving.
- Record timestamps and amperage drawn during each gate open/close event.

### 4. Data Logging & Presentation
- Store the last five gate opening and closing events in memory, with explicit timestamps.
- Display real-time measurements on an SSD1306 OLED (auto-scrolling every 3 seconds).
- Provide a real-time web server dashboard accessible at static IP `192.168.10.190`, displaying all key parameters and recent gate events.

---

## Hardware Requirements
- **Microcontroller**: ESP32-S3 (Seeed Studio XIAO ESP32S3 Sense Module)
- **Current Sensor**: ACS758LCB-050B Hall-effect linear current sensor
- **ADC**: Adafruit ADS1115 16-bit ADC module
- **Environmental Sensor**: AHT10 temperature/humidity sensor
- **Display**: SSD1306 OLED (128x64)
- **Power Supply**: Stable regulated 5VDC rail
- **Battery Plant**: Two (2) CSB 9Ah VRLA stationary batteries (12VDC nominal)

---

## Software Requirements
- **Platform**: Arduino IDE (ESP32 board package installed)
- **Libraries required**:
  - WiFi.h (built-in ESP32 Wi-Fi library)
  - Wire.h (I2C communication)
  - Adafruit_ADS1X15
  - U8g2lib (OLED display)
  - WebServer.h
  - AHTxx (Enjoyneering library)
  - FS.h and LittleFS.h (filesystem for logging)
  - NTP synchronization (built-in Arduino ESP32 support)

---

## Calibration Data (Explicitly Verified)
| Parameter               | Calibrated Value |
|-------------------------|------------------|
| Current Sensor Zero-V   | 2.5321 V         |
| ACS758 Sensitivity      | 0.1006 V/A       |
| Voltage correction      | Factor 0.974     |

---

## Communication & Networking
- **Wi-Fi Network**: `SphereMako`
- **Static IP Configuration**:
  - IP: `192.168.10.190`
  - Gateway: `192.168.10.1`
  - Subnet Mask: `255.255.255.0`
- **Time Synchronization**: NTP servers `pool.ntp.org`, `time.nist.gov`

---

## Known Issues & Mitigations
- Initial I2C transaction NACK errors occasionally appear due to sensor startup delays. These are explicitly handled with retries and increased delays.

- LittleFS corruption is explicitly managed through automatic reformatting if corruption is detected during startup.

---

## Usage Instructions
1. Clone this repository from GitHub.
2. Open `entrance_gate_project.ino` in Arduino IDE.
3. Ensure all libraries listed above are installed.
4. Upload the sketch to ESP32.
5. Connect to the Wi-Fi network `SphereMako`.
6. Access real-time data at [http://192.168.10.190](http://192.168.10.190).

---

## Repository Structure
