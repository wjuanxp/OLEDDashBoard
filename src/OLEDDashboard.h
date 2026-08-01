/**
 * @file OLEDDashboard.h
 * @brief Professional dashboard UI library for SSD1306 OLED displays.
 *
 * OLEDDashboard is a widget-based dashboard framework for IoT and sensor
 * projects. Users only provide data; the library handles layout, rendering,
 * auto-scaling, icons, theming and historical visualisation.
 *
 * Example:
 * @code
 * #include <OLEDDashboard.h>
 *
 * Adafruit_SSD1306 display(128, 64, &Wire, -1);
 * OledDashboard::OLEDDashboard dashboard(display);
 *
 * void setup() {
 *     dashboard.begin();
 *     dashboard.setWiFi(4);
 *     dashboard.setBattery(92);
 * }
 *
 * void loop() {
 *     dashboard.setTemperature(23.6);
 *     dashboard.setHumidity(45.2);
 *     dashboard.addTemperatureHistory(23.6);
 *     dashboard.update();          // redraw changed widgets + display()
 *     delay(1000);
 * }
 * @endcode
 *
 * The library never allocates memory dynamically and keeps its working set
 * well inside the Arduino Uno's 2 KB of SRAM.
 */

#ifndef OLED_DASHBOARD_H
#define OLED_DASHBOARD_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "DashboardTheme.h"
#include "Graph.h"
#include "Layout.h"
#include "Statistics.h"
#include "Widgets.h"

namespace OledDashboard {

/**
 * @brief The top-level dashboard. Owns all widgets and the render loop.
 */
class OLEDDashboard {
public:
    /**
     * @brief Construct a dashboard bound to a display object.
     * @param display  An Adafruit_SSD1306 instance (constructed by the user).
     */
    explicit OLEDDashboard(Adafruit_SSD1306& display);

    /**
     * @brief Initialise the display and draw the first frame.
     * @param i2cAddr  Display I2C address (default 0x3C).
     */
    void begin(uint8_t i2cAddr = 0x3C);

    // ------------------------------------------------------------------
    // Data entry (simple API)
    // ------------------------------------------------------------------

    /// Set the temperature, e.g. 23.6 (degrees Celsius).
    void setTemperature(float value) {
        temperature().value(value);
    }

    /// Set the relative humidity, e.g. 45.2 (%).
    void setHumidity(float value) { humidity().value(value); }

    /// Set the battery level, 0..100 (%).
    void setBattery(uint8_t percent) { battery().percent(percent); }

    /// Mark the battery as charging / not charging.
    void setBatteryCharging(bool charging) { battery().charging(charging); }

    /// Set the WiFi signal strength, 0..4.
    void setWiFi(uint8_t strength) { wifi().strength(strength); }

    /// Append one temperature history sample (e.g. 23.6).
    void addTemperatureHistory(float temperature) {
        graph().addSample(floatToX10(temperature));
    }

    // ------------------------------------------------------------------
    // Configuration
    // ------------------------------------------------------------------

    /// Apply a built-in theme.
    void setTheme(ThemeId id) { setTheme(themeFor(id)); }

    /// Apply a custom theme.
    void setTheme(const Theme& theme);

    /// Select the graph rendering style (fade by default).
    void setGraphStyle(GraphStyle style) { graph().setStyle(style); }

    /// Set the temperature display unit (°C or °F). The temperature value is
    /// converted to the selected unit; the footer stats follow the same unit.
    void setTemperatureUnit(ValueUnit unit) {
        temp_.setUnit(unit);
        footer_.setTemperatureUnit(unit);
    }

    /// Set the number of samples retained in the history (<= 128).
    void setGraphHistorySize(uint16_t size) {
        graph().setHistorySize(size);
    }

    // ------------------------------------------------------------------
    // Rendering
    // ------------------------------------------------------------------

    /**
     * @brief Update all widgets and push the frame to the display.
     *
     * Only dirty (changed) widgets are redrawn; display.display() is called
     * exactly once per call.
     */
    void update();

    /// Force a full redraw of every widget.
    void draw();

    // ------------------------------------------------------------------
    // Fluent API
    // ------------------------------------------------------------------

    /// Access the temperature widget: dashboard.temperature().value(23.6);
    SensorValueWidget& temperature() { return temp_; }
    /// Access the humidity widget: dashboard.humidity().value(45.2);
    SensorValueWidget& humidity() { return humidity_; }
    /// Access the WiFi widget: dashboard.wifi().strength(4);
    WifiWidget& wifi() { return wifi_; }
    /// Access the battery widget: dashboard.battery().percent(92);
    BatteryWidget& battery() { return battery_; }
    /// Access the graph widget (add samples, change style, ...).
    Graph& graph() { return graph_; }

    /// Current theme in effect.
    const Theme& theme() const { return theme_; }

private:
    void applyTheme();

    Adafruit_SSD1306& display_;
    Theme theme_;

    SensorValueWidget temp_;
    SensorValueWidget humidity_;
    DividerWidget divider_;
    WifiWidget wifi_;
    BatteryWidget battery_;
    Graph graph_;
    FooterWidget footer_;
};

}  // namespace OledDashboard

#endif  // OLED_DASHBOARD_H
