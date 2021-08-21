#include "ac-settings-encoder.h"

#define IR_OUTPUT_PIN 14
#define IR_OUTPUT_INACTIVE LOW
#define IR_FREQ 38000

ACSettingsEncoder ac_settings(IR_OUTPUT_PIN, IR_OUTPUT_INACTIVE);

void setup() {
  pinMode(IR_OUTPUT_PIN, OUTPUT);
  digitalWrite(IR_OUTPUT_PIN, IR_OUTPUT_INACTIVE);
  analogWriteFreq(IR_FREQ);

  Serial.begin(115200);
  delay(500);

  // TODO: Initialize WiFi
  // TODO: Initialize HTTP server
}

void loop() {}
