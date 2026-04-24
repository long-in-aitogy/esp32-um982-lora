#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

// ============ GẮN CÁC THƯ VIỆN CẦN THIẾT ==============

#include <Arduino.h>
#ifdef UNIT_TEST
#include "mockPubSubClient.h"
#else
#include <PubSubClient.h>
#endif
#include "Top_Lvl_Config.h"

extern PubSubClient mqtt;

// ================= KHAI BÁO HÀM =================

int setupMQTT();
int loopMQTT();
int publishData(String payload, bool isGGA);
int publishRaw(String payload, bool isGGA);
int publishHealth(String payload); // Thêm hàm gửi thông tin sức khỏe
bool isMqttConnected();             // Thêm hàm lấy trạng thái kết nối MQTT

#endif