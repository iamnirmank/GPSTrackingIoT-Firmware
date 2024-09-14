#pragma once
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "customConfig.h"

class GPS
{
public:
    static GPS &GET()
    {
        static GPS instance;
        return instance;
    }
    void IHandleData();

private:
    GPS();
    TinyGPSPlus gps;
    SoftwareSerial gps_serial;
    unsigned long epoch_time();
    void displayInfo();
    unsigned long previousTime = millis();
};