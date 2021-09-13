package(default_visibility = ["//visibility:public"])

cc_library(
  name = "ac-settings-encoder",
  testonly = True,
  srcs = ["ac-settings-encoder.cpp"],
  hdrs = ["ac-settings-encoder.h"],
  deps = [
    "//test/mock:Arduino",
  ],
)
cc_library(
  name = "eeprom-wifi",
  testonly = True,
  srcs = ["eeprom-wifi.cpp"],
  hdrs = ["eeprom-wifi.h"],
  deps = [
    "//test/mock:Arduino",
    "//test/mock:EEPROM",
    "//test/mock:ESP8266WiFi",
  ],
)
