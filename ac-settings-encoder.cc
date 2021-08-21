#include "ac-settings-encoder.h"

namespace {

using ::ACSettingsEncoder::FanSpeed;
using ::ACSettingsEncoder::Mode;

constexpr kIRPinInactive = LOW;

constexpr kPWMUniform = 128;
constexpr kPWMOff = 0;

constexpr kSignalLow_us = 585;
constexpr kSignalHigh_us = 1700;
constexpr kSignalAdjust_us = 50;

constexpr kFanLow = bit(0);
constexpr kFanMedium = bit(1);
constexpr kFanHigh = bit(2);
constexpr kFanMask = kFanLow | kFanMedium | kFanHigh;
constexpr kModeCool = bit(3);
constexpr kModeDehum = bit(5);
constexpr kModeFanOnly = bit(6);
constexpr kModeMask = kModeCool | kModeDehum | kModeFanOnly;

constexpr int kHeaderSequence_us[] = {
  9100, 4500,
  kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us,
  kSignalHigh_us,
  kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us,
  kSignalHigh_us,
  kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us,
};

} // namespace

ACSettingsEncoder::ACSettingsEncoder(uint8_t ir_output_pin) : ir_output_pin(ir_output_pin) {
  fan_mode_flags = kFanHigh | kModeCool;
}

FanSpeed ACSettingsEncoder::getFanSpeed() {
  switch (fan_mode_flags & kFanMask) {
    case kFanLow:
      return FanSpeed::kLow;
    case kFanMedium:
      return FanSpeed::kMedium;
    default:
      return FanSpeed::kHigh;
  }
}
void ACSettingsEncoder::setFanSpeed(FanSpeed speed) {
  fan_mode_flags = fan_mode_flags & ~kFanMask;
  switch (speed) {
    case kLow:
      fan_mode_flags |= kFanLow;
      break;
    case kMedium:
      fan_mode_flags |= kFanMedium;
      break;
    default:
      fan_mode_flags |= kFanHigh;
      break;
  }
}

Mode ACSettingsEncoder::getMode() {
  switch (fan_mode_flags & kModeMask) {
    case kModeDehum:
      return Mode::kDehum;
    case kModeFanOnly:
      return Mode::kFanOnly;
    default:
      return Mode::kCool;
  }
}
void ACSettingsEncoder::setMode(Mode mode) {
  fan_mode_flags = fan_mode_flags & ~kModeMask;
  switch (mode) {
    case kDehum:
      fan_mode_flags |= kModeDehum;
      break;
    case kFanOnly:
      fan_mode_flags |= kModeFanOnly;
      break;
    default:
      fan_mode_flags |= kModeCool;
      break;
  }
}

void ACSettingsEncoder::sendSettings() {
  bool signal_active = true;
  for (int i = 0; i < sizeof(header_sequence_us) / sizeof(int); i++) {
    if (signal_active) {
      analogWrite(ir_output_pin, kPWMUniform);
    } else {
      analogWrite(ir_output_pin, kPWMOff);
      digitalWrite(ir_output_pin, kIRPinInactive);
    }
    signal_active = !signal_active;
    delayMicroseconds(header_sequence_us[i] - kSignalAdjust_us);
  }
  sendByte(fan_mode_flags);
  if (power_on) {
    sendBit(0);
    sendBit(0);
    sendBit(1);
    sendBit(0);
  } else {
    sendBit(1);
    sendBit(0);
    sendBit(0);
    sendBit(1);
  }
  // Thermostat in degrees C not implemented, always use degrees F.
  sendBit(/* thermostat_in_f= */true);
  sendBit(timer_on);
  sendBit(power_on);
  sendByte(thermostat);
  analogWrite(ir_output_pin, kPWMOff);
  digitalWrite(ir_output_pin, kIRPinInactive);
}

void ACSettingsEncoder::sendBit(bool b) {
  analogWrite(ir_output_pin, kPWMOff);
  digitalWrite(ir_output_pin, kIRPinInactive);
  if (b) {
    delayMicroseconds(kSignalHigh_us - kSignalAdjust_us);
  } else {
    delayMicroseconds(kSignalLow_us - kSignalAdjust_us);
  }
  analogWrite(ir_output_pin, kPWMUniform);
  delayMicroseconds(kSignalLow_us - kSignalAdjust_us);
}

void ACSettingsEncoder::sendByte(uint8_t b) {
  for (int i = 0; i < 8; i++) {
    sendBit(b & bit(i));
  }
}
