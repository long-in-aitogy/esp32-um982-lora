#if NMEA_COMMUNICATION_PROTOCOL == 1
#include "hardware/Lora_handler.h"
#include "functions/Nmea_Handler_LoRa.h"

static bool lora_idle;

static double txNumber;

static int16_t rssiValue;

static RadioEvents_t RadioEvents;

int16_t rxSize;
char rxpacket[BUFFER_SIZE];

int loraSetup( void ) {   
    txNumber=0;
    rssiValue=0;
  
    RadioEvents.RxDone = OnRxDone;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                               LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                               LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                               0, true, false, 0, LORA_IQ_INVERSION_ON, true );
    
    lora_idle = true;
    return 0;
}

int loraReceive()
{
  if(lora_idle)
  {
    lora_idle = false;
    Serial.println("[LoRa RX] Entering RX mode");
    Radio.Rx(0);
  }
  Radio.IrqProcess( );
  return 0;
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    rssiValue=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    Radio.Sleep( );
    Serial.printf("\r\n[LoRa RX] Received packet with rssi %d , length %d\r\n",rssiValue,rxSize);
    lora_idle = true;
}

#endif