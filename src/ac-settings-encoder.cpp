#include "ac-settings-encoder.h"

#include <Arduino.h>

namespace {

constexpr unsigned long kRepeatSendDelayMs = 100;

constexpr int kPWMUniform = 128;
constexpr int kPWMOff = 0;

constexpr int kSignalLow_us = 585;
constexpr int kSignalHigh_us = 1700;

constexpr uint8_t kFanLow = bit(0);
constexpr uint8_t kFanMedium = bit(1);
constexpr uint8_t kFanHigh = bit(2);
constexpr uint8_t kFanMask = kFanLow | kFanMedium | kFanHigh;
constexpr uint8_t kModeCool = bit(3);
constexpr uint8_t kModeDehum = bit(5);
constexpr uint8_t kModeFanOnly = bit(6);
constexpr uint8_t kModeMask = kModeCool | kModeDehum | kModeFanOnly;

constexpr int kHeaderSequence_us[] = {
  9100, 4500,
  kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us,
  kSignalHigh_us,
  kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us,
  kSignalHigh_us,
  kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us, kSignalLow_us,
};

} // namespace

ACSettingsEncoder::ACSettingsEncoder(uint8_t ir_output_pin, uint8_t ir_output_inactive) :
      ir_output_pin(ir_output_pin),
      ir_output_inactive(ir_output_inactive) {
  fan_mode_flags = kFanHigh | kModeCool;
}

ACSettingsEncoder::FanSpeed ACSettingsEncoder::getFanSpeed() {
  switch (fan_mode_flags & kFanMask) {
    case kFanLow:
      return FanSpeed::kLow;
    case kFanMedium:
      return FanSpeed::kMedium;
    default:
      return FanSpeed::kHigh;
  }
}
void ACSettingsEncoder::setFanSpeed(ACSettingsEncoder::FanSpeed speed) {
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

ACSettingsEncoder::Mode ACSettingsEncoder::getMode() {
  switch (fan_mode_flags & kModeMask) {
    case kModeDehum:
      return Mode::kDehum;
    case kModeFanOnly:
      return Mode::kFanOnly;
    default:
      return Mode::kCool;
  }
}
void ACSettingsEncoder::setMode(ACSettingsEncoder::Mode mode) {
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

void delayMicrosSinceLast(unsigned long us) {
  static unsigned long target_us = 0;
  unsigned long now_us = micros();
  if (!us) {
    target_us = now_us;
  } else {
    target_us += us;
  }
  if (target_us > now_us) {
    delayMicroseconds(target_us - now_us);
  }
}

void ACSettingsEncoder::send() {
  // Send twice to improve reliability
  sendOnce();
  delay(kRepeatSendDelayMs);
  sendOnce();
}

void ACSettingsEncoder::sendOnce() {
  noInterrupts();
  bool signal_active = true;
  delayMicrosSinceLast(0);
  for (int i = 0; i < sizeof(kHeaderSequence_us) / sizeof(int); i++) {
    if (signal_active) {
      analogWrite(ir_output_pin, kPWMUniform);
    } else {
      analogWrite(ir_output_pin, kPWMOff);
      digitalWrite(ir_output_pin, ir_output_inactive);
    }
    signal_active = !signal_active;
    delayMicrosSinceLast(kHeaderSequence_us[i]);
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
  digitalWrite(ir_output_pin, ir_output_inactive);
  interrupts();
}

void ACSettingsEncoder::sendBit(bool b) {
  analogWrite(ir_output_pin, kPWMOff);
  digitalWrite(ir_output_pin, ir_output_inactive);
  if (b) {
    delayMicrosSinceLast(kSignalHigh_us);
  } else {
    delayMicrosSinceLast(kSignalLow_us);
  }
  analogWrite(ir_output_pin, kPWMUniform);
  delayMicrosSinceLast(kSignalLow_us);
}

void ACSettingsEncoder::sendByte(uint8_t b) {
  for (int i = 0; i < 8; i++) {
    sendBit(b & bit(i));
  }
}
