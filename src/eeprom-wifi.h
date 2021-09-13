#ifndef __EEPROM_WIFI_H
#define __EEPROM_WIFI_H

#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>

// Attempts to connect to WiFi from stored credentials.
// On failure, prompts for new credentials via serial and retries.
// This function blocks until WiFi configuration/connection is successful.
void initEEPROMWiFi(
    EEPROMClass& = EEPROM, ESP8266WiFiClass& = WiFi, HardwareSerial& = Serial);

#endif
