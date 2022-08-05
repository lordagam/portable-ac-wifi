#include "src/ac-settings-encoder.h"

#include <stdint.h>
#include <vector>

#include "gmock/gmock.h"
#include "gmock-global/gmock-global.h"
#include "gtest/gtest.h"

MOCK_GLOBAL_FUNC2(analogWrite, void(int, int));
MOCK_GLOBAL_FUNC2(digitalWrite, void(int, int));
MOCK_GLOBAL_FUNC1(delayMicroseconds, void(uint32_t));
MOCK_GLOBAL_FUNC1(delay, void(uint32_t));
MOCK_GLOBAL_FUNC0(micros, uint32_t());

namespace {

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::ElementsAre;
using ::testing::Invoke;
using ::testing::IsEmpty;

constexpr int kTestPin = 2;
constexpr int kInactive = 1;

class ACSettingsEncoderTest : public testing::Test {
 protected:
  ACSettingsEncoderTest() : encoder(kTestPin, kInactive),
      clock_us_(0),
      pin_value_(0),
      last_edge_us_(0) {}

  void registerMocksAndSend() {
    EXPECT_GLOBAL_CALL(delayMicroseconds, delayMicroseconds(_))
        .Times(AnyNumber())
        .WillRepeatedly(Invoke([&](uint32_t us) {
          clock_us_ += us;
        }));
    EXPECT_GLOBAL_CALL(delay, delay(_))
        .Times(AnyNumber());
    EXPECT_GLOBAL_CALL(micros, micros())
        .Times(AnyNumber())
        .WillRepeatedly(Invoke([&]() {
          return clock_us_;
        }));
    EXPECT_GLOBAL_CALL(analogWrite, analogWrite(kTestPin, _))
        .Times(AnyNumber())
        .WillRepeatedly(Invoke([&](int, int value) {
          // Alternate between 0 and 50% PWM.
          EXPECT_EQ(pin_value_, value > 0 ? 0 : 128);
          pin_value_ = value;
          durations_us.push_back(clock_us_ - last_edge_us_);
          last_edge_us_ = clock_us_;
        }));
    EXPECT_GLOBAL_CALL(
        digitalWrite, digitalWrite(kTestPin, kInactive))
            .Times(AnyNumber())
            .WillRepeatedly(Invoke([&](int, int) {
              // digitalWrite only used when PWM is off.
              EXPECT_EQ(pin_value_, 0);
            }));

    encoder.send();
  }

  std::vector<uint32_t> getDurations(int n) {
    EXPECT_GE(durations_us.size(), n);
    std::vector<uint32_t> result(
        durations_us.begin(), durations_us.begin() + n);
    durations_us.erase(durations_us.begin(), durations_us.begin() + n);
    return result;
  }

  ACSettingsEncoder encoder;
  std::vector<uint32_t> durations_us;

