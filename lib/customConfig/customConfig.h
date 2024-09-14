#pragma once
#include <Arduino.h>

class CustomConfig
{
public:
    static CustomConfig &GET()
    {
        static CustomConfig instance;
        return instance;
    }
    std::string username = "654321";
    std::string password = "123456789";
    int8_t gps_rx = 5;
    int8_t gps_tx = 4;
    uint32_t gps_baud = 9600;
    std::string apn = "web";
    std::string mqtt_api = "mqtt.omnecal.com";
    int mqtt_port = 1883;
    int8_t custom_button = 16;
    int8_t relay_pin = 15;
    int8_t volMeasurement_pin = 14;
    int8_t buzzer_pin = 45;
    int8_t neoPixel_pin = 2;
};