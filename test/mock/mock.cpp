// Mock implementations for host-side compile verification only.
// Renders into a real 128x64 monochrome buffer so the dashboard output can be
// inspected as ASCII art (see printFrame()).
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#include <stdio.h>

TwoWire Wire;

MockSerial Serial;

MockWiFiClass WiFi;

Adafruit_GFX::~Adafruit_GFX() {}

// ---------------------------------------------------------------------------
// Real software framebuffer (only used for host verification)
// ---------------------------------------------------------------------------

static void setpx(Adafruit_SSD1306* d, int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || x >= d->width() || y < 0 || y >= d->height()) return;
    uint8_t& byte = d->pixel(x, y);
    const uint8_t bit = 1 << (y & 7);
    if (color) {
        byte |= bit;
    } else {
        byte &= (uint8_t)~bit;
    }
}

void Adafruit_SSD1306::drawPixel(int16_t x, int16_t y, uint16_t color) {
    setpx(this, x, y, color);
}

void Adafruit_GFX::drawPixel(int16_t x, int16_t y, uint16_t color) {
    setpx(static_cast<Adafruit_SSD1306*>(this), x, y, color);
}

static void hline(Adafruit_SSD1306* d, int16_t x0, int16_t y, int16_t w,
                  uint16_t color) {
    if (y < 0 || y >= d->height()) return;
    if (x0 < 0) {
        w += x0;
        x0 = 0;
    }
    if (x0 + w > d->width()) w = d->width() - x0;
    for (int16_t x = x0; x < x0 + w; ++x) setpx(d, x, y, color);
}

static void vline(Adafruit_SSD1306* d, int16_t x, int16_t y0, int16_t h,
                  uint16_t color) {
    if (x < 0 || x >= d->width()) return;
    if (y0 < 0) {
        h += y0;
        y0 = 0;
    }
    if (y0 + h > d->height()) h = d->height() - y0;
    for (int16_t y = y0; y < y0 + h; ++y) setpx(d, x, y, color);
}

void Adafruit_GFX::drawFastHLine(int16_t x0, int16_t y0, int16_t w,
                                 uint16_t color) {
    hline(static_cast<Adafruit_SSD1306*>(this), x0, y0, w, color);
}

void Adafruit_GFX::drawFastVLine(int16_t x0, int16_t y0, int16_t h,
                                 uint16_t color) {
    vline(static_cast<Adafruit_SSD1306*>(this), x0, y0, h, color);
}

void Adafruit_GFX::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                            uint16_t color) {
    // Bresenham
    int16_t dx = x1 > x0 ? x1 - x0 : x0 - x1;
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = y1 > y0 ? y0 - y1 : y1 - y0;
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy;
    for (;;) {
        setpx(static_cast<Adafruit_SSD1306*>(this), x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int16_t e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Adafruit_GFX::fillRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                            uint16_t color) {
    if (x0 < 0) {
        w += x0;
        x0 = 0;
    }
    if (y0 < 0) {
        h += y0;
        y0 = 0;
    }
    for (int16_t y = y0; y < y0 + h; ++y) hline(static_cast<Adafruit_SSD1306*>(this), x0, y, w, color);
}

Adafruit_SSD1306::Adafruit_SSD1306(int16_t w, int16_t h, TwoWire* wire,
                                   int8_t reset)
    : Adafruit_GFX(w, h), wire_(wire) {
    (void)reset;
    memset(buffer_, 0, sizeof(buffer_));
}

bool Adafruit_SSD1306::begin(uint8_t vccstate, uint8_t i2caddr) {
    (void)vccstate;
    (void)i2caddr;
    return true;
}

void Adafruit_SSD1306::clearDisplay() { memset(buffer_, 0, sizeof(buffer_)); }

void Adafruit_SSD1306::display() {}

uint8_t& Adafruit_SSD1306::pixel(int16_t x, int16_t y) {
    // SSD1306 page-major layout: column-major within pages of 8 rows.
    return buffer_[(y >> 3) * 128 + x];
}

void Adafruit_SSD1306::printFrame() {
    for (int16_t y = 0; y < 64; ++y) {
        for (int16_t x = 0; x < 128; ++x) {
            putchar((pixel(x, y) & (1 << (y & 7))) ? '#' : '.');
        }
        putchar('\n');
    }
}
