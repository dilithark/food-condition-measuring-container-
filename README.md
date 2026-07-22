# ESP32-CAM Web Server with OLED Display Integration

A customized ESP32-CAM project that runs a web video streaming / face detection server and displays real-time camera sensor information, resolution status, frame buffer location, and network connection details on a 0.96-inch I2C SSD1306 OLED display.

---

## 📌 Features

- **Web Camera Server**: Stream video or perform face detection using standard `esp_camera` drivers.
- **Real-Time OLED Status Display**:
  - Sensor PID (e.g., OV2640, OV3660)
  - Current Frame Resolution (UXGA, SVGA, QVGA, 240x240, etc.)
  - Pixel Format (JPEG / RGB565 / RAW)
  - Frame Buffer Location (PSRAM or internal DRAM)
  - WiFi Connection Status & Local IP Address
- **Dynamic Pin re-mapping for I2C**: Uses safe pins (`GPIO 15` for SDA, `GPIO 14` for SCL) to avoid interference with camera data lines.

---

## 🛠️ Hardware Requirements

| Hardware Component | Details |
| :--- | :--- |
| **ESP32-CAM Board** | AI-Thinker, ESP-EYE, or ESP32-S3 Eye |
| **OLED Display** | 0.96-inch SSD1306 (128x64 resolution, I2C interface) |
| **FTDI Programmer** | Required for uploading code to standard ESP32-CAM |
| **Jumper Wires** | Female-to-Female connectors |

---

## 🔌 Hardware Connections (ESP32-CAM to OLED)

> ⚠️ **Important:** Connect the OLED to the following pins to avoid conflicts with camera data lines.

| OLED Display Pin | ESP32-CAM Pin | Notes |
| :--- | :--- | :--- |
| **VCC** | **3.3V** or **5V** | Check display voltage tolerance |
| **GND** | **GND** | Ground connection |
| **SDA** | **GPIO 15** | I2C Data Line |
| **SCL** | **GPIO 14** | I2C Clock Line |

*Note: Disconnect or avoid pulling GPIO 15 low during boot/flashing if it interferes with board strapping states.*

---

## 💻 Software & Library Requirements

1. **Arduino IDE** (v1.8.x or v2.x) with **ESP32 Board Support** installed.
2. Libraries (Install via **Tools > Manage Libraries** in Arduino IDE):
   - **Adafruit SSD1306** (by Adafruit)
   - **Adafruit GFX Library** (by Adafruit)
   - **Wire** (Built-in C++ library for I2C)
   - **WiFi** & **esp_camera** (Included in ESP32 board package)

---

## 🚀 Setup & Flashing Instructions

1. **Open Project**: Load the `.ino` sketch into Arduino IDE.
2. **Select Board**:
   - Go to `Tools > Board > ESP32 Arduino` and select **AI Thinker ESP32-CAM** (or your specific camera model).
3. **Configure Board**:
   - Ensure PSRAM is enabled if available (`Tools > PSRAM > Enabled`).
4. **Update WiFi Credentials**:
   ```cpp
   const char *ssid = "YOUR_WIFI_SSID";
   const char *password = "YOUR_WIFI_PASSWORD";
   ```
5. **Set Board Configuration**:
   - Make sure your camera model definition is correctly selected in `board_config.h` (e.g., `#define CAMERA_MODEL_AI_THINKER`).
6. **Upload**:
   - Connect GPIO 0 to GND for programming mode.
   - Click **Upload**.
   - After flashing, disconnect GPIO 0 from GND and press the Reset button.

---

## 🖥️ OLED Screen Output Overview

When running, the screen will display:

```text
--- CAM & SENSOR ---
PID: 0x2642
Res: 240x240
Format: RGB565
FB Loc: PSRAM
--------------------
IP: 192.168.1.50
```

---

## 🔍 Troubleshooting

- **OLED Allocation Failed**:
  - Check I2C address (default is `0x3C`; if unresponsive, try changing to `0x3D` in code).
  - Verify physical connections on `GPIO 15` (SDA) and `GPIO 14` (SCL).
- **Camera Init Failed**:
  - Ensure external 5V power supply provides sufficient current (at least 1A).
  - Confirm ribbon cable from camera module is firmly seated.

---

## 📄 License

This project is open-source under the MIT License.
