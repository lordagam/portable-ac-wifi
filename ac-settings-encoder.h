#ifndef __AC_SETTINGS_ENCODER_H
#define __AC_SETTINGS_ENCODER_H

class ACSettingsEncoder {
 public:
  enum FanSpeed {
    kLow,
    kMedium,
    kHigh,
  };
  enum Mode {
    kCool,
    kDehum,
    kFanOnly,
  };

  ACSettingsEncoder(uint8_t ir_output_pin) : ir_output_pin(ir_output_pin) {}

  FanSpeed getFanSpeed();
  void setFanSpeed(FanSpeed speed);

  Mode getMode();
  void setMode(Mode mode);

  bool getPowerEnabled() { return power_on; }
  void setPowerEnabled(bool enabled) { power_on = enabled; }

  bool getTimerEnabled() { return timer_on; }
  void setTimerEnabled(bool enabled) { timer_on = enabled; }

  uint8_t getThermostatInF() { return thermostat; }
  void setThermostatInF(uint8_t degrees_f) { thermostat = degrees_f; }

  // Transmit current settings via infrared signal.
  void sendSettings();

 private:
  // Arduino PWM output pin for generating infrared signal.
  uint8_t ir_output_pin;
  // Fan and cooling mode.
  uint8_t fan_mode_flags;
  // Enables unit main power.
  bool power_on = true;
  // When true, the unit will shut off after 8 hours of operation.
  bool timer_on = false;
  // Target temperature in degrees Fahrenheit.
  uint8_t thermostat = 70;

  void sendBit(bool b);
  void sendByte(uint8_t b);
};

#endif
