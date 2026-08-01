// Mock Adafruit_AHTX0.h + Adafruit_Sensor bits for host compile verification.
#ifndef MOCK_ADAFRUIT_AHTX0_H
#define MOCK_ADAFRUIT_AHTX0_H

#include <Arduino.h>

typedef struct {
    int32_t version;
    int32_t sensor_id;
    int32_t type;
    int32_t reserved0;
    int32_t timestamp;
    union {
        float data[4];
        struct {
            float x, y, z;
            float temperature;
            float relative_humidity;
        };
    };
} sensors_event_t;

class Adafruit_AHTX0 {
public:
    Adafruit_AHTX0() {}
    bool begin() { return true; }
    void getEvent(sensors_event_t* humidity, sensors_event_t* temp) {
        (void)humidity;
        temp->temperature = 23.0f;
    }
};

#endif  // MOCK_ADAFRUIT_AHTX0_H
