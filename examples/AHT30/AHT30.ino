/**
 * AHT30
 *
 * Temperature + humidity dashboard fed by an AHT20/AHT30 sensor
 * (Adafruit AHT20 library). Install via Library Manager:
 *   "Adafruit AHT20"
 *
 * Wiring:
 *   AHT30 VIN  -> 3.3V/5V
 *   AHT30 GND  -> GND
 *   AHT30 SDA  -> A4 (Uno/Nano)
 *   AHT30 SCL  -> A5 (Uno/Nano)
 *   OLED  SDA  -> A4, SCL -> A5
 */

#include <Adafruit_AHTX0.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OLEDDashboard.h>
#include <Wire.h>

Adafruit_AHTX0 aht;
Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

void setup() {
    Serial.begin(115200);

    if (!aht.begin()) {
        Serial.println("AHT30 not found - check wiring");
        while (1) delay(100);
    }

    dashboard.begin();
    dashboard.setWiFi(3);
    dashboard.setBattery(80);
}

void loop() {
    sensors_event_t humidityEvent;
    sensors_event_t tempEvent;
    aht.getEvent(&humidityEvent, &tempEvent);

    dashboard.setTemperature(tempEvent.temperature);
    dashboard.setHumidity(humidityEvent.relative_humidity);
    dashboard.addTemperatureHistory(tempEvent.temperature);
    dashboard.update();

    delay(1000);
}
