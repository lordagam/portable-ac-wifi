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
cc_test(
  name = "settings-handler-test",
  srcs = ["settings-handler-test.cpp"],
  deps = [
    "//src:settings-handler",
    "//test/mock:ac-settings-encoder",
    "//test/mock:ESP8266WebServer",
    "@gmock-global//:gmock-global",
    "@gtest//:gtest",
    "@gtest//:gtest_main",
  ],
)
