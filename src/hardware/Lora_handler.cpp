#include "hardware/Lora_handler.h"

DeviceClass_t loraWanClass = LORAWAN_CLASS;
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

uint8_t devEui[] = { 0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x02 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
            0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
uint8_t appPort = 2;
uint8_t confirmedNbTrials = 4;

bool overTheAirActivation = LORAWAN_OTA;
bool loraWanAdr = LORAWAN_ADR;
bool keepNet = LORAWAN_NET_RESERVE;
bool isTxConfirmed = TX_CONFIRMED;

void prepareTxFrame( uint8_t port )
{
    appDataSize = 4;
    appData[0] = 0x00;
    appData[1] = 0x01;
    appData[2] = 0x02;
    appData[3] = 0x03;
}

void setupLoraWan()
{
    #ifdef DUMP_AT_COMMANDS
	enableAt();
    #endif
	deviceState = DEVICE_STATE_INIT;
	LoRaWAN.ifskipjoin();
}