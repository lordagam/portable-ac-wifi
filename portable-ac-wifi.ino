#include <Adafruit_MCP9808.h>
#include <ESP8266WebServer.h>

#include "src/ac-settings-encoder.h"
#include "src/eeprom-wifi.h"
#include "src/index.html.h"
#include "src/settings-handler.h"

// Default MCP9808 breakout address, no address pins connected.
#define TEMP_SENSOR_I2C_ADDR 0x18
// The most precise resolution (0.0625 deg), but longest sample time (250ms).
#define TEMP_SENSOR_RESOLUTION_MODE 3
#define IR_OUTPUT_PIN 14
#define IR_OUTPUT_INACTIVE LOW
#define IR_FREQ 38000
#define SERVER_HTTP_PORT 80
#define SERIAL_BAUD 115200

Adafruit_MCP9808 temp_sensor;
ACSettingsEncoder ac(IR_OUTPUT_PIN, IR_OUTPUT_INACTIVE);
ESP8266WebServer server(SERVER_HTTP_PORT);

float sampleTempF() {
  // Resume temperature sampling.
  temp_sensor.wake();
  float f = temp_sensor.readTempF();
  // Stop temperature sampling to reduce power consumption.
  temp_sensor.shutdown();
  return f;
}

void handleSettingsCallback() {
  handleSettings(server, ac, sampleTempF());
}

void initWebServer() {
  server.on("/settings", handleSettingsCallback);
  server.on("/", []() {
    server.send(kHTTPOK, kIndexContentType, FPSTR(kIndexHtml));
  });
  server.begin();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(IR_OUTPUT_PIN, OUTPUT);
  digitalWrite(IR_OUTPUT_PIN, IR_OUTPUT_INACTIVE);
  analogWriteFreq(IR_FREQ);

  Serial.begin(SERIAL_BAUD);
  delay(500);

  if (!temp_sensor.begin(TEMP_SENSOR_I2C_ADDR)) {
    Serial.println(F("Couldn't find MCP9808!"));
  }
  temp_sensor.setResolution(TEMP_SENSOR_RESOLUTION_MODE);

  // Get the A/C state in sync with internal state.
  ac.send();

  initEEPROMWiFi();
  initWebServer();
}

void loop() {
  server.handleClient();
}
