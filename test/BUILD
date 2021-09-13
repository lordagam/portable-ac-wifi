cc_test(
  name = "ac-settings-encoder-test",
  srcs = ["ac-settings-encoder-test.cpp"],
  deps = [
    "//src:ac-settings-encoder",
    "@gmock-global//:gmock-global",
    "@gtest//:gtest",
    "@gtest//:gtest_main",
  ],
)
cc_test(
  name = "eeprom-wifi-test",
  srcs = ["eeprom-wifi-test.cpp"],
  deps = [
    "//src:eeprom-wifi",
    "//test/mock:EEPROM",
    "//test/mock:ESP8266WiFi",
    "@gmock-global//:gmock-global",
    "@gtest//:gtest",
    "@gtest//:gtest_main",
  ],
)
