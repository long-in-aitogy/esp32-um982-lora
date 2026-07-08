#include "Top_Lvl_Config.h"

#if CONNECT_USING_4G
#define GSM_CODE

#include "Prog_Config.h"
#include "hardware/Sim_handler.h"

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm        modem(debugger);
#else
TinyGsm        modem(SerialAT);
#endif

bool startSIM() {
    
    int retrys = 1;

    digitalWrite(LED_PIN, HIGH);
    pinMode(MODEM_DC_PIN, OUTPUT);
    digitalWrite(MODEM_DC_PIN, HIGH);
    delay(1000);
    digitalWrite(MODEM_DC_PIN, LOW);
    SerialMon.println("[GSM] Doi modem SIM/GSM khoi dong (khoang 8 giay)...");
    delay(8000);
    digitalWrite(LED_PIN, LOW);

    SerialMon.println("[GSM] Khoi tao modem... So lan thu: " + String(retrys));
    
    #ifndef NATIVE_BUILD
    modem.init();
    #endif

    String modemInfo = modem.getModemInfo();
    while (modemInfo.length() == 0) {
        if (retrys > 10) {
            SerialMon.println("[GSM] Lay thong tin modem that bai sau nhieu lan thu ! Khoi dong modem that bai !");
            return false;
        }
        SerialMon.println("[GSM] Lay thong tin modem that bai ! So lan thu: " 
            + String(retrys));
        delay(1000);
        #ifndef NATIVE_BUILD
        modem.restart();
        #endif
        modemInfo = modem.getModemInfo();
        retrys++;
    }
    SerialMon.print("[GSM] Khoi dong modem thanh cong.Thong tin modem: ");
    SerialMon.println(modemInfo);
    digitalWrite(LED_PIN, LOW);

    delay(3000);
    return true;
}

bool connectGSM() {
    uint8_t retries = 0;
    while (retries < 10) {
        SerialMon.println("[GSM] Dang ket noi mang GSM...");
        if (modem.isNetworkConnected()) {
            SerialMon.println("[GSM] Mang GSM da ket noi.");

            if (modem.isGprsConnected()) {
                SerialMon.println("[GSM] Du lieu di dong da san sang.");
                return true;
            }

            SerialMon.print("[GSM] Dang mo PDP context voi APN: ");
            SerialMon.println(APN);
            if (modem.gprsConnect(APN, GPRS_USER, GPRS_PASS)) {
                SerialMon.println("[GSM] GPRS/PDP da ket noi thanh cong.");
                return true;
            }

            SerialMon.println("[GSM] Mo GPRS/PDP that bai. Dang thu lai...");
        }
        while (!modem.waitForNetwork(60000)) {
            SerialMon.println("[GSM] Ket noi mang GSM that bai. Dang thu lai... So lan thu: " + String(retries));
            retries++;
            delay(10000);
        }
    }
    SerialMon.println("[GSM] Ket noi mang GSM that bai sau " + String(retries) + " lan thu. Ket thuc ket noi !");
    return false;
}

#endif // GSM_CODE
