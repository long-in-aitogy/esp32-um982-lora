#ifndef NTRIP_HANDLER_IP_H
#define NTRIP_HANDLER_IP_H

// ============ GẮN CÁC THƯ VIỆN CẦN THIẾT ==============

#include <Arduino.h>
#include "Top_Lvl_Config.h"

// =============== KHAI BÁO HÀM =================

int setupNTRIP();
int loopNTRIP(String currentGGA);
int connectNTRIP();
bool isNtripConnected(); // Thêm hàm lấy trạng thái NTRIP

#if CONNECT_USING_WIFI
#include "hardware/Wifi_handler.h"
extern WiFiClient ntripClient;
#endif
#if CONNECT_USING_4G
#include "hardware/Sim_handler.h"
extern TinyGsmClient ntripClient;
#endif

#endif