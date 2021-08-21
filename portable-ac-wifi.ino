#include <EEPROM.h>
#include <ESP8266WiFi.h>

#include "ac-settings-encoder.h"

#define IR_OUTPUT_PIN 14
#define IR_OUTPUT_INACTIVE LOW
#define IR_FREQ 38000

ACSettingsEncoder ac(IR_OUTPUT_PIN, IR_OUTPUT_INACTIVE);

#define SSID_MAX 32
#define PSK_MAX 64
#define CONFIG_SIZE (SSID_MAX + PSK_MAX)

#define CONNECT_TIMEOUT_MS 30000

bool connectToWifi() {
  char ssid[SSID_MAX + 1] = {0};
  char psk[PSK_MAX + 1] = {0};
  memcpy(ssid, &EEPROM[0], SSID_MAX);
  memcpy(psk, &EEPROM[SSID_MAX], PSK_MAX);
  if (!ssid[0]) {
    Serial.println("No WiFi configuration found.");
    return false;
  }

  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, psk);
  unsigned long connect_start_ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - connect_start_ms > CONNECT_TIMEOUT_MS) {
      Serial.println("Connection failed!");
      return false;
    }
    delay(500);
  }

  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  return true;
}

bool configureWifi() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);

  char ssid[SSID_MAX] = {0};
  char psk[PSK_MAX] = {0};

  Serial.print("Enter WiFi SSID: ");
  Serial.setTimeout(10000);
  String ssid_s = Serial.readString();
  Serial.print(ssid_s);
  if (ssid_s.length() <= 1) {
    return false;
  }
  if (ssid_s.length() - 1 > SSID_MAX) {
    Serial.println("SSID is too long!");
    return false;
  }
  // Truncate trailing newline character.
  memcpy(ssid, ssid_s.c_str(), ssid_s.length() - 1);
  
  Serial.print("Enter WiFi PSK: ");
  Serial.setTimeout(30000);
  String psk_s = Serial.readString();
  Serial.print(psk_s);
  if (psk_s.length() <= 1) {
    return false;
  }
  if (psk_s.length() - 1 > PSK_MAX) {
    Serial.println("PSK is too long!");
    return false;
  }
  // Truncate trailing newline character.
  memcpy(psk, psk_s.c_str(), psk_s.length() - 1);

  EEPROM.put(0, ssid);
  EEPROM.put(SSID_MAX, psk);
  return EEPROM.commit();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(IR_OUTPUT_PIN, OUTPUT);
  digitalWrite(IR_OUTPUT_PIN, IR_OUTPUT_INACTIVE);
  analogWriteFreq(IR_FREQ);

  Serial.begin(115200);
  delay(500);

  EEPROM.begin(CONFIG_SIZE);
  while (!connectToWifi()) {
    if (!configureWifi()) {
      Serial.println("WiFi configuration failed!");
      delay(1000);
    }
  }

  // TODO: Initialize HTTP server
}

void loop() {}
