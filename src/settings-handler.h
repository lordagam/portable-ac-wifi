#ifndef __SETTINGS_HANDLER_H
#define __SETTINGS_HANDLER_H

#include <ESP8266WebServer.h>

#include "ac-settings-encoder.h"

constexpr int kHTTPOK = 200;

void handleSettings(
  ESP8266WebServer& server, ACSettingsEncoder& ac, float ambient_temp_f);

#endif