/**
 * @file Layout.h
 * @brief Fixed layout geometry for the OLEDDashboard reference UI.
 *
 * The reference screen is 128x64. The top "sensor area" is split into a
 * left information panel (104px) and a right status panel (24px) separated by
 * a vertical divider. Below sits the historical graph and a footer row.
 *
 * @section Geometry
 *   +-------------------------------------------------+
 *   | Temp      23.6 C    |  WiFi        <- sensor    |
 *   +--------------------------|                     |
 *   | Humidity  45.2%      | Battery 92%             |
 *   +--------------------------+                     |
 *   |                                                 |
 *   |        Historical Fade Graph   (16px)          |
 *   |                                                 |
 *   | Min:22.9   Max:24.1   Avg:23.4   <- footer      |
 *   +-------------------------------------------------+
 */

#ifndef OLED_DASHBOARD_LAYOUT_H
#define OLED_DASHBOARD_LAYOUT_H

#include <stdint.h>

namespace OledDashboard {

/**
 * @brief Screen metrics for the reference 128x64 panel.
 *
 * All values are compile-time constants (constexpr) so the layout costs zero
 * RAM and the compiler can fold everything into the call sites.
 */
struct Layout {
    /// Display width in pixels (SSD1306 128x64).
    static constexpr int16_t kScreenW = 128;
    /// Display height in pixels.
    static constexpr int16_t kScreenH = 64;

    /// Width of the left information panel (temperature/humidity).
    static constexpr int16_t kLeftPanelW = 104;
    /// Width of the right status panel (WiFi / battery).
    static constexpr int16_t kRightPanelW = kScreenW - kLeftPanelW;  // 24
    /// X coordinate of the vertical divider between the two panels.
    static constexpr int16_t kDividerX = kLeftPanelW;

    /// Y of the temperature row (top of screen).
    static constexpr int16_t kTempRowY = 0;
    /// Height of the temperature row. The 8x16 large font is drawn inset so
    /// the digits keep ~1 px of clearance from the screen top and divider.
    static constexpr int16_t kTempRowH = 18;
    /// Y of the horizontal divider between temperature and humidity.
    static constexpr int16_t kDivider1Y = kTempRowY + kTempRowH;
    /// Y of the humidity row.
    static constexpr int16_t kHumidityRowY = kDivider1Y + 1;
    /// Height of the humidity row.
    static constexpr int16_t kHumidityRowH = 18;
    /// Y of the divider above the graph area.
    static constexpr int16_t kDivider2Y = kHumidityRowY + kHumidityRowH;
    /// Y of the vertical divider span (sensor area height).
    static constexpr int16_t kSensorAreaH = kDivider2Y + 1;

    /// Y of the top of the graph area.
    static constexpr int16_t kGraphY = kDivider2Y + 1;
    /// Height of the graph area.
    static constexpr int16_t kGraphH = 16;
    /// Bottom (exclusive) of the graph drawing region.
    static constexpr int16_t kGraphBottom = kGraphY + kGraphH;

    /// Y of the footer statistics row. Height is 9 so the compact text has
    /// ~2 px of clearance from the graph and the bottom edge.
    static constexpr int16_t kFooterY = kGraphBottom + 1;

    /// Horizontal padding used inside rows (text/icon insets).
    static constexpr int16_t kPadding = 2;
    /// Right inset for the big numeric values.
    static constexpr int16_t kValueRightInset = 3;
    /// Recommended history length. 16 keeps the library (plus the 1024-byte
    /// SSD1306 framebuffer) within an ATmega328P's 2 KB SRAM budget with stack
    /// headroom; raise it to 128 (one sample per pixel column) on boards with
    /// more RAM (Mega, ESP32).
    static constexpr uint16_t kDefaultHistory = 16;
};

}  // namespace OledDashboard

#endif  // OLED_DASHBOARD_LAYOUT_H
