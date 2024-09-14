#include "gprs.h"
#include <TinyGsmClient.h>
#include "customConfig.h"
#include <string>
#include <Arduino.h>

GPRS ::GPRS() : debugger(swSer, Serial), modem(debugger), client(modem)
{
    // Serial.println("Here");
    swSer.begin(9600, EspSoftwareSerial::SWSERIAL_8N1, 11, 12);
    std::string imei = modem.getIMEI().c_str();
    // Serial.println(imei.c_str());
    if (imei.size() == 15)
    {
        // CustomConfig::GET()
        //     .username = imei.substr(imei.size() - 6);
    }
}

void GPRS::initialize()
{
    if (connectionFlag != CONNECTING)
    {
        connectionFlag = NOT_CONNECTED;
        Serial.println(modem.getSimStatus());
        connectionFlag = modem.isGprsConnected() ? CONNECTED : NOT_CONNECTED;
        if (connectionFlag == NOT_CONNECTED)
        {
            connectionFlag = CONNECTING;
            Serial.print("LOG:: GPRS waiting for network...");
            if (!modem.waitForNetwork(6000L, true))
            {
                Serial.println("ERROR:: Unable to connect to the Network");
                connectionFlag = NOT_CONNECTED;
                return;
            }
            if (!modem.isNetworkConnected())
            {
                connectionFlag = NOT_CONNECTED;
                Serial.println("LOG:: GPRS Network connected");
                return;
            }
            Serial.println(("LOG:: GPRS connecting to " + CustomConfig::GET().apn).c_str());
            if (!modem.gprsConnect(CustomConfig::GET().apn.c_str(), "", ""))
            {
                connectionFlag = NOT_CONNECTED;
                Serial.println("ERROR:: GPRS Connection failed");
                vTaskDelay(pdMS_TO_TICKS(1000));
                return;
            }
        }
        else
        {
            Serial.println("ERROR:: GPRS Connection connected");
            connectionFlag = CONNECTED;
            return;
        }
    }
    Serial.println(connectionFlag);

    return;
}