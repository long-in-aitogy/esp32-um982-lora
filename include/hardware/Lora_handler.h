#ifndef LORA_HANDLER_H
#define LORA_HANDLER_H

#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "Prog_Config.h"

inline constexpr int RX_TIMEOUT_VALUE = 1000;
inline constexpr uint8_t BUFFER_SIZE = 255; // Define the payload size here

extern char rxpacket[BUFFER_SIZE];

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
int loraReceive();
int loraSetup(void);
#endif