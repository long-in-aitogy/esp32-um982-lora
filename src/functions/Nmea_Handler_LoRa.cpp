#if (NMEA_COMMUNICATION_PROTOCOL == LORA_SERIAL || NMEA_COMMUNICATION_PROTOCOL == 1)
#define NTRIP_HANDLER_LORA_CODE

#include "functions/Nmea_Handler_LoRa.h"
#include <LoRaWan_APP.h>

#ifdef UNIT_TEST
#include <ArduinoFake.h>
#endif

int pushNmeaLoRaToGnss(McpsIndication_t *mcpsIndication) {
    #ifdef UNIT_TEST
    auto& serialOut = Serial;
    #else
    auto& serialOut = Serial1;
    #endif
    if (mcpsIndication->BufferSize > 0)
    {
        if (mcpsIndication->Buffer == nullptr) {
            serialOut.println("[NMEA over LoRA] Loi: Buffer rong!");
            return -2;
        }

        serialOut.write(mcpsIndication->Buffer, mcpsIndication->BufferSize);
        serialOut.println("[NMEA over LoRA] Da nhan duoc NMEA qua LoRA, da gui den mach RTK!");
        serialOut.println("[NMEA over LoRA] Kich thuoc du lieu: " + String(mcpsIndication->BufferSize) + " bytes");
        serialOut.println("[NMEA over LoRA] Noi dung du lieu: " + String((char*)mcpsIndication->Buffer));
        return 0;
    }
    serialOut.println("[NMEA over LoRA] Loi: Buffer co do dai bang 0!");
    return -1;
}

#endif