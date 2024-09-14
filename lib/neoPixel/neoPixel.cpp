#include "neoPixel.h"
#include "customConfig.h"

NeoPixel::NeoPixel()
{
    pixels = *new Adafruit_NeoPixel(noPixels, CustomConfig::GET().neoPixel_pin, NEO_GRB + NEO_KHZ800);
}

void NeoPixel::initialize()
{
    pixels.begin();
    pixels.setBrightness(5);
}

void NeoPixel::stopAll()
{
    pixels.clear();
    pixels.show();
}

void NeoPixel::blinkAll(int r, int g, int b)
{
    pixels.clear();
    for (int i = 0; i < noPixels; i++)
    {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
        pixels.show();
    }
}
