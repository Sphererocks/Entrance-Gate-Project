# Entrance Gate Project – Design Document

## System Architecture

### Overview
The **Entrance Gate Project** utilizes an ESP32-S3 microcontroller module to monitor and report battery voltage, current draw, environmental conditions (temperature and humidity), and gate entrance status in real-time. It provides data visualization through an OLED display and a dedicated webserver interface.

---

## Hardware Architecture

### Microcontroller
- **ESP32-S3 (XIAO ESP32S3 Sense Module)**
  - Wi-Fi connectivity
  - GPIO for I/O handling
  - Integrated I2C bus communication

### Sensors and Modules
- **Current Monitoring**: ACS758LCB-050B linear current sensor
- **Analog-to-Digital Conversion**: ADS1115 ADC (16-bit resolution)
- **Environmental Sensing**: AHT10 temperature/humidity sensor
- **User Interface**: SSD1306 OLED (128x64 pixels, auto-scrolling interface)

### Power Supply
- **Main Supply**: Stable regulated 5VDC power rail
- **Battery Plant**: Two (2) x CSB 9Ah VRLA stationary batteries (12V nominal, 18Ah total capacity)

---

## Software Architecture

### Core Functionalities
- **Data Acquisition**
  - I2C communication for sensor data (ADS1115, AHT10)
  - Precise calibration for voltage and current measurements
- **Real-time Display**
  - OLED display cycles through battery, environmental, and system information every 3 seconds
- **Data Logging and Gate Status Tracking**
  - Store last 5 gate opening and closing events with timestamps and current readings
- **Web Server Interface**
  - Real-time data dashboard accessible at `192.168.10.190`

### Networking and Synchronization
- **Wi-Fi**: Static IP configuration (`192.168.10.190`)
- **Time Synchronization**: Robust NTP synchronization using external servers (`pool.ntp.org` and `time.nist.gov`)

### Robustness and Error Handling
- **I2C Error Mitigation**: Explicit retry mechanism for sensor initialization
- **Filesystem (LittleFS)**: Automatic format and recovery in case of corruption detected at startup

---

## Data Flow & Storage

### Data Acquisition Flow
