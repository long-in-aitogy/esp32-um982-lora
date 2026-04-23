#ifndef LORA_HANDLER_H
#define LORA_HANDLER_H

#ifndef LORAWAN_CLASS
#define LORAWAN_CLASS CLASS_A
#endif

#ifndef ACTIVE_REGION
#define ACTIVE_REGION LORAMAC_REGION_AS923
#endif

#ifndef LORAWAN_OTA
#define LORAWAN_OTA true
#endif

#ifndef LORAWAN_ADR
#define LORAWAN_ADR true
#endif

#ifndef LORAWAN_NET_RESERVE
#define LORAWAN_NET_RESERVE false
#endif

#ifndef TX_CONFIRMED
#define TX_CONFIRMED false
#endif

#include <LoRaWan_APP.h>

void prepareTxFrame( uint8_t port );
void setupLoraWan();
#endif