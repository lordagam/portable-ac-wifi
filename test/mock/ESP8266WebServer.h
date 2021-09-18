#ifndef __TEST_MOCK_ESP8266WEBSERVER_H
#define __TEST_MOCK_ESP8266WEBSERVER_H

#include "Arduino.h"
#include "gmock/gmock.h"

#define HTTP_POST 123

class MockESP8266WebServer {
 public:
  MOCK_METHOD0(method, int());
  MOCK_METHOD3(send, void(int, const char*, const char*));
  MOCK_METHOD1(arg, String(const char*));
};

typedef MockESP8266WebServer ESP8266WebServer;

#endif
