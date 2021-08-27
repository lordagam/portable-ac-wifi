#include "settings-handler.h"

#include <string>

#include <ESP8266WebServer.h>

#include "ac-settings-encoder.h"

namespace {

constexpr char kJsonContentType[] = "text/json";
constexpr char kCoolString[] = "cool";
constexpr char kDehumString[] = "dehum";
constexpr char kFanOnlyString[] = "fan_only";
constexpr char kLowString[] = "low";
constexpr char kMediumString[] = "medium";
constexpr char kHighString[] = "high";
constexpr char kTrueString[] = "true";
constexpr char kFalseString[] = "false";
constexpr int kThermostatMinF = 50;
constexpr int kThermostatMaxF = 100;

const char* modeToString(ACSettingsEncoder::Mode mode) {
  switch (mode) {
    case ACSettingsEncoder::kCool:
      return kCoolString;
    case ACSettingsEncoder::kDehum:
      return kDehumString;
    case ACSettingsEncoder::kFanOnly:
      return kFanOnlyString;
  }
  return "";
}

ACSettingsEncoder::Mode stringToMode(const char* mode) {
  if (!strcmp(mode, kDehumString)) {
    return ACSettingsEncoder::kDehum;
  }
  if (!strcmp(mode, kFanOnlyString)) {
    return ACSettingsEncoder::kFanOnly;
  }
  return ACSettingsEncoder::kCool;
}

const char* fanSpeedToString(ACSettingsEncoder::FanSpeed fan) {
  switch (fan) {
    case ACSettingsEncoder::kLow:
      return kLowString;
    case ACSettingsEncoder::kMedium:
      return kMediumString;
    case ACSettingsEncoder::kHigh:
      return kHighString;
  }
  return "";
}

ACSettingsEncoder::FanSpeed stringToFanSpeed(const char* fan) {
  if (!strcmp(fan, kLowString)) {
    return ACSettingsEncoder::kLow;
  }
  if (!strcmp(fan, kMediumString)) {
    return ACSettingsEncoder::kMedium;
  }
  return ACSettingsEncoder::kHigh;
}

void handleSettingsUpdate(ESP8266WebServer& server, ACSettingsEncoder& ac) {
  ac.setFanSpeed(stringToFanSpeed(server.arg("fan").c_str()));
  ac.setMode(stringToMode(server.arg("mode").c_str()));
  if (server.arg("timer").equals(kTrueString)) {
    ac.timerOn();
  } else {
    ac.timerOff();
  }
  if (server.arg("power").equals(kTrueString)) {
    ac.powerOn();
  } else {
    ac.powerOff();
  }
  int thermostatInF = server.arg("thermostatInF").toInt();
  thermostatInF = min(thermostatInF, kThermostatMaxF);
  thermostatInF = max(thermostatInF, kThermostatMinF);
  ac.setThermostatInF(thermostatInF);
  ac.send();
  server.send(200);
}

} // namespace

void handleSettings(ESP8266WebServer& server, ACSettingsEncoder& ac) {
  if (server.method() == HTTP_POST) {
    handleSettingsUpdate(server, ac);
    return;
  }
  std::string json = "{";
  char tmp[32];
  snprintf(tmp, sizeof(tmp), "fan:\"%s\",", fanSpeedToString(ac.getFanSpeed()));
  json += tmp;
  snprintf(tmp, sizeof(tmp), "mode:\"%s\",", modeToString(ac.getMode()));
  json += tmp;
  snprintf(tmp, sizeof(tmp), "timer:%s,", ac.isTimerOn() ? kTrueString : kFalseString);
  json += tmp;
  snprintf(tmp, sizeof(tmp), "power:%s,", ac.isPowerOn() ? kTrueString : kFalseString);
  json += tmp;
  snprintf(tmp, sizeof(tmp), "thermostatInF:%d,", ac.getThermostatInF());
  json += tmp;
  json += "}";
  server.send(200, kJsonContentType, json.c_str());
}
