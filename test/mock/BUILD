package(default_visibility = ["//visibility:public"])

cc_library(
  name = "ac-settings-encoder",
  testonly = True,
  hdrs = ["ac-settings-encoder.h"],
  deps = [
    "//src:ac-settings-encoder",
    "@gtest//:gtest",
  ],
)
cc_library(
  name = "Arduino",
  testonly = True,
  hdrs = ["//test/mock:Arduino.h"],
  strip_include_prefix = "",
  deps = [
    "@gtest//:gtest",
  ],
)
cc_library(
  name = "EEPROM",
  testonly = True,
  hdrs = ["//test/mock:EEPROM.h"],
  strip_include_prefix = "",
  deps = [
    "@gtest//:gtest",
  ],
)
cc_library(
  name = "ESP8266WebServer",
  testonly = True,
  hdrs = ["//test/mock:ESP8266WebServer.h"],
  strip_include_prefix = "",
  deps = [
    ":Arduino",
    "@gtest//:gtest",
  ],
)
cc_library(
  name = "ESP8266WiFi",
  testonly = True,
  hdrs = ["//test/mock:ESP8266WiFi.h"],
  strip_include_prefix = "",
  deps = [
    "@gtest//:gtest",
  ],
)
