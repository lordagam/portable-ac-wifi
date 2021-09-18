#ifndef __TEST_MOCK_AC_SETTINGS_ENCODER_H
#define __TEST_MOCK_AC_SETTINGS_ENCODER_H

#include "src/ac-settings-encoder.h"
#include "gmock/gmock.h"

class MockACSettingsEncoder : public ACSettingsEncoder {
 public:
  MockACSettingsEncoder() : ACSettingsEncoder(0, 0) {}
  MOCK_METHOD(void, send, (), (override));
};

#endif
