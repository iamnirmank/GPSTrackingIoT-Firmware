#include "gps.h"
#include "customConfig.h"
#include "shared.h"
#include <Arduino.h>
#include "handler.h"

GPS::GPS() : gps_serial(CustomConfig::GET().gps_rx, CustomConfig::GET().gps_tx)
{
    Serial.println("GPS Begined");
    gps_serial.begin(CustomConfig::GET().gps_baud);
}

void GPS::IHandleData()
{
    unsigned long start = millis();
    do
    {
        while (gps_serial.available())
        {
            previousTime = millis();
            gps.encode(gps_serial.read());
        }
    } while (millis() - start < 2000);
    if (gps.location.isValid() && gps.date.isValid() && gps.time.isValid())
    {
        std::string gpsData = data_format({(double)epoch_time(),
                                           gps.location.lat(),
                                           gps.location.lng(),
                                           gps.speed.kmph(),
                                           gps.course.deg()});
        Serial.println(gpsData.c_str());
        Handler::GET()
            .handleRequest(G,
                           gpsData);
    }
    else
    {
        Serial.println("ERROR::Invalid GPS Data");
        return;
    }
    if (previousTime - millis() > 10000 && gps.charsProcessed() < 10)
    {
        Serial.println(F("No GPS detected: check wiring."));
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

unsigned long GPS::epoch_time()
{
    struct tm t;
    time_t epoch;
    t.tm_year = gps.date.year() - 1900;
    t.tm_mon = gps.date.month() - 1;
    t.tm_mday = gps.date.day();
    t.tm_hour = gps.time.hour();
    t.tm_min = gps.time.minute();
    t.tm_sec = gps.time.second();
    epoch = mktime(&t);
    return ((unsigned long)epoch);
}