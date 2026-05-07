#include "helper.h"

// ================= ĐỊNH NGHĨA CÁC BIẾN TOÀN CỤC =================
String nmeaBuffer = "";
unsigned long lastHealthCheck = 0;
String latestGGA = "";

// Toạ độ của mục tiêu, tạm thời để giá trị mẫu
String targetGGA = "$GNGGA,045151.00,2104.44183385,N,10546.62503715,E,1,28,0.7,22.4381,M,-28.2448,M,,*6C";

// Semaphore
SemaphoreHandle_t mqttClientMutex = NULL;
SemaphoreHandle_t nmeaBufferMutex = NULL;

/* ===================== NGUYÊN MẪU HÀM ======================== */

void taskNmea(void* parameter);
void gnssParseTask(void* parameter);
void gnssPublishTask(void* parameter);
void healthCheckTask(void* parameter);

/* ==================SETUP VÀ LOOP======================== */

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("\n=========================================");
    Serial.println("     ESP32 GNSS GATEWAY KHOI DONG        ");
    Serial.println("=========================================");

    // Khởi tạo giao tiếp với UM980
    Serial1.begin(GNSS_BAUD, SERIAL_8N1, RX_GNSS, TX_GNSS);
    bool networkConnected = false;

    connection_init:
#if CONNECT_USING_WIFI
    Serial.println("[SETUP] Su dung ket noi WIFI");
    networkConnected = setupWiFi();
#endif
#if CONNECT_USING_4G
    Serial.println("[SETUP] Su dung ket noi SIM/GSM");
    if (startSIM()) {
        if (connectGSM()) {
            networkConnected = true;
        }
    }
#endif
    if (networkConnected) {
        Serial.println("[SETUP] Ket noi mang thanh cong!");
        setupMQTT();
        #if NMEA_COMMUNICATION_PROTOCOL == TCP_IP
        setupNTRIP();
        #endif
    } else {
        Serial.println("[ERROR] Khong the ket noi mang. Vui long kiem tra cau hinh va thu lai.");
        goto connection_init;
    }

    Serial.println("[SETUP] Khoi dong cac task...");

    Serial.println("[Setup] Tao mutex de dong bo hoa tai nguyen chung");
    mqttClientMutex = xSemaphoreCreateMutex();
    while (mqttClientMutex == NULL) {
        Serial.println("[ERROR] Tao mutex mqttClientMutex that bai! Dang thu lai...");
        mqttClientMutex = xSemaphoreCreateMutex();
    }
    nmeaBufferMutex = xSemaphoreCreateMutex();

    while (nmeaBufferMutex == NULL) {
        Serial.println("[ERROR] Tao mutex nmeaBufferMutex that bai! Dang thu lai...");
        nmeaBufferMutex = xSemaphoreCreateMutex();
    }

    Serial.println("[SETUP] Task NMEA: Doc du lieu NMEA tu UM980");
    xTaskCreatePinnedToCore(taskNmea, "NMEA Task", 4096, NULL, 1, NULL, 0);
    Serial.println("[SETUP] Da khoi dong Task NMEA!");

    Serial.println("[SETUP] Task GNSS Parse: Phan tich du lieu NMEA va chuan bi payload");
    xTaskCreatePinnedToCore(gnssParseTask, "GNSS Parse Task", 4096, NULL, 3, NULL, 0);
    Serial.println("[SETUP] Da khoi dong Task GNSS Parse!");

    Serial.println("[SETUP] Task GNSS Publish: Gui du lieu da duoc phan tich len MQTT");
    xTaskCreatePinnedToCore(gnssPublishTask, "GNSS Publish Task", 4096, NULL, 2, NULL, 0);
    Serial.println("[SETUP] Da khoi dong Task GNSS Publish!");

    Serial.println("[SETUP] Task Health Check: Gui thong tin suc khoe thiet bi len MQTT moi 30s");
    xTaskCreatePinnedToCore(healthCheckTask, "Health Check Task", 4096, NULL, 4, NULL, 0);
    Serial.println("[SETUP] Da khoi dong Task Health Check!");

    Serial.println("=========================================");
    Serial.println("        KHOI DONG HOAN TAT               ");
    Serial.println("=========================================\n");
}

/* ================= TRIỂN KHAI HÀM TASK ====================== */

void taskNmea(void* parameter) {
    // không sử dụng tài nguyên chung, không cần mutex
    while (true) {
        #if NMEA_COMMUNICATION_PROTOCOL == TCP_IP
        loopNTRIP(latestGGA);
        #else
        loraWanMain();
        #endif
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void gnssParseTask(void* parameter) {
    // sử dụng nmeaBuffer làm tài nguyên chung với publishTask, cần mutex để tránh xung đột
    
    while (true) {
        if (xSemaphoreTake(nmeaBufferMutex, pdMS_TO_TICKS(MUTEX_TIMEOUT_MS))) {
            if (Serial1.available()) {
                roverReadCharFromRtk(nmeaBuffer);
            }
            xSemaphoreGive(nmeaBufferMutex);
        }
    }
    vTaskDelay(pdMS_TO_TICKS(10));
}

void gnssPublishTask(void* parameter) {
    // sử dụng nmeaBuffer làm tài nguyên chung với gnssParseTask
    // sử dụng chung đối tượng lớp PubSubClient là mqtt với healthCheckTask
    while (true) {
        if (xSemaphoreTake(nmeaBufferMutex, pdMS_TO_TICKS(MUTEX_TIMEOUT_MS))
            && xSemaphoreTake(mqttClientMutex, pdMS_TO_TICKS(MUTEX_TIMEOUT_MS)))
        {
            loopMQTT();
            publishGGA(nmeaBuffer);
            xSemaphoreGive(mqttClientMutex);
            xSemaphoreGive(nmeaBufferMutex);
        }
    }
}

void healthCheckTask(void* parameter) {
    while (true) {
        if (xSemaphoreTake(mqttClientMutex, pdMS_TO_TICKS(MUTEX_TIMEOUT_MS)))
        {
            loopMQTT();
            sendDeviceHealth();
            xSemaphoreGive(mqttClientMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(HEALTH_INTERVAL));
    }
}

void loop() {}
