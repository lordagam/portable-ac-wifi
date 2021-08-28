#ifndef __SETTINGS_HANDLER_H
#define __SETTINGS_HANDLER_H

#include <ESP8266WebServer.h>
#include "ac-settings-encoder.h"

void handleSettings(ESP8266WebServer& server, ACSettingsEncoder& ac);

#endif