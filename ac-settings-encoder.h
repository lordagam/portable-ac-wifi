#ifndef __AC_SETTINGS_ENCODER_H
#define __AC_SETTINGS_ENCODER_H

#include <stdint.h>

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

  ACSettingsEncoder(uint8_t ir_output_pin, uint8_t ir_output_inactive);

  FanSpeed getFanSpeed();
  void setFanSpeed(FanSpeed speed);

  Mode getMode();
  void setMode(Mode mode);

  bool isPowerOn() { return power_on; }
  void powerOn() { power_on = true; }
  void powerOff() { power_on = false; }

  bool isTimerOn() { return timer_on; }
  void timerOn() { timer_on = true; }
  void timerOff() { timer_on = false; }

  uint8_t getThermostatInF() { return thermostat; }
  void setThermostatInF(uint8_t degrees_f) { thermostat = degrees_f; }

  // Transmit current settings via infrared signal.
  void send();

 private:
  // Arduino PWM output pin for generating infrared signal.
  uint8_t ir_output_pin;
  // Inactive state for the infrared output pin.
  uint8_t ir_output_inactive;
  // Fan and cooling mode.
  uint8_t fan_mode_flags;
  // Enables unit main power.
  bool power_on = false;
  // When true, the unit will shut off after 8 hours of operation.
  bool timer_on = false;
  // Target temperature in degrees Fahrenheit.
  uint8_t thermostat = 70;

  void sendBit(bool b);
  void sendByte(uint8_t b);
};

#endif
