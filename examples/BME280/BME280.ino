/**
 * BME280
 *
 * Dashboard fed by a BME280 (temperature, humidity, pressure).
 * Install via Library Manager: "Adafruit BME280 Library".
 *
 * Wiring:
 *   BME280 VIN -> 3.3V
 *   BME280 GND -> GND
 *   BME280 SDA -> A4, SCL -> A5 (Uno/Nano)
 *   OLED  SDA  -> A4, SCL -> A5
 */

#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OLEDDashboard.h>
#include <Wire.h>

Adafruit_BME280 bme;
Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

void setup() {
    Serial.begin(115200);

    if (!bme.begin(0x76)) {
        Serial.println("BME280 not found - check wiring/address");
        while (1) delay(100);
    }

    dashboard.begin();
    dashboard.setWiFi(4);
    dashboard.setBattery(55);
}

void loop() {
    dashboard.setTemperature(bme.readTemperature());
    dashboard.setHumidity(bme.readHumidity());
    dashboard.addTemperatureHistory(bme.readTemperature());
    dashboard.update();

    // The graph keeps the last 128 samples: 128 seconds at 1 Hz.
    delay(1000);
}
