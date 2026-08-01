// Mock Adafruit_GFX.h for host-side compile verification only.
#ifndef MOCK_ADAFRUIT_GFX_H
#define MOCK_ADAFRUIT_GFX_H

#include <Arduino.h>

class Adafruit_GFX {
public:
    Adafruit_GFX(int16_t w, int16_t h) : _width(w), _height(h) {}
    virtual ~Adafruit_GFX();

    int16_t width() const { return _width; }
    int16_t height() const { return _height; }

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                          uint16_t color);
    virtual void drawFastHLine(int16_t x0, int16_t y0, int16_t w,
                               uint16_t color);
    virtual void drawFastVLine(int16_t x0, int16_t y0, int16_t h,
                               uint16_t color);
    virtual void fillRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                          uint16_t color);

protected:
    int16_t _width;
    int16_t _height;
};

#endif  // MOCK_ADAFRUIT_GFX_H
