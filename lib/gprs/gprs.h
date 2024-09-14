#pragma once
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>
#include <StreamDebugger.h>
#include "shared.h"

class GPRS
{
public:
    static GPRS &GET()
    {
        static GPRS instance;
        return instance;
    }
    TinyGsm modem;
    TinyGsmClient client;
    void initialize();
    enum ConnectionStatus connectionFlag = NOT_CONNECTED;

private:
    GPRS();
    StreamDebugger debugger;
    EspSoftwareSerial::UART swSer;
};