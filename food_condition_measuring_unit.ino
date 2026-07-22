#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===========================
// Select camera model in board_config.h
// ===========================
#include "board_config.h"

// ===========================
// OLED Configuration (I2C)
// ===========================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C  // Common I2C address for SSD1306 (0x3C or 0x3D)

#define I2C_SDA 15  // Custom SDA pin for ESP32-CAM
#define I2C_SCL 14  // Custom SCL pin for ESP32-CAM

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===========================
// Enter your WiFi credentials
// ===========================
const char *ssid = "Dilitha Rajapaksha’s iPhone";
const char *password = "********";

void startCameraServer();
void setupLedFlash();

// Helper to display sensor and connection details on OLED
void updateOledDisplay(sensor_t *s, camera_config_t &config, bool wifiConnected) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println("--- CAM & SENSOR ---");

  // Sensor PID
  display.printf("PID: 0x%04X\n", s->id.PID);

  // Resolution output
  display.print("Res: ");
  switch (config.frame_size) {
    case FRAMESIZE_UXGA:    display.println("UXGA (1600x1200)"); break;
    case FRAMESIZE_SVGA:    display.println("SVGA (800x600)");   break;
    case FRAMESIZE_QVGA:    display.println("QVGA (320x240)");   break;
    case FRAMESIZE_240X240: display.println("240x240");         break;
    default:                display.println("Custom/Other");    break;
  }

  // Format
  display.print("Format: ");
  if (config.pixel_format == PIXFORMAT_JPEG) {
    display.println("JPEG");
  } else if (config.pixel_format == PIXFORMAT_RGB565) {
    display.println("RGB565");
  } else {
    display.println("RAW/Other");
  }

  // PSRAM vs DRAM location
  display.print("FB Loc: ");
  display.println(config.fb_location == CAMERA_FB_IN_PSRAM ? "PSRAM" : "DRAM");

  // WiFi Status
  display.println("--------------------");
  if (wifiConnected) {
    display.print("IP: ");
    display.println(WiFi.localIP());
  } else {
    display.println("WiFi: Connecting...");
  }

  display.display();
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // Initialize I2C on GPIO 15 (SDA) and GPIO 14 (SCL)
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize OLED Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 OLED allocation failed"));
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Initializing...");
    display.display();
  }

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  //config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.printf("Cam Init Failed!\nErr: 0x%x", err);
    display.display();
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }

  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

#if defined(LED_GPIO_NUM)
  setupLedFlash();
#endif

  // Update display with initial sensor configuration before WiFi connects
  updateOledDisplay(s, config, false);

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  // Final display update with assigned IP address
  updateOledDisplay(s, config, true);

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  delay(10000);
}