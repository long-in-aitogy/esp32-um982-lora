#include <Arduino.h>
#include <unity.h>

void setUp(void) {
  ArduinoFakeReset();

  fakeit::When(OverloadedMethod(ArduinoFake(Serial), print, size_t(const char[]))).AlwaysReturn(1);
  fakeit::When(OverloadedMethod(ArduinoFake(Serial), print, size_t(int, int))).AlwaysReturn(1);
  fakeit::When(OverloadedMethod(ArduinoFake(Serial), println, size_t(const char[]))).AlwaysReturn(1);
  fakeit::When(OverloadedMethod(ArduinoFake(Serial), println, size_t(const String&))).AlwaysReturn(1);

  fakeit::When(OverloadedMethod(ArduinoFake(Function), random, long(long))).Return(1234);
  fakeit::When(Method(ArduinoFake(Function), delay)).AlwaysReturn();
}

void tearDown(void) {}

#ifndef NATIVE_BUILD
void setup() {}

void loop() {}
#else

int main() {
  UNITY_BEGIN();
  
  return UNITY_END();
}
#endif