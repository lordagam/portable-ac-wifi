#include <stdint.h>

#define bit(x) (1 << (x))

void analogWrite(int pin, int val);
void digitalWrite(int pin, int val);

void delayMicroseconds(uint64_t us);
