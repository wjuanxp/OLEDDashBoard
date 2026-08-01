/**
 * RandomData
 *
 * Demonstrates every graph style and a simulated signal source, useful for
 * experimenting with themes and graph rendering without external sensors.
 *
 * Press/release digital pin D2 (with a button to GND) to cycle graph styles:
 *   Fade -> Line -> Bars -> Filled -> Dots
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OLEDDashboard.h>
#include <Wire.h>

#define MODE_BUTTON 2

Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

static OledDashboard::GraphStyle gStyles[] = {
    OledDashboard::GraphStyle::kFade,
    OledDashboard::GraphStyle::kLine,
    OledDashboard::GraphStyle::kBars,
    OledDashboard::GraphStyle::kFilled,
    OledDashboard::GraphStyle::kDots,
};
static uint8_t gStyleIndex = 0;

void setup() {
    pinMode(MODE_BUTTON, INPUT_PULLUP);

    dashboard.begin();
    dashboard.setWiFi(1 + (millis() / 1000) % 4);
    dashboard.setBattery(45);
}

void loop() {
    static bool lastPressed = true;
    const bool pressed = (digitalRead(MODE_BUTTON) == LOW);
    if (pressed && !lastPressed) {
        gStyleIndex = (gStyleIndex + 1) % (sizeof(gStyles) / sizeof(gStyles[0]));
        dashboard.setGraphStyle(gStyles[gStyleIndex]);
    }
    lastPressed = pressed;

    // A drifting "sensor" signal with a little noise.
    const float base = 22.0f + 3.0f * sinf((float)millis() / 3000.0f);
    const float t = base + ((float)(millis() % 100) - 50.0f) / 50.0f;
    const float h = 40.0f + 20.0f * (float)(millis() % 1000) / 1000.0f;

    dashboard.setTemperature(t);
    dashboard.setHumidity(h);
    dashboard.addTemperatureHistory(t);
    dashboard.update();

    delay(250);
}
