/**********************************************************
   Blynk + ESP32 Multi Sensor Monitor
   Sensors:
   - MQ135 Gas Sensor
   - HX711 Load Cell
   - BMP280 Temperature & Pressure
   - DHT11 Temperature & Humidity
   - GPS (NEO-6M / NEO-M8N)
**********************************************************/

#define BLYNK_TEMPLATE_ID "TMPL67DhRK1TF"
#define BLYNK_TEMPLATE_NAME "ESP32 Sensor Monitor"
#define BLYNK_AUTH_TOKEN "MeXBszbLx4w89eIaPmZaLe0z3RgVumbB"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#include "HX711.h"
#include <DHT.h>

#include <TinyGPS++.h>

//===================== WiFi =====================
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

//===================== MQ135 ====================
#define MQ135_PIN 35

//===================== HX711 ====================
#define HX_DOUT 18
#define HX_SCK 19

//===================== DHT11 ====================
#define DHTPIN 4
#define DHTTYPE DHT11

//===================== GPS ======================
#define GPS_RX 16   // ESP32 RX2
#define GPS_TX 17   // ESP32 TX2

HardwareSerial GPSSerial(2);
TinyGPSPlus gps;

//===================== Objects ==================
HX711 scale;
Adafruit_BMP280 bmp;
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

// HX711 calibration factor
float calibration_factor = -7050;

//================================================

void sendData()
{
  //---------------- MQ135 ----------------
  int gasValue = analogRead(MQ135_PIN);

  //---------------- HX711 ----------------
  float weight = scale.get_units(10);

  //---------------- BMP280 ---------------
  float bmpTemperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;

  //---------------- DHT11 ----------------
  float humidity = dht.readHumidity();
  float dhtTemperature = dht.readTemperature();

  Serial.println("--------------------------------");

  // MQ135
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  // Weight
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" g");

  // BMP280
  Serial.print("BMP Temperature: ");
  Serial.print(bmpTemperature);
  Serial.println(" C");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // DHT11
  if (isnan(humidity) || isnan(dhtTemperature))
  {
    Serial.println("DHT11 Read Failed");
  }
  else
  {
    Serial.print("DHT Temperature: ");
    Serial.print(dhtTemperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Blynk.virtualWrite(V4, dhtTemperature);
    Blynk.virtualWrite(V5, humidity);
  }

  // Send existing sensors
  Blynk.virtualWrite(V0, gasValue);
  Blynk.virtualWrite(V1, weight);
  Blynk.virtualWrite(V2, bmpTemperature);
  Blynk.virtualWrite(V3, pressure);

  //---------------- GPS ----------------
  if (gps.location.isValid())
  {
    Serial.println("GPS Data");

    Serial.print("Latitude : ");
    Serial.println(gps.location.lat(), 6);

    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);

    Serial.print("Speed    : ");
    Serial.print(gps.speed.kmph());
    Serial.println(" km/h");

    Serial.print("Altitude : ");
    Serial.print(gps.altitude.meters());
    Serial.println(" m");

    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());

    // Send to Blynk
    Blynk.virtualWrite(V6, gps.location.lat());
    Blynk.virtualWrite(V7, gps.location.lng());
    Blynk.virtualWrite(V8, gps.speed.kmph());
    Blynk.virtualWrite(V9, gps.altitude.meters());
    Blynk.virtualWrite(V10, gps.satellites.value());
  }
  else
  {
    Serial.println("Waiting for GPS Fix...");
  }
}

//================================================

void setup()
{
  Serial.begin(115200);

  // Connect WiFi & Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // I2C
  Wire.begin(21, 22);

  // BMP280
  if (!bmp.begin(0x76))
  {
    Serial.println("BMP280 not found!");
    while (1);
  }

  // HX711
  scale.begin(HX_DOUT, HX_SCK);
  scale.set_scale(calibration_factor);
  scale.tare();

  // DHT11
  dht.begin();

  // GPS
  GPSSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  // Timer
  timer.setInterval(2000L, sendData);

  Serial.println("================================");
  Serial.println("ESP32 Multi Sensor System Ready");
  Serial.println("================================");
}

//================================================

void loop()
{
  while (GPSSerial.available())
  {
    gps.encode(GPSSerial.read());
  }

  Blynk.run();
  timer.run();
}
