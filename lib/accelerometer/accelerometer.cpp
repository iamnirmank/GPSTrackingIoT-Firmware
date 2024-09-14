#include "accelerometer.h"
#include <Arduino.h>
#include <Wire.h>
#include <vector>
#include "handler.h"
#include "neoPixel.h"
#include "gpio.h"

void Accelerometer::initialize()
{
    // Accelerometer
    if (!Wire.begin(8, 9))
    {
        Serial.println("Wire not begined");
    };

    Wire.beginTransmission(0x53);
    Wire.write(0x2C);
    Wire.write(0x08);
    Wire.endTransmission();

    Wire.beginTransmission(0x53);
    Wire.write(0x31);
    Wire.write(0x08);
    Wire.endTransmission();

    Wire.beginTransmission(0x53);
    Wire.write(0x2D);
    Wire.write(0x08);
    Wire.endTransmission();
}

void Accelerometer::handleAccelerometer()
{

    std::vector<double> values = getVal();
    for (int i = 0; i < values.size(); i++)
    {
        if (values.at(i) > 2)
        {
            Handler::GET().handleRequest(E, data_format(
                                                values));
            NeoPixel::GET().blinkAll(255, 0, 0);
            GPIO::GET().handleBuzzer(GPIO::ALERT);
            delay(1000);
            NeoPixel::GET().stopAll();
        }
    }
}

std::vector<double> Accelerometer::getVal()
{
    Wire.beginTransmission(0x53);
    Wire.write(0x32);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte x0 = Wire.read();

    Wire.beginTransmission(0x53);
    Wire.write(0x33);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte x1 = Wire.read();
    x1 = x1 & 0x03;

    uint16_t x = (x1 << 8) + x0;
    int16_t xf = x;
    if (xf > 511)
    {
        xf = xf - 1024;
    }
    double xa = xf * 0.004;

    Wire.beginTransmission(0x53);
    Wire.write(0x34);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);

    byte y0 = Wire.read();

    Wire.beginTransmission(0x53);
    Wire.write(0x35);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte y1 = Wire.read();
    y1 = y1 & 0x03;

    uint16_t y = (y1 << 8) + y0;
    int16_t yf = y;
    if (yf > 511)
    {
        yf = yf - 1024;
    }
    double ya = yf * 0.004;

    Wire.beginTransmission(0x53);
    Wire.write(0x36);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte z0 = Wire.read();

    Wire.beginTransmission(0x53);
    Wire.write(0x37);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte z1 = Wire.read();
    z1 = z1 & 0x03;

    uint16_t z = (z1 << 8) + z0;
    int16_t zf = z;
    if (zf > 511)
    {
        zf = zf - 1024;
    }
    double za = zf * 0.004;
    // Serial.println(xa);
    // Serial.println(ya);
    // Serial.println(za);

    return {xa, ya, za};
}