/**
 * DHT22
 *
 * Temperature + humidity dashboard fed by a DHT22 (AM2302) sensor.
 * Install via Library Manager: "DHT sensor library" by Adafruit.
 *
 * Wiring:
 *   DHT22 VCC -> 3.3V/5V
 *   DHT22 GND -> GND
 *   DHT22 DAT -> D2 (10k pull-up to VCC)
 *   OLED SDA  -> A4, SCL -> A5 (Uno/Nano)
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <OLEDDashboard.h>
#include <Wire.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

void setup() {
    Serial.begin(115200);
    dht.begin();

    dashboard.begin();
    dashboard.setWiFi(2);
    dashboard.setBattery(65);
}

void loop() {
    const float h = dht.readHumidity();
    const float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        // DHT22 occasionally fails; skip this cycle.
        delay(2000);
        return;
    }

    dashboard.setTemperature(t);
    dashboard.setHumidity(h);
    dashboard.addTemperatureHistory(t);
    dashboard.update();

    delay(2000);
}
