#pragma once
#include "PubSubClient.h"
#include <string>
#include "shared.h"

class MQTT
{
public:
    static MQTT &GET()
    {
        static MQTT instance;
        return instance;
    }
    void intialize();
    bool sendData(Topic topic, std::string data);
    PubSubClient mqtt;
    bool mqttStatus();
    bool sending = false;
    ConnectionStatus connectionFlag = NOT_CONNECTED;

private:
    MQTT();
    static void mqttCallback(char *topic, uint8_t *payload, unsigned int len);
};
