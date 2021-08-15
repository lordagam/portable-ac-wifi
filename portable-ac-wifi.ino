#define IR_PIN 14
#define IR_PIN_INACTIVE LOW
#define IR_FREQ 38000

#define PWM_UNIFORM 128
#define PWM_OFF 0

#define SIGNAL_LOW_US 585
#define SIGNAL_HIGH_US 1700
#define SIGNAL_ADJUST_US 50

constexpr int header_sequence_us[] = {
  9100, 4500,
  SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US,
  SIGNAL_HIGH_US,
  SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US,
  SIGNAL_HIGH_US,
  SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US, SIGNAL_LOW_US,
};

#define MODE_FAN_LOW bit(0)
#define MODE_FAN_MEDIUM bit(1)
#define MODE_FAN_HIGH bit(2)
#define MODE_COOL bit(3)
#define MODE_DEHUM bit(5)
#define MODE_FAN_ONLY bit(6)

uint8_t mode_flags = MODE_COOL | MODE_FAN_HIGH;
bool power_on = true;
bool deg_f = true;
bool timer_on = false;
uint8_t thermostat = 70;

void sendBit(bool b) {
  analogWrite(IR_PIN, PWM_OFF);
  digitalWrite(IR_PIN, IR_PIN_INACTIVE);
  if (b) {
    delayMicroseconds(SIGNAL_HIGH_US - SIGNAL_ADJUST_US);
  } else {
    delayMicroseconds(SIGNAL_LOW_US - SIGNAL_ADJUST_US);
  }
  analogWrite(IR_PIN, PWM_UNIFORM);
  delayMicroseconds(SIGNAL_LOW_US - SIGNAL_ADJUST_US);
}

void sendByte(uint8_t b) {
  for (int i = 0; i < 8; i++) {
    sendBit(b & bit(i));
  }
}

void sendSettings() {
  bool signal_active = true;
  for (int i = 0; i < sizeof(header_sequence_us) / sizeof(int); i++) {
    if (signal_active) {
      analogWrite(IR_PIN, PWM_UNIFORM);
    } else {
      analogWrite(IR_PIN, PWM_OFF);
      digitalWrite(IR_PIN, IR_PIN_INACTIVE);
    }
    signal_active = !signal_active;
    delayMicroseconds(header_sequence_us[i] - SIGNAL_ADJUST_US);
  }
  sendByte(mode_flags);
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
  sendBit(deg_f);
  sendBit(timer_on);
  sendBit(power_on);
  sendByte(thermostat);
  analogWrite(IR_PIN, PWM_OFF);
  digitalWrite(IR_PIN, IR_PIN_INACTIVE);
}

void setup() {
  pinMode(IR_PIN, OUTPUT);
  digitalWrite(IR_PIN, IR_PIN_INACTIVE);
  analogWriteFreq(IR_FREQ);

  Serial.begin(115200);
  delay(500);

  // TODO: Initialize WiFi
  // TODO: Initialize HTTP server
}

void loop() {}
