#if (NTRIP_COMMUNICATION_PROTOCOL == LORA_SERIAL || NTRIP_COMMUNICATION_PROTOCOL == 1)
#define NTRIP_HANDLER_LORA_CODE

#include "functions/NTRIP_Handler_LoRa.h"

uint8_t *ReceivedBuffer;
uint8_t ReceivedBufferSize;

int loopNmeaLoRa() {
    int bytesRead = Serial1.readBytes(ReceivedBuffer, ReceivedBufferSize);
    Serial1.write(ReceivedBuffer, bytesRead); 
}

#endif