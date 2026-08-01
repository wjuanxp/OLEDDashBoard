// Mock Wire.h for host-side compile verification only.
#ifndef MOCK_WIRE_H
#define MOCK_WIRE_H

#include <Arduino.h>

class TwoWire {
public:
    void begin() {}
    void setClock(uint32_t) {}
    void beginTransmission(uint8_t) {}
    uint8_t endTransmission() { return 0; }
    size_t write(uint8_t) { return 1; }
    size_t write(const uint8_t*, size_t n) { return n; }
    void requestFrom(uint8_t, uint8_t) {}
    void requestFrom(uint8_t, uint8_t, uint8_t) {}
    uint8_t available() { return 0; }
    uint8_t read() { return 0; }
};

extern TwoWire Wire;

#endif  // MOCK_WIRE_H
