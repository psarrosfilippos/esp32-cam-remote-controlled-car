#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

// ===== CAMERA PIN CONFIG =====
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5

#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ===== MOTOR CONTROL PINS =====
#define IN1 2
#define IN2 14
#define IN3 15
#define IN4 13

// ===== WIFI CREDENTIALS =====
const char* ssid     = "***";
const char* password = "***";

// ===== WEB SERVER =====
WebServer server(80);

// ===== IMAGE BUFFER =====
String imgBase64 = "";

// ===== CAMERA SETUP =====
void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x", err);
  }
}

// ===== MOTOR FUNCTIONS =====
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// ===== HTML PAGE =====
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head><title>ESP32 Car</title></head>
  <body style="text-align:center">
    <h2>ESP32 CAMERA CAR</h2>
    <img id="cam" src="/capture" width="320" height="240"><br><br>
    <button onclick="sendCommand('F')">Forward</button><br>
    <button onclick="sendCommand('L')">Left</button>
    <button onclick="sendCommand('S')">Stop</button>
    <button onclick="sendCommand('R')">Right</button><br>
    <button onclick="sendCommand('B')">Backward</button>
    <script>
      setInterval(() => {
        document.getElementById('cam').src = '/capture?' + new Date().getTime();
      }, 500);
      function sendCommand(cmd) {
        fetch('/cmd?dir=' + cmd);
      }
    </script>
  </body>
</html>
)rawliteral";

// ===== HANDLERS =====
void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleCapture() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }

  WiFiClient client = server.client();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: image/jpeg");
  client.println("Content-Length: " + String(fb->len));
  client.println();
  client.write(fb->buf, fb->len);

  esp_camera_fb_return(fb);
}

void handleCommand() {
  String dir = server.arg("dir");
  if      (dir == "F") moveForward();
  else if (dir == "B") moveBackward();
  else if (dir == "L") moveLeft();
  else if (dir == "R") moveRight();
  else                 stopMotors();
  server.send(200, "text/plain", "OK");
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopMotors();

  WiFi.softAP(ssid, password);
  Serial.println("WiFi started");
  Serial.println(WiFi.softAPIP());

  startCamera();

  server.on("/", handleRoot);
  server.on("/capture", handleCapture);
  server.on("/cmd", handleCommand);
  server.begin();
}

// ===== LOOP =====
void loop() {
  server.handleClient();
}

