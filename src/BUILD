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
  name = "settings-handler",
  testonly = True,
  srcs = ["settings-handler.cpp"],
  hdrs = ["settings-handler.h"],
  deps = [
    ":ac-settings-encoder",
    ":target-cooling",
    "//test/mock:ESP8266WebServer",
  ],
)
cc_library(
  name = "target-cooling",
  testonly = True,
  srcs = ["target-cooling.cpp"],
  hdrs = ["target-cooling.h"],
  deps = [
    ":ac-settings-encoder",
    "//test/mock:Arduino",
  ],
)
