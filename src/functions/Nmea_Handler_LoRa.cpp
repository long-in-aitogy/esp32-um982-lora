#if (NMEA_COMMUNICATION_PROTOCOL == LORA_SERIAL || NMEA_COMMUNICATION_PROTOCOL == 1)
#define NTRIP_HANDLER_LORA_CODE

#include "functions/Nmea_Handler_LoRa.h"
#include "hardware/Lora_handler.h"

extern uint8_t *ReceivedBuffer;
extern uint8_t ReceivedBufferSize;

int loopNmeaLoRa() {
    if (ReceivedBufferSize > 0)
    {
        Serial1.write(ReceivedBuffer, ReceivedBufferSize);
        Serial1.println("[NMEA over LoRA] Da nhan duoc NMEA qua LoRA, da gui den mach RTK!");
        Serial1.println("[NMEA over LoRA] Kich thuoc du lieu: " + String(ReceivedBufferSize) + " bytes");
        Serial1.println("[NMEA over LoRA] Noi dung du lieu: " + String((char*)ReceivedBuffer));
        return 0;
    }
    return -1;
}

#endif