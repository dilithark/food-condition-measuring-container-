# 📦 IoT-Based Food Condition Measuring Container

An ESP32-based IoT system for monitoring food storage conditions in real time. The project integrates multiple environmental sensors and uploads data to the Blynk IoT platform, allowing users to monitor food storage conditions remotely.

---

## 🚀 Features

- 🌡️ Temperature Monitoring (BMP280 & DHT11)
- 💧 Humidity Monitoring
- 📊 Atmospheric Pressure Monitoring
- ⚖️ Weight Measurement using HX711 Load Cell
- 🌫️ Gas Detection using MQ135
- 📍 GPS Location Tracking
- ☁️ Real-time Monitoring with Blynk IoT
- 📱 Mobile Dashboard Support

---

## 🛠 Hardware Used

| Component | Description |
|------------|-------------|
| ESP32 Dev Board | Main Controller |
| MQ135 | Air Quality / Gas Sensor |
| HX711 Amplifier | Load Cell Interface |
| Load Cell | Weight Measurement |
| BMP280 | Temperature & Pressure Sensor |
| DHT11 | Temperature & Humidity Sensor |
| NEO-6M GPS Module | GPS Tracking |
| Food Storage Container | Prototype Enclosure |

---

## 📡 Sensor Data

The system measures:

- Gas Concentration
- Weight
- Temperature (BMP280)
- Atmospheric Pressure
- Temperature (DHT11)
- Humidity
- GPS Latitude
- GPS Longitude
- GPS Speed
- GPS Altitude
- Satellite Count

---

## 📱 Blynk Virtual Pins

| Virtual Pin | Data |
|--------------|------|
| V0 | MQ135 Gas Value |
| V1 | Weight |
| V2 | BMP280 Temperature |
| V3 | Pressure |
| V4 | DHT11 Temperature |
| V5 | Humidity |
| V6 | Latitude |
| V7 | Longitude |
| V8 | Speed |
| V9 | Altitude |
| V10 | Satellite Count |

---

## 🔌 Wiring

### MQ135
| MQ135 | ESP32 |
|--------|-------|
| AO | GPIO35 |
| VCC | 5V |
| GND | GND |

### HX711
| HX711 | ESP32 |
|--------|-------|
| DT | GPIO18 |
| SCK | GPIO19 |
| VCC | 5V |
| GND | GND |

### DHT11
| DHT11 | ESP32 |
|--------|-------|
| DATA | GPIO4 |
| VCC | 3.3V |
| GND | GND |

### BMP280 (I2C)
| BMP280 | ESP32 |
|---------|-------|
| SDA | GPIO21 |
| SCL | GPIO22 |
| VCC | 3.3V |
| GND | GND |

### GPS Module
| GPS | ESP32 |
|-----|-------|
| TX | GPIO16 |
| RX | GPIO17 |
| VCC | 5V |
| GND | GND |

---

## 📚 Required Libraries

Install the following libraries using the Arduino Library Manager.

- Blynk
- Adafruit BMP280
- Adafruit Unified Sensor
- HX711
- DHT Sensor Library
- TinyGPS++
- Wire

---

## ⚙️ Configuration

Update your WiFi credentials.

```cpp
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";
```

Update your Blynk credentials.

```cpp
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"
```

---

## 📈 Dashboard

The Blynk dashboard displays:

- Gas Sensor Reading
- Weight
- Temperature
- Humidity
- Pressure
- GPS Location
- Speed
- Altitude
- Satellite Status

---

## 📷 Prototype

> Add images of the completed hardware here.

Example:

```
images/prototype.jpg
images/dashboard.jpg
images/circuit.jpg
```

---

## 🧠 Future Improvements

- Food freshness prediction using Machine Learning
- Cloud database integration
- Mobile notifications
- OLED display
- SD card data logging
- Battery backup
- CO₂ and VOC concentration estimation
- AI-based food spoilage detection

---

## 📂 Project Structure

```
Food-Condition-Measuring-Container/
│
├── Food_Condition_Container.ino
├── README.md
├── images/
│   ├── prototype.jpg
│   ├── dashboard.jpg
│   └── circuit.jpg
└── docs/
```

---

## 👨‍💻 Author

**Dilitha Rajapaksha**

IT Undergraduate | University of Moratuwa

- Robotics & IoT Enthusiast
- Embedded Systems Developer
- AI Enthusiast

GitHub:
https://github.com/dilithark

---

## ⭐ Support

If you found this project helpful, consider giving it a ⭐ on GitHub.
