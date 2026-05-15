
#include <unity.h>
#include <Arduino.h>
#include "hardware/Lora_handler.h"

/*=========== TESTS ============*/
void test_loraSetup_success() {
    Serial.println("Testing loraSetup for successful initialization...");
    int result = loraSetup();
    TEST_ASSERT_EQUAL(0, result);
}

void test_loraReceive_noPacket() {
    Serial.println("Testing loraReceive with no incoming packets...");
    int result = loraReceive();
    TEST_ASSERT_EQUAL(0, result);
}

void test_OnRxDone() {
    Serial.println("Testing OnRxDone callback with sample data...");
    uint8_t samplePayload[] = "Hello, LoRa!";
    uint16_t sampleSize = sizeof(samplePayload) - 1; // exclude null terminator
    int16_t sampleRssi = -70;
    int8_t sampleSnr = 7;

    OnRxDone(samplePayload, sampleSize, sampleRssi, sampleSnr);

    TEST_ASSERT_EQUAL(sampleSize, rxSize);
    TEST_ASSERT_EQUAL_STRING("Hello, LoRa!", rxpacket);
}

void test_receive_real_packet() {
    Serial.println("Testing loraReceive with a real packet...");
    // This test would require sending a real LoRa packet to the device during the test run.
    // It is not feasible to automate this test without a controlled environment, so it is commented out.
    // You can manually send a packet and observe the output to verify correct reception.

    Serial.println("The device will first wait for 15 seconds before listening for an incoming packet.");
    delay(15000); // Wait for 15 seconds to allow the tester to send a packet

    int received = 0;

    for (int i = 0; i < 10; i++) {
        loraReceive();
        delay(200);
        if (rxSize > 0) {
            Serial.println("Packet received during test_receive_real_packet.");
            Serial.printf("Packet size: %d\n", rxSize);
            Serial.println("Packet content (as string):");
            Serial.println(rxpacket);
            Serial.println("Hex dump of received packet:");
            for (int i = 0; i < rxSize; i++) {
                Serial.printf("%02X ", static_cast<uint8_t>(rxpacket[i]));

                if ((i + 1) % 16 == 0) {
                    Serial.println();
                }
            }
            Serial.println();

            received = 1;
            break;
        }
        delay(800); // Wait for a packet to arrive
    }

    TEST_ASSERT_TRUE(received); // Assert that a packet was received
}

/*=========== MAIN FUNCTIONS ============*/

void setup() {
    UNITY_BEGIN();
    Serial.begin(115200);
    delay(2000);

    RUN_TEST(test_loraSetup_success);
    RUN_TEST(test_loraReceive_noPacket);
    RUN_TEST(test_OnRxDone);
    RUN_TEST(test_receive_real_packet);

    delay(30000);
    Serial.println("All tests completed. Waiting 30 seconds before ending the test suite.");

    UNITY_END();
}

void loop() {

}