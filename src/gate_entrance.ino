
### Event Logging
- Events stored explicitly in LittleFS filesystem with timestamps, ensuring minimal memory footprint and persistent logging capability.

---

## Electrical Calibration Details

- **Current Sensor Zero-Current Voltage**: `2.5321V`
- **ACS758 Sensitivity**: `0.1006 V/A`
- **Voltage Reading Correction Factor**: `0.974`

---

## GPIO Pin Assignment (ESP32-S3)

| GPIO | Pin Name     | Function                 |
|------|--------------|--------------------------|
| 5    | SDA          | I2C Data                 |
| 4    | SCL          | I2C Clock                |
| 2    | Gate_Open    | Gate Open Signal Input   |
| 3    | Gate_Close   | Gate Closed Signal Input |

---

## Maintenance & Troubleshooting

- Regularly review OLED and web dashboard for anomalies in battery or sensor data.
- Inspect serial monitor for initialization and synchronization status periodically.
- Ensure Wi-Fi network stability for uninterrupted data reporting and NTP synchronization.

---

## Future Improvements
- Integration of remote monitoring capabilities via cloud services.
- Implementation of alert notifications (email/SMS) for critical battery or gate issues.

---

## Revision History

| Date       | Version | Description                         |
|------------|---------|-------------------------------------|
| 2025-05-13 | 1.0     | Initial Project Documentation Setup |

---

## License & Authorship

- MIT License. See [LICENSE](LICENSE) for details.
- Developed by Sphere LLC.

