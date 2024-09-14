#include "mqtt.h"
#include "gprs.h"
#include "customConfig.h"
#include <string>
#include "shared.h"
#include "handler.h"

MQTT::MQTT() : mqtt(GPRS::GET().client)
{
    mqtt.setServer(CustomConfig::GET().mqtt_api.c_str(), CustomConfig::GET().mqtt_port);
    mqtt.setCallback(mqttCallback);
    mqtt.setKeepAlive(60);
}

void MQTT::mqttCallback(char *topic, uint8_t *payload, unsigned int len)
{
    std::string topicStr(topic);
    size_t slashPosition = topicStr.find("/");
    std::string onlyTopic = topicStr.erase(0, slashPosition + 1);
    std::string result(reinterpret_cast<char *>(payload), reinterpret_cast<char *>(payload + len));
    Serial.printf("Message Recived: \n Topic:\t %s, \n Value:\t %s\n", onlyTopic.c_str(), result.c_str());

    if (onlyTopic == "G")
    {
        Handler::GET().handleResponse(Response_G, result);
    }
    if (onlyTopic == "O")
    {
        Handler::GET().handleResponse(Response_O, result);
    }
    if (onlyTopic == "I")
    {
        Handler::GET().handleResponse(Response_I, result);
    }
    if (onlyTopic == "Q")
    {
        Handler::GET().handleResponse(Response_Q, result);
    }
    if (onlyTopic == "A")
    {
        Handler::GET().handleResponse(Response_A, result);
    }
}

void MQTT::intialize()
{
    Serial.println("Connecting MQTT...");
    Serial.println(mqtt.state());
    if (GPRS::GET().connectionFlag == CONNECTED && connectionFlag != CONNECTING)
    {
        connectionFlag = NOT_CONNECTED;
        connectionFlag = mqtt.connected() ? CONNECTED : NOT_CONNECTED;
        if (connectionFlag != CONNECTED)
        {
            if (!mqtt.connect(CustomConfig::GET().username.c_str(),
                              CustomConfig::GET().username.c_str(),
                              CustomConfig::GET().password.c_str()))
            {
                connectionFlag = NOT_CONNECTED;
                Serial.println("ERROR:: MQTT Connection failed");
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            else
            {
                mqtt.subscribe((CustomConfig::GET().username + "/#").c_str());
                connectionFlag = CONNECTED;
                Serial.println("ERROR:: MQTT Connection connected");
            }
        }
    }
    Serial.println(mqtt.connected());
    return;
}

bool MQTT::sendData(Topic topic, std::string data)
{
    sending = true;
    Serial.println(mqtt.connected());
    std::string topicVal = "";
    switch (topic)
    {
    case G:
        topicVal = 'G';
        break;

    case C:
        topicVal = 'C';
        break;

    case E:
        topicVal = 'E';
        break;
    case A:
        topicVal = 'A';
    }
    bool published = mqtt.publish(topicVal.c_str(), data.c_str(), true);
    sending = false;
    return published;
}

bool MQTT::mqttStatus()
{
    return mqtt.connected();
}
