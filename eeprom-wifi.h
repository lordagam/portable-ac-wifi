#ifndef __EEPROM_WIFI_H
#define __EEPROM_WIFI_H

// Attempts to connect to WiFi from stored credentials.
// On failure, prompts for new credentials via serial and retries.
// This function blocks until WiFi configuration/connection is successful.
void initEEPROMWiFi();

#endif
