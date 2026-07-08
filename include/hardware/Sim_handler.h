#ifndef SIM_HANDLER_H
#define SIM_HANDLER_H

// xác định loại modem
#define TINY_GSM_MODEM_SIM7600

// Set serial for debug console
#define SerialMon Serial

// Set serial for AT commands
#define SerialAT Serial2

#include <TinyGsmClient.h>
#include "Top_Lvl_Config.h"

#ifdef DUMP_AT_COMMANDS
#define TINY_GSM_DEBUG SerialMon
#include <StreamDebugger.h>
extern StreamDebugger debugger;
extern TinyGsm        modem;
#else
extern TinyGsm        modem;
#endif

bool startSIM();
bool connectGSM();

#endif