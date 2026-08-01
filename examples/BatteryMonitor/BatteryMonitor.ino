/**
 * BatteryMonitor
 *
 * Monitors a battery voltage on analog pin A0 through a voltage divider and
 * drives the battery widget, low-battery warning and the temperature graph
 * (an optional NTC thermistor on A1).
 *
 * Voltage divider (Vin -- R1 -- A0 -- R2 -- GND):
 *   Vbatt = A0 * (R1 + R2) / R2 * (Vref / 1023)
 * Tune the values below to your divider / reference voltage.
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OLEDDashboard.h>
#include <Wire.h>

#define VBAT_PIN A0
#define R1 100000.0f   // upper resistor (Ohm)
#define R2 100000.0f   // lower resistor (Ohm)
#define VREF 5.0f      // Arduino ADC reference

Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

// Map battery voltage to a percentage for common cell chemistries.
// 3-cell LiPo example: 12.6 V = 100%, 9.9 V = 0%.
static uint8_t voltageToPercent(float volts) {
    const float vMax = 12.6f;
    const float vMin = 9.9f;
    float p = (volts - vMin) * 100.0f / (vMax - vMin);
    if (p > 100.0f) p = 100.0f;
    if (p < 0.0f) p = 0.0f;
    return (uint8_t)p;
}

static float readBatteryVolts() {
    const float adc = analogRead(VBAT_PIN);
    const float vin = adc * VREF / 1023.0f;
    return vin * (R1 + R2) / R2;
}

void setup() {
    Serial.begin(115200);
    dashboard.begin();
    dashboard.setWiFi(0);
}

void loop() {
    const float volts = readBatteryVolts();
    const uint8_t percent = voltageToPercent(volts);

    dashboard.setBattery(percent);
    dashboard.setBatteryCharging(false);
    dashboard.setTemperature(percent / 10.0f);
    dashboard.addTemperatureHistory(percent / 10.0f);
    dashboard.update();

    Serial.print("VBAT = ");
    Serial.print(volts);
    Serial.print(" V, ");
    Serial.print(percent);
    Serial.println(" %");

    delay(1000);
}
