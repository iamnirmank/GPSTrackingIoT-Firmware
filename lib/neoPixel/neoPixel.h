#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "customConfig.h"

class NeoPixel
{
public:
    static NeoPixel &GET()
    {
        static NeoPixel instance;
        return instance;
    }
    void blinkAll(int r, int g, int b);
    void initialize();
    void stopAll();
    Adafruit_NeoPixel pixels;

private:
    NeoPixel();
    const int noPixels = 3;
};