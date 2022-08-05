#include "src/settings-handler.h"

#include <stdint.h>
#include <vector>

#include "mock/ac-settings-encoder.h"
#include "gmock/gmock.h"
#include "gmock-global/gmock-global.h"
#include "gtest/gtest.h"

// Only necessary due to dependency on ACSettingsEncoder.
MOCK_GLOBAL_FUNC2(analogWrite, void(int, int));
MOCK_GLOBAL_FUNC2(digitalWrite, void(int, int));
MOCK_GLOBAL_FUNC1(delay, void(uint32_t));
MOCK_GLOBAL_FUNC1(delayMicroseconds, void(uint32_t));
MOCK_GLOBAL_FUNC0(micros, uint32_t());

namespace {

using ::testing::_;
using ::testing::StrEq;
using ::testing::Return;

TEST(HandleSettingsTest, ReturnsCurrentSettings) {
  MockESP8266WebServer server;
  MockACSettingsEncoder encoder;
  encoder.powerOn();
  encoder.setMode(ACSettingsEncoder::kFanOnly);
  encoder.setFanSpeed(ACSettingsEncoder::kLow);
  encoder.setThermostatInF(123);

  EXPECT_CALL(server, method()).WillOnce(Return(HTTP_POST + 1));

  EXPECT_CALL(encoder, send()).Times(0);
  EXPECT_CALL(
    server,
    send(200, StrEq("application/json"),
        StrEq(
          "{\"ambient\":\"65.500000\","
          "\"fan\":\"low\","
          "\"mode\":\"fan_only\","
          "\"timer\":false,"
          "\"power\":true,"
          "\"thermostatInF\":123}")));

  handleSettings(server, encoder, /*ambient_temp_f=*/65.5);
}

TEST(HandleSettingsTest, UpdatesSettingsAndCallsSend) {
  MockESP8266WebServer server;
  MockACSettingsEncoder encoder;

  EXPECT_CALL(server, method()).WillOnce(Return(HTTP_POST));
  EXPECT_CALL(server, arg(StrEq("fan")))
    .WillOnce(Return(String("medium")));
  EXPECT_CALL(server, arg(StrEq("mode")))
    .WillOnce(Return(String("dehum")));
  EXPECT_CALL(server, arg(StrEq("timer")))
    .WillOnce(Return(String("false")));
  EXPECT_CALL(server, arg(StrEq("power")))
    .WillOnce(Return(String("true")));
  EXPECT_CALL(server, arg(StrEq("thermostatInF")))
    .WillOnce(Return(String("55")));

  EXPECT_GLOBAL_CALL(delay, delay(_));
  EXPECT_CALL(encoder, send()).Times(2);
  EXPECT_CALL(
    server,
    send(200, StrEq("application/json"),
        StrEq(
          "{\"ambient\":\"0.000000\","
          "\"fan\":\"medium\","
          "\"mode\":\"dehum\","
          "\"timer\":false,"
          "\"power\":true,"
          "\"thermostatInF\":55}")));

  handleSettings(server, encoder, /*ambient_temp_f=*/0);

  EXPECT_EQ(encoder.getFanSpeed(), ACSettingsEncoder::kMedium);
  EXPECT_EQ(encoder.getMode(), ACSettingsEncoder::kDehum);
  EXPECT_FALSE(encoder.isTimerOn());
  EXPECT_TRUE(encoder.isPowerOn());
  EXPECT_EQ(encoder.getThermostatInF(), 55);
}

} // namespace
