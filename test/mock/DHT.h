// Mock DHT.h for host compile verification.
#ifndef MOCK_DHT_H
#define MOCK_DHT_H

#include <Arduino.h>

#define DHT11 11
#define DHT22 22
#define DHT21 21

class DHT {
public:
    DHT(uint8_t pin, uint8_t type) : pin_(pin), type_(type) {}
    void begin() {}
    float readHumidity() { return 45.0f; }
    float readTemperature() { return 23.0f; }

private:
    uint8_t pin_;
    uint8_t type_;
};

#endif  // MOCK_DHT_H
