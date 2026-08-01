/**
 * WeatherStation
 *
 * A small weather station: temperature/humidity from a DHT22, optional
 * rain/light sensors, and a battery level. Uses the Industrial theme (bars
 * graph) to show the different look.
 *
 * Install via Library Manager: "DHT sensor library" by Adafruit.
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <OLEDDashboard.h>
#include <Wire.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define RAIN_PIN A0   // analog rain sensor (optional)

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

void setup() {
    Serial.begin(115200);
    dht.begin();

    dashboard.begin();
    dashboard.setTheme(OledDashboard::ThemeId::kIndustrial);
    dashboard.setBattery(70);
}

void loop() {
    const float h = dht.readHumidity();
    const float t = dht.readTemperature();

    if (!isnan(h) && !isnan(t)) {
        dashboard.setTemperature(t);
        dashboard.setHumidity(h);
        dashboard.addTemperatureHistory(t);
    }

    // Rough "outdoor" indicator: show 1 bar when raining, 4 when dry.
    const int rain = analogRead(RAIN_PIN);  // lower = wetter
    dashboard.setWiFi(rain > 500 ? 4 : 1);
    dashboard.update();

    delay(2000);
}
