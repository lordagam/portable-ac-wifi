#ifndef __TARGET_COOLING_H
#define __TARGET_COOLING_H

#include "ac-settings-encoder.h"

constexpr int kNumSamples = 100;

class TargetCooling {
 public:
  TargetCooling(ACSettingsEncoder& ac) : ac(ac) {}

  void setEnabled(bool e) { is_enabled = e; }
  bool isEnabled() { return is_enabled; }

  void setThermostatInF(float t) { thermostat_f = t; }
  float getThermostatInF() { return thermostat_f; }

  void process(float ambient_temp_f);

 private:
  ACSettingsEncoder& ac;
  bool is_enabled = false;
  float thermostat_f = 70;
  float ambient_samples_f[kNumSamples];
  int sample_write_index = -1;

  float averageAmbientInF(float sample_f);
};

#endif
