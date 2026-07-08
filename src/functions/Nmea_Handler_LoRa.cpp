#if (NMEA_COMMUNICATION_PROTOCOL == LORA_SERIAL || NMEA_COMMUNICATION_PROTOCOL == 1)
#define NTRIP_HANDLER_LORA_CODE

#include "functions/Nmea_Handler_LoRa.h"
#include <LoRaWan_APP.h>

int pushNmeaLoRaToGnss(char* nmeaData, size_t dataSize) {
    // Không sử dụng tài nguyên chung, không cần mutex
    if (dataSize > 0)
    {
        if (nmeaData == nullptr) {
            Serial1.println("[NMEA over LoRA] Loi: Buffer rong!");
            return -2;
        }

        Serial1.write(nmeaData, dataSize);
        Serial1.println("[NMEA over LoRA] Da nhan duoc NMEA qua LoRA, da gui den mach RTK!");
        Serial1.println("[NMEA over LoRA] Kich thuoc du lieu: " + String(dataSize) + " bytes");
        Serial1.println("[NMEA over LoRA] Noi dung du lieu: " + String(nmeaData));
        return 0;
    }
    Serial1.println("[NMEA over LoRA] Loi: Buffer co do dai bang 0!");
    return -1;
}

#endif