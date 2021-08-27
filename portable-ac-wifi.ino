#include <ESP8266WebServer.h>

#include "ac-settings-encoder.h"
#include "eeprom-wifi.h"
#include "index.html.h"
#include "settings-handler.h"

#define IR_OUTPUT_PIN 14
#define IR_OUTPUT_INACTIVE LOW
#define IR_FREQ 38000

ACSettingsEncoder ac(IR_OUTPUT_PIN, IR_OUTPUT_INACTIVE);
ESP8266WebServer server(80);

void handleSettingsCallback() {
  handleSettings(server, ac);
}

void initWebServer() {
  server.on("/settings", handleSettingsCallback);
  server.on("/", []() {
    server.send(200, kIndexContentType, kIndexHtml);
  });
  server.begin();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(IR_OUTPUT_PIN, OUTPUT);
  digitalWrite(IR_OUTPUT_PIN, IR_OUTPUT_INACTIVE);
  analogWriteFreq(IR_FREQ);

  Serial.begin(115200);
  delay(500);

  initEEPROMWiFi();
  initWebServer();
}

void loop() {
  server.handleClient();
}
