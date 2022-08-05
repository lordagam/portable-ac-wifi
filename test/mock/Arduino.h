#ifndef __TEST_MOCK_ARDUINO_H
#define __TEST_MOCK_ARDUINO_H

#include <stdint.h>
#include <stdlib.h>

#include "gmock/gmock.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define bit(x) (1 << (x))
#define F(str) str
#define LED_BUILTIN 99
#define LOW 88
#define HIGH 77

void analogWrite(int pin, int val);
void digitalWrite(int pin, int val);

static void noInterrupts() {}
static void interrupts() {}

void delayMicroseconds(uint32_t us);
void delay(uint32_t ms);
uint32_t micros();
uint32_t millis();

class FakeString {
 public:
  FakeString() : c_str_("") {}
  FakeString(const char* c_str) : c_str_(c_str) {}

  size_t length() {
    return strlen(c_str_);
  }
  const char* c_str() {
    return c_str_;
  }
  bool equals(const char* str) {
    return !strcmp(c_str_, str);
  }
  int toInt() {
    return atoi(c_str_);
  }

 private:
  const char* c_str_;
};
typedef FakeString String;

#endif
