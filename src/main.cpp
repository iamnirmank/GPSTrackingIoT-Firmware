#include <Arduino.h>
#include <TaskManager.h>
#include "gps.h"
#include "mqtt.h"
#include "gprs.h"
#include "elm.h"
#include "handler.h"
#include "gpio.h"
#include "accelerometer.h"
#include "neoPixel.h"

void loopMqtt(void *);
void loopGPS(void *);
void loopELM(void *);
void loopConsumer(void *);
void loopAccelerometer(void *);
void loopGPIO(void *pvparams);

void setup()
{
  Serial.begin(115200);

  Serial.println("neopixel init");
  NeoPixel::GET().initialize(); // bad // gpio_reinit
  NeoPixel::GET().blinkAll(255, 9, 0);
  delay(5000);

  Serial.println("accelero init");
  Accelerometer::GET().initialize(); // good
  delay(1000);

  NeoPixel::GET().stopAll();
  xTaskCreate(
      loopGPS,   // Task function.
      "TaskGPS", // Name of task.
      4096 * 2,  // Stack size of task
      NULL,      // Parameter of the task
      4,         // Priority of the task
      NULL       // Task handle to keep track of created task
  );
  delay(3000);

  xTaskCreate(
      loopMqtt,   // Task function.
      "TaskMQTT", // Name of task.
      4096 * 2,   // Stack size of task
      NULL,       // Parameter of the task
      5,          // Priority of the task
      NULL        // Task handle to keep track of created task
  );
  delay(3000);

  xTaskCreate(
      loopAccelerometer,   // Task function.
      "TaskAccelerometer", // Name of task.
      4096 * 2,            // Stack size of task
      NULL,                // Parameter of the task
      3,                   // Priority of the task
      NULL                 // Task handle to keep track of created task
  );
  delay(2000);
  xTaskCreate(
      loopGPIO,   // Task function.
      "TaskGPIO", // Name of task.
      4096 * 2,   // Stack size of task
      NULL,       // Parameter of the task
      3,          // Priority of the task
      NULL        // Task handle to keep track of created task
  );
  delay(2000);
  xTaskCreatePinnedToCore(
      loopELM,   // Task function.
      "TaskELM", // Name of task.
      4096 * 2,  // Stack size of task
      NULL,      // Parameter of the task
      4,         // Priority of the task
      NULL,      // Task handle to keep track of created task
      1          // Pin task to core 0
  );
  delay(2000);
  xTaskCreate(
      loopConsumer,
      "Consumer",
      4096 * 2,
      NULL,
      4,
      NULL);
  NeoPixel::GET().blinkAll(0, 255, 0);
  delay(2000);
  NeoPixel::GET().stopAll();
}

void loopMqtt(void *pvparams)
{
  Serial.println("MQTT Task");
  while (true)
  {
    Serial.println("MQTT here!");
    if (GPRS::GET().connectionFlag != CONNECTING)
    {
      Serial.println("GPRS Con");
      GPRS::GET().connectionFlag = NOT_CONNECTED;
      GPRS::GET().connectionFlag = GPRS::GET().modem.isGprsConnected() ? CONNECTED : NOT_CONNECTED;
      if (GPRS::GET().connectionFlag == NOT_CONNECTED)
      {
        Serial.println("GPRS Int");

        Serial.println("IN GPRS RECONNECTING");
        GPRS::GET().initialize();
      }
    }
    else if (GPRS::GET().connectionFlag == CONNECTED && MQTT::GET().connectionFlag != CONNECTING)
    {
      Serial.println("MQTT Con");

      MQTT::GET().connectionFlag = NOT_CONNECTED;
      MQTT::GET().connectionFlag = MQTT::GET().mqtt.loop() ? CONNECTED : NOT_CONNECTED;
      if (MQTT::GET().connectionFlag == NOT_CONNECTED)
      {
        Serial.println("IN MQTT RECONNECTING");
        MQTT::GET().intialize();
      }
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
  vTaskDelete(NULL);
}

void loopGPS(void *pvparams)
{
  Serial.println("GPS Task");
  while (true)
  {
    // Serial.println("GPS : Here!");
    GPS::GET().IHandleData();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
  vTaskDelete(NULL);
}

void loopELM(void *pvparams)
{
  Serial.println("ELM Task");
  while (true)
  {
    // Serial.println("ELM : Here!");
    ELM::GET().IHandleData();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
  vTaskDelete(NULL);
}
void loopAccelerometer(void *pvparams)
{
  Serial.println("accelerometer Task");
  while (true)
  {
    // Serial.println("accelerometer : Here!");
    Accelerometer::GET().handleAccelerometer();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  vTaskDelete(NULL);
}

void loopGPIO(void *pvparams)
{
  Serial.println("GPIO Task");
  while (true)
  {
    // Serial.println("GPIO : Here!");
    GPIO::GET().handleInputs();
    vTaskDelay(pdMS_TO_TICKS(300));
  }
  vTaskDelete(NULL);
}

void loopConsumer(void *pvParams)
{
  Serial.println("GPIO Task");
  while (true)
  {
    // Serial.println("GPIO : Here!");
    Handler::GET().resolveData();
    vTaskDelay(pdMS_TO_TICKS(300));
  }
  vTaskDelete(NULL);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
