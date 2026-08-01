/**
 * BasicDashboard
 *
 * The reference OLEDDashboard example. Simulates a temperature / humidity
 * sensor so the full UI can be seen without any hardware besides the display.
 *
 * Wiring: SSD1306 128x64 I2C display -> A4 (SDA), A5 (SCL) on Uno/Nano.
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OLEDDashboard.h>
#include <Wire.h>

// Display object (128x64, I2C, no reset pin).
Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

void setup() {
    Serial.begin(115200);
    dashboard.begin();  // initialises the display and draws the first frame

    dashboard.setWiFi(4);
    dashboard.setBattery(92);
}

void loop() {
    // Simulate a slowly drifting temperature.
    static uint32_t lastMs = 0;
    static float t = 21.5f;
    const uint32_t now = millis();
    if (now - lastMs >= 1000) {
        lastMs = now;

        t += 0.1f * (float)((now / 1000) % 3) - 0.05f;
        const float h = 45.0f + 3.0f * (float)((now / 1000) % 5);

        dashboard.setTemperature(t);
        dashboard.setHumidity(h);
        dashboard.addTemperatureHistory(t);

        dashboard.update();
    }
}
