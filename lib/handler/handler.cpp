#include "handler.h"
#include "mqtt.h"
#include <Arduino.h>
#include "gprs.h"
#include "gpio.h"
#include <string>

void Handler::handleRequest(Topic topic, std::string data)
{

    DataObject structData;
    structData.data = data;
    structData.topic = topic;
    dataList.insert(dataList.begin(), structData);
    return;
}

void Handler::handleResponse(ResponseTopic topic, std::string data)
{
    std::vector<std::string> datas = serializedResponse(data);
    switch (topic)
    {
    case Response_G:
        Serial.printf("GAP: %s, %s\n", datas[0], datas[1]);
        break;
    case Response_O:
        // OTA::GET().handleOta();
        break;
    case Response_I:
        GPIO::GET().handleIgnition();
        break;
    case Response_Q:
        tripRequest = true;
        GPIO::GET().handleBuzzer(GPIO::QUEUE);
        break;
    case Response_A:
        GPIO::GET().handleBuzzer(GPIO::ALERT);
    }
}

void Handler::resolveData()
{
    if (!sending)
    {
        sending = true;
        if (GPRS::GET().connectionFlag == CONNECTED && MQTT::GET().connectionFlag == CONNECTED)
        {
            if (!dataList.empty())
            {
                DataObject data = dataList.back();
                std::string topicVal = "";
                switch (data.topic)
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
                    break;
                }
                Serial.printf("topic: %s, payload: %s \n", topicVal.c_str(), data.data.c_str());

                if (MQTT::GET().sendData(data.topic, data.data))
                {
                    dataList.pop_back();
                }
            }
        }
        sending = false;
    }
}
