/**
 * AHT30
 *
 * Temperature + humidity dashboard fed by an AHT20/AHT30 sensor.
 *
 * Uses the lightweight, allocation-free AHT20 driver (dvarrel/AHT20) which is
 * protocol-compatible with the AHT30 (same I2C address 0x38). Install via
 * Library Manager: "AHT20". It is used instead of the heavy Adafruit_AHTX0
 * driver so the sketch (plus the 1024-byte SSD1306 framebuffer) stays within
 * an ATmega328P's 2 KB of SRAM.
 *
 * Wiring:
 *   AHT30 VIN  -> 3.3V/5V
 *   AHT30 GND  -> GND
 *   AHT30 SDA  -> A4 (Uno/Nano)
 *   AHT30 SCL  -> A5 (Uno/Nano)
 *   OLED  SDA  -> A4, SCL -> A5
 */

#include <AHT20.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OLEDDashboard.h>
#include <Wire.h>

AHT20 aht;
Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

void setup() {
    Serial.begin(115200);

    // The AHT20 driver does not start Wire itself, so initialise I2C first.
    Wire.begin();

    // Bring the OLED up first so the screen is never left blank, even if the
    // sensor is missing.
    dashboard.begin();
    dashboard.setWiFi(3);
    dashboard.setBattery(80);

    if (!aht.begin()) {
        Serial.println("AHT30 not found - check wiring");
        // Do not hang here: keep the dashboard running so the display shows
        // something, and the serial message points at the fault.
        dashboard.setTemperature(0.0f);
        dashboard.setHumidity(0.0f);
    } else {
        Serial.println("AHT30 ready");
    }
}

void loop() {
    const float t = aht.getTemperature();
    const float h = aht.getHumidity();

    dashboard.setTemperature(t);
    dashboard.setHumidity(h);
    dashboard.addTemperatureHistory(t);
    dashboard.update();

    delay(1000);
}
