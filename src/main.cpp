#include "helper.h"

// ================= ĐỊNH NGHĨA CÁC BIẾN TOÀN CỤC =================
unsigned long lastHealthCheck = 0;
String latestGGA = "";

// Toạ độ của mục tiêu, tạm thời để giá trị mẫu
String targetGGA = "$GNGGA,045151.00,2104.44183385,N,10546.62503715,E,1,28,0.7,22.4381,M,-28.2448,M,,*6C";

gga_data_struct ggaData;
gga_data_struct targetGgaData;
ksxt_data_struct ksxtData;

int gnssRoverParseAndMqtt()
{
    String nmeaBuffer = "";
    char c = Serial1.read();
    nmeaBuffer += c;

    if (c == '\n')
    {
        nmeaBuffer.trim();

        // Bắt dòng tọa độ
        if (nmeaBuffer.startsWith("$GNGGA") || nmeaBuffer.startsWith("$GPGGA") || nmeaBuffer.startsWith("$KSXT"))
        {
            // Cập nhật tọa độ mới nhất để NTRIP dùng xác thực (Mode 3)
            latestGGA = nmeaBuffer;

            // Đẩy lên MQTT
            String jsonPayload = "";
            if (nmeaBuffer.startsWith("$KSXT"))
            {
                bool parseOk = parseKSXT_toStruct(nmeaBuffer, ksxtData);
                if (parseOk)
                {
                    jsonPayload = parseKSXT_toJSON(ksxtData);
                }
                publishData(jsonPayload, false);
            }
            else
            {
                publishRaw(nmeaBuffer, true);
                bool parseOk = parseGGA_toStruct(nmeaBuffer, ggaData);
                if (parseOk)
                {
                    jsonPayload = parseGGA_toJSON(ggaData);
                }
                
            }
            publishRaw(nmeaBuffer, false);
            publishData(jsonPayload, false);
            return 0;
        }
        // Bắt dòng phản hồi lệnh
        else if (nmeaBuffer.startsWith("#"))
        {
            Serial.print("[UM980 RESPONSE] ");
            Serial.println(nmeaBuffer);
            return -1;
        }
        return -1;
    }
}

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
        setupNTRIP();
    } else {
        Serial.println("[ERROR] Khong the ket noi mang. Vui long kiem tra cau hinh va thu lai.");
        goto connection_init;
    }

    Serial.println("=========================================");
    Serial.println("        KHOI DONG HOAN TAT               ");
    Serial.println("=========================================\n");
}

void loop()
{
    // 1. Duy trì kết nối mạng
    loopMQTT();
    loopNTRIP(latestGGA);

    // === KIỂM TRA VÀ GỬI THÔNG TIN SỨC KHỎE ===
    if (millis() - lastHealthCheck >= HEALTH_INTERVAL)
    {
        sendDeviceHealth();
        lastHealthCheck = millis();
    }

    // 2. Xử lý luồng dữ liệu từ UM980
    while (Serial1.available())
    {
        gnssRoverParseAndMqtt();
    }
}
