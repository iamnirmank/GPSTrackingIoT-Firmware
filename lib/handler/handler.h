#pragma once
#include "shared.h"
#include <nvs_flash.h>

static RTC_DATA_ATTR std::vector<DataObject> dataList;

class Handler
{
public:
    static Handler &GET()
    {
        static Handler instance;
        return instance;
    }

    void handleRequest(Topic topic, std::string data);
    void handleResponse(ResponseTopic topic, std::string data);
    void resolveData();
    bool tripRequest = false;
    bool sending = false;

private:
    Handler(){};
};