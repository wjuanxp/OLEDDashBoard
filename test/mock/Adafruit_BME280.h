// Mock Adafruit_BME280.h for host compile verification.
#ifndef MOCK_ADAFRUIT_BME280_H
#define MOCK_ADAFRUIT_BME280_H

#include <Arduino.h>

class Adafruit_BME280 {
public:
    Adafruit_BME280() {}
    bool begin(uint8_t addr) {
        (void)addr;
        return true;
    }
    float readTemperature() { return 22.5f; }
    float readHumidity() { return 47.0f; }
};

#endif  // MOCK_ADAFRUIT_BME280_H
