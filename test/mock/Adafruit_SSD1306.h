// Mock Adafruit_SSD1306.h for host-side compile verification only.
#ifndef MOCK_ADAFRUIT_SSD1306_H
#define MOCK_ADAFRUIT_SSD1306_H

#include <Adafruit_GFX.h>
#include <Wire.h>

#define SSD1306_SWITCHCAPVCC 0x02
#define SSD1306_I2C_ADDRESS 0x3C

class Adafruit_SSD1306 : public Adafruit_GFX {
public:
    Adafruit_SSD1306(int16_t w, int16_t h, TwoWire* wire, int8_t reset);

    virtual bool begin(uint8_t vccstate, uint8_t i2caddr);
    void clearDisplay();
    void display();

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);

    // Host-verification helpers.
    uint8_t& pixel(int16_t x, int16_t y);
    void printFrame();

private:
    uint8_t buffer_[128 * 64 / 8];
    TwoWire* wire_;
};

#endif  // MOCK_ADAFRUIT_SSD1306_H
