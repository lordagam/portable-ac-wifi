#include "eeprom-wifi.h"

#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>

namespace {

constexpr int kSSIDMax = 32;
constexpr int kPSKMax = 64;
constexpr int kConfigSize = (kSSIDMax + kPSKMax);

constexpr unsigned long kConnectTimeoutMs = 30000;

bool connectToWifi() {
  char ssid[kSSIDMax + 1] = {0};
  char psk[kPSKMax + 1] = {0};
  memcpy(ssid, &EEPROM[0], kSSIDMax);
  memcpy(psk, &EEPROM[kSSIDMax], kPSKMax);
  if (!ssid[0]) {
    Serial.println("No WiFi configuration found.");
    return false;
  }

  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, psk);
  unsigned long connect_start_ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - connect_start_ms > kConnectTimeoutMs) {
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

  char ssid[kSSIDMax] = {0};
  char psk[kPSKMax] = {0};

  Serial.print("Enter WiFi SSID: ");
  Serial.setTimeout(10000);
  String ssid_s = Serial.readString();
  Serial.print(ssid_s);
  if (ssid_s.length() <= 1) {
    return false;
  }
  if (ssid_s.length() - 1 > kSSIDMax) {
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
  if (psk_s.length() - 1 > kPSKMax) {
    Serial.println("PSK is too long!");
    return false;
  }
  // Truncate trailing newline character.
  memcpy(psk, psk_s.c_str(), psk_s.length() - 1);

  EEPROM.put(0, ssid);
  EEPROM.put(kSSIDMax, psk);
  return EEPROM.commit();
}

} // namespace

void initEEPROMWiFi() {
  EEPROM.begin(kConfigSize);
  while (!connectToWifi()) {
    if (!configureWifi()) {
      Serial.println("WiFi configuration failed!");
      delay(1000);
    }
  }
}
