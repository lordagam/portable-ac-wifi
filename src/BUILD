cc_library(
  name = "ac-settings-encoder",
  testonly = True,
  srcs = ["ac-settings-encoder.cpp"],
  hdrs = ["ac-settings-encoder.h"],
  deps = [
    "//src/testing:fake-Arduino",
  ],
)

cc_test(
  name = "ac-settings-encoder-test",
  srcs = ["ac-settings-encoder-test.cpp"],
  deps = [
    ":ac-settings-encoder",
    "@gmock-global//:gmock-global",
    "@gtest//:gtest",
    "@gtest//:gtest_main",
  ],
)
