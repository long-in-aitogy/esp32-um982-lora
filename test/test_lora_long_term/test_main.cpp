
#include "Top_Lvl_Config.h"
#include <unity.h>
#include <Arduino.h>
#include "hardware/Lora_handler.h"

/*=========== TESTS ============*/
void test_receive_large_packets() {
    Serial.println("Testing loraReceive with larger (243-byte) packets...");

    // This test would require sending a real LoRa packet to the device during the test run.
    // It is not feasible to automate this test without a controlled environment, so it is commented out.
    // You can manually send a packet and observe the output to verify correct reception.

    int received = 0;

    Serial.printf("Listening for packets...\n");
    loraReceive();
    if (rxSize > 0) {
        digitalWrite(LED_PIN, HIGH); // Turn on LED to indicate packet reception
        delay(60); // Keep LED on for a short time
        Serial.println("===================================================");
        Serial.printf("At time: %.3f s, ", millis()/1000.0f);
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
        rxSize = 0;
        memcpy(rxpacket, "", sizeof(rxpacket));
        digitalWrite(LED_PIN, LOW); // Turn off LED after processing
        delay(40); // Short delay to ensure LED state is visible
    }
}

/*=========== MAIN FUNCTIONS ============*/

void setup() {
    UNITY_BEGIN();
    Serial.begin(115200);
    delay(2000);

    // Initialize board hardware as done in production `main()`
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
    loraSetup();
    pinMode(LED_PIN, OUTPUT);
    delay(2000);

    Serial.println("Setting up LoRa...");
    int result = loraSetup();

    digitalWrite(LED_PIN, HIGH); // Ensure LED is off after test
    delay(100);
    digitalWrite(LED_PIN, LOW);
}

void loop() {
    if (0) {
        UNITY_END();
    }
    test_receive_large_packets();
    delay(100);
}