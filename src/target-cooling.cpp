#include "target-cooling.h"

#include <Arduino.h>

#include "ac-settings-encoder.h"

namespace {

constexpr float kToleranceInF = 1.0;

} // namespace

void TargetCooling::process(float ambient_temp_f) {
  if (!is_enabled) {
    return;
  }

  float average_f = averageAmbientInF(ambient_temp_f);
  bool is_cooling = ac.getThermostatInF() == kThermostatMinF;

  bool updated = false;
  if (average_f > thermostat_f + kToleranceInF && !is_cooling) {
    ac.setMode(ACSettingsEncoder::Mode::kCool);
    ac.powerOn();
    ac.setThermostatInF(kThermostatMinF);
    updated = true;
  }
  if (average_f < thermostat_f - kToleranceInF && is_cooling) {
    ac.setThermostatInF(kThermostatMaxF);
    updated = true;
  }

  if (updated) {
    ac.send();
  }
}

float TargetCooling::averageAmbientInF(float sample_f) {
  if (sample_write_index < 0) {
    for (int i = 0; i < kNumSamples; i++) {
      ambient_samples_f[i] = sample_f;
    }
    sample_write_index = 0;
  }

  ambient_samples_f[sample_write_index] = sample_f;
  sample_write_index = (sample_write_index + 1) % kNumSamples;

  float result = 0;
  for (int i = 0; i < kNumSamples; i++) {
    result += ambient_samples_f[i] / kNumSamples;
  }
  return result;
}