 private:
  uint32_t clock_us_;
  int pin_value_;
  uint32_t last_edge_us_;
};

TEST_F(ACSettingsEncoderTest, DefaultSettings) {
  EXPECT_EQ(encoder.getFanSpeed(), ACSettingsEncoder::kHigh);
  EXPECT_EQ(encoder.getMode(), ACSettingsEncoder::kCool);
  EXPECT_FALSE(encoder.isPowerOn());
  EXPECT_FALSE(encoder.isTimerOn());
  EXPECT_EQ(encoder.getThermostatInF(), 70);
}

TEST_F(ACSettingsEncoderTest, ModifiesFanSpeed) {
  encoder.setFanSpeed(ACSettingsEncoder::kLow);
  EXPECT_EQ(encoder.getFanSpeed(), ACSettingsEncoder::kLow);
  EXPECT_EQ(encoder.getMode(), ACSettingsEncoder::kCool);
}

TEST_F(ACSettingsEncoderTest, ModifiesFanSpeedAndMode) {
  encoder.setFanSpeed(ACSettingsEncoder::kLow);
  encoder.setMode(ACSettingsEncoder::kFanOnly);
  EXPECT_EQ(encoder.getFanSpeed(), ACSettingsEncoder::kLow);
  EXPECT_EQ(encoder.getMode(), ACSettingsEncoder::kFanOnly);
}

TEST_F(ACSettingsEncoderTest, ModifiesPower) {
  encoder.powerOn();
  EXPECT_TRUE(encoder.isPowerOn());
  encoder.powerOff();
  EXPECT_FALSE(encoder.isPowerOn());
}

TEST_F(ACSettingsEncoderTest, ModifiesTimer) {
  encoder.timerOn();
  EXPECT_TRUE(encoder.isTimerOn());
  encoder.timerOff();
  EXPECT_FALSE(encoder.isTimerOn());
}

TEST_F(ACSettingsEncoderTest, ModifiesThermostat) {
  encoder.setThermostatInF(55);
  EXPECT_EQ(encoder.getThermostatInF(), 55);
}

TEST_F(ACSettingsEncoderTest, SendsHeader) {
  registerMocksAndSend();

  EXPECT_THAT(
      getDurations(22),
      ElementsAre(
          0, 9100, 4500, 585, 585, 585, 585, 585, 585, 585, 1700, 585, 585, 585,
          585, 585, 1700, 585, 585, 585, 585, 585));
}

TEST_F(ACSettingsEncoderTest, SendsDefaultValues) {
  registerMocksAndSend();

  for (int i = 0; i < 2; i++) {
    // Skip header
    getDurations(22);
    // Fan/mode flags
    EXPECT_THAT(
        getDurations(16),
        ElementsAre(
            585, 585, 585, 585, 1700, 585, 1700, 585, 585, 585, 585, 585, 585,
            585, 585, 585));
    // Power/degrees/timer/power
    EXPECT_THAT(
        getDurations(14),
        ElementsAre(
            1700, 585, 585, 585, 585, 585, 1700, 585, 1700, 585, 585, 585, 585,
            585));
    // Thermostat
    EXPECT_THAT(
        getDurations(16),
        ElementsAre(
            585, 585, 1700, 585, 1700, 585, 585, 585, 585, 585, 585, 585, 1700,
            585, 585, 585));
  }
  EXPECT_THAT(durations_us, IsEmpty());
}

TEST_F(ACSettingsEncoderTest, SendsWithThermostatZero) {
  encoder.setThermostatInF(0);
  registerMocksAndSend();

  for (int i = 0; i < 2; i++) {
    // Skip header
    getDurations(22);
    // Fan/mode flags
    EXPECT_THAT(
        getDurations(16),
        ElementsAre(
            585, 585, 585, 585, 1700, 585, 1700, 585, 585, 585, 585, 585, 585,
            585, 585, 585));
    // Power/degrees/timer/power
    EXPECT_THAT(
        getDurations(14),
        ElementsAre(
            1700, 585, 585, 585, 585, 585, 1700, 585, 1700, 585, 585, 585, 585,
            585));
    // Thermostat
    EXPECT_THAT(
        getDurations(16),
        ElementsAre(
            585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585,
            585, 585));
  }
  EXPECT_THAT(durations_us, IsEmpty());
}

TEST_F(ACSettingsEncoderTest, SendsWithPowerOn) {
  encoder.powerOn();
  registerMocksAndSend();

  for (int i = 0; i < 2; i++) {
    // Skip header
    getDurations(22);
    // Fan/mode flags
    EXPECT_THAT(
        getDurations(16),
        ElementsAre(
            585, 585, 585, 585, 1700, 585, 1700, 585, 585, 585, 585, 585, 585,
            585, 585, 585));
    // Power/degrees/timer/power
    EXPECT_THAT(
        getDurations(14),
        ElementsAre(
            585, 585, 585, 585, 1700, 585, 585, 585, 1700, 585, 585, 585, 1700,
            585));
    // Thermostat
    EXPECT_THAT(
        getDurations(16),
        ElementsAre(
            585, 585, 1700, 585, 1700, 585, 585, 585, 585, 585, 585, 585, 1700,
            585, 585, 585));
  }
  EXPECT_THAT(durations_us, IsEmpty());
}

TEST_F(ACSettingsEncoderTest, SendsWithFanLow) {
  encoder.setFanSpeed(ACSettingsEncoder::kLow);
  registerMocksAndSend();

  for (int i = 0; i < 2; i++) {
    // Skip header
    getDurations(22);
    // Fan/mode flags
    EXPECT_THAT(
        getDurations(16),
        ElementsAre(
            1700, 585, 585, 585, 585, 585, 1700, 585, 585, 585, 585, 585, 585,
            585, 585, 585));
    // Power/degrees/timer/power
    EXPECT_THAT(
        getDurations(14),
        ElementsAre(
            1700, 585, 585, 585, 585, 585, 1700, 585, 1700, 585, 585, 585, 585,
            585));
    // Thermostat
    EXPECT_THAT(
        getDurations(16),
        ElementsAre(
            585, 585, 1700, 585, 1700, 585, 585, 585, 585, 585, 585, 585, 1700,
            585, 585, 585));
  }
  EXPECT_THAT(durations_us, IsEmpty());
}

} // namespace
