#include "gpio.h"
#include "customConfig.h"
#include "handler.h"
#include "shared.h"
#include "neoPixel.h"

GPIO::GPIO()
{
    pinMode(CustomConfig::GET().relay_pin, OUTPUT);
    pinMode(CustomConfig::GET().buzzer_pin, OUTPUT);
    pinMode(CustomConfig::GET().custom_button, INPUT);
}

void GPIO::handleIgnition()
{
    digitalWrite(CustomConfig::GET().relay_pin, HIGH);
    delay(1000);
    digitalWrite(CustomConfig::GET().relay_pin, LOW);
}

void GPIO::handleBuzzer(BuzzerType type)
{
    int freq;
    switch (type)
    {
    case ALERT:
        freq = 10;
        break;
    case QUEUE:
        freq = 7;
    default:
        break;
    }
    for (int i = 0; i < freq * 1.5; i++)
    {
        digitalWrite(CustomConfig::GET().buzzer_pin, HIGH);
        delay(10000 / freq);
        digitalWrite(CustomConfig::GET().buzzer_pin, LOW);
        delay(5000 / freq);
    }
}

void GPIO::handleInputs()
{
    if (!digitalRead(CustomConfig::GET().custom_button))
    {
#warning "Put a guard here ; dont run below code if gpio isnt initialized"
        Serial.println("HIGH");
        NeoPixel::GET().blinkAll(255, 0, 0);
        Handler::GET().handleRequest(A, data_format({

                                            0.0}));
        vTaskDelay(pdMS_TO_TICKS(500));
        NeoPixel::GET().stopAll();
        vTaskDelay(pdMS_TO_TICKS(500));
        NeoPixel::GET().blinkAll(255, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        NeoPixel::GET().stopAll();
        vTaskDelay(pdMS_TO_TICKS(500));
        NeoPixel::GET().blinkAll(0, 255, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
        NeoPixel::GET().stopAll();
    }
}