
#include <unity.h>
#include <Arduino.h>
#include "Top_Lvl_Config.h"
#include "hardware/Lora_handler.h"

/*=========== TESTS ============*/
void test_loraSetup_success() {
    Serial.println("Testing loraSetup for successful initialization...");
    int result = loraSetup();
    TEST_ASSERT_EQUAL(0, result);
}

void test_receive_large_packets() {
    Serial.println("Testing loraReceive with larger (243-byte) packets...");
    // This test would require sending a real LoRa packet to the device during the test run.
    // It is not feasible to automate this test without a controlled environment, so it is commented out.
    // You can manually send a packet and observe the output to verify correct reception.

    Serial.println("The device will first wait for 10 seconds before listening for an incoming packet.");
    delay(10000); // Wait for 10 seconds to allow the tester to send a packet

    int received = 0;

    for (int i = 0; i < 60; i++) {
        loraReceive();
        delay(200);
        if (rxSize > 0) {
            Serial.println("Packet received during test_receive_large_packets.");
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
            Serial.print("===================================================\n");
            Serial.println();

            received++;
            if (received >= 3) {
                break; // Stop after receiving 3 large packets
            }
        }
        delay(200); // Wait for a packet to arrive
    }

    TEST_ASSERT_TRUE(received >= 3); // Assert that at least 3 large packets were received
}

/*=========== MAIN FUNCTIONS ============*/

void setup() {
    UNITY_BEGIN();
    Serial.begin(115200);
    delay(2000);

    RUN_TEST(test_loraSetup_success);
    RUN_TEST(test_receive_large_packets);

    Serial.println("All tests completed. Waiting before ending the test suite.");
    for (int i = 0; i < 60; i++) {
        Serial.printf("Ending in %d seconds...\n", 60 - i);
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
    }

    UNITY_END();
}

void loop() {

}