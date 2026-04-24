#include <Arduino.h>
#include <unity.h>

#include "functions/MQTT_Manager.h"
#include "Prog_Config.h"

extern int connectMQTT();

void setUp(void) {
  mqtt.mockReset();
}

void tearDown(void) {}

void test_setupMQTT_sets_server_and_callback(void) {
  int rc = setupMQTT();

  TEST_ASSERT_EQUAL(0, rc);
  TEST_ASSERT_EQUAL_STRING(MQTT_SERVER, mqtt.server_.c_str());
  TEST_ASSERT_EQUAL_UINT16(MQTT_PORT, mqtt.port_);
}

void test_connectMQTT_subscribes_when_connect_success(void) {
  mqtt.mockSetConnectResult(true);

  int rc = connectMQTT();

  TEST_ASSERT_EQUAL(0, rc);
  TEST_ASSERT_TRUE(mqtt.connected());
  TEST_ASSERT_EQUAL(1, mqtt.connectCallCount_);
  TEST_ASSERT_EQUAL(1, mqtt.subscribeCallCount_);
  TEST_ASSERT_EQUAL_STRING(TOPIC_SUB_CMD, mqtt.lastSubscribedTopic_.c_str());
}

void test_publishData_routes_to_gga_topic(void) {
  mqtt.mockSetConnected(true);

  int rc = publishData("$GGA,abc", true);

  TEST_ASSERT_EQUAL(0, rc);
  TEST_ASSERT_EQUAL(1, mqtt.publishCallCount_);
  TEST_ASSERT_EQUAL_STRING(TOPIC_PUB_DATA_GGA, mqtt.lastPublishedTopic_.c_str());
  TEST_ASSERT_EQUAL_STRING("$GGA,abc", mqtt.lastPublishedPayload_.c_str());
}

void test_publishData_returns_error_when_disconnected(void) {
  mqtt.mockSetConnected(false);

  int rc = publishData("payload", false);

  TEST_ASSERT_EQUAL(-1, rc);
  TEST_ASSERT_EQUAL(0, mqtt.publishCallCount_);
}

#ifndef NATIVE_BUILD
void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_setupMQTT_sets_server_and_callback);
  RUN_TEST(test_connectMQTT_subscribes_when_connect_success);
  RUN_TEST(test_publishData_routes_to_gga_topic);
  RUN_TEST(test_publishData_returns_error_when_disconnected);
  UNITY_END();
}

void loop() {}
#else
int main() {
  UNITY_BEGIN();
  RUN_TEST(test_setupMQTT_sets_server_and_callback);
  RUN_TEST(test_connectMQTT_subscribes_when_connect_success);
  RUN_TEST(test_publishData_routes_to_gga_topic);
  RUN_TEST(test_publishData_returns_error_when_disconnected);
  return UNITY_END();
}
#endif