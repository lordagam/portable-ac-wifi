#include "src/eeprom-wifi.h"

#include <string.h>

#include "Arduino.h"
#include "EEPROM.h"
#include "ESP8266WiFi.h"
#include "gmock/gmock.h"
#include "gmock-global/gmock-global.h"
#include "gtest/gtest.h"

MOCK_GLOBAL_FUNC2(digitalWrite, void(int, int));
MOCK_GLOBAL_FUNC1(delay, void(uint64_t));
MOCK_GLOBAL_FUNC0(millis, uint64_t());

namespace {

using ::testing::_;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::Return;
using ::testing::StrEq;

class EEPROMWiFiTest : public testing::Test {
 protected:
  FakeEEPROM EEPROM;
  MockESP8266WiFi WiFi;
  MockSerial Serial;
};

TEST_F(EEPROMWiFiTest, SuccessfulConnection) {
  ON_GLOBAL_CALL(digitalWrite, digitalWrite(_, _)).WillByDefault(Return());
  uint64_t clock = 0;
  ON_GLOBAL_CALL(millis, millis()).WillByDefault(Invoke([&]() {
    return clock;
  }));
  ON_GLOBAL_CALL(delay, delay(_)).WillByDefault(Invoke([&](uint64_t duration) {
    clock += duration;
  }));
  // 32 characters (max SSID length)
  const char* ssid = "01234567890123456789012345678901";
  // 64 characters (max PSK length)
  const char* psk = "0123456789012345678901234567890123456789012345678901234567"
                    "890123";
  EEPROM.begin(strlen(ssid) + strlen(psk));
  memcpy(&EEPROM[0], ssid, strlen(ssid));
  memcpy(&EEPROM[strlen(ssid)], psk, strlen(psk));
  EXPECT_CALL(WiFi, begin(StrEq(ssid), StrEq(psk)));
  EXPECT_CALL(WiFi, status())
      .WillOnce(Return(WL_PENDING))
      .WillOnce(Return(WL_CONNECTED));

  initEEPROMWiFi(EEPROM, WiFi, Serial);
}

} // namespace
