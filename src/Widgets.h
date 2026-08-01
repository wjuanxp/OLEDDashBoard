/**
 * @file Widgets.h
 * @brief Widget framework and concrete dashboard widgets.
 *
 * Every piece of the dashboard is a widget. Each widget owns a rectangular
 * region, knows how to update its internal state and how to draw itself, and
 * can be made invisible. Widgets track their own dirtiness so the dashboard
 * can avoid redrawing unchanged regions.
 *
 * The widget regions are carefully non-overlapping (see Layout.h): clearing a
 * widget rect to the background never erases a neighbouring widget or a
 * divider line.
 *
 * @file Widgets.h also exposes the internal bitmap text renderer used by all
 * widgets (custom 5x7 small font and 8x16 large bold font from Fonts.h).
 */

#ifndef OLED_DASHBOARD_WIDGETS_H
#define OLED_DASHBOARD_WIDGETS_H

#include <stdint.h>

#include <Adafruit_GFX.h>

#include "DashboardTheme.h"
#include "Layout.h"
#include "Statistics.h"

namespace OledDashboard {

/**
 * @brief Base class for all dashboard widgets.
 */
class Widget {
public:
    /// Construct an empty (invisible) widget.
    Widget() : x_(0), y_(0), w_(0), h_(0), visible_(false), dirty_(true) {}

    /// Construct a widget over the given screen rectangle.
    Widget(int16_t x, int16_t y, int16_t w, int16_t h)
        : x_(x), y_(y), w_(w), h_(h), visible_(true), dirty_(true) {}

    virtual ~Widget() {}

    /// Recompute any derived state (called before draw when dirty).
    virtual void update() = 0;

    /// Render the widget into the given canvas.
    virtual void draw(Adafruit_GFX& gfx) = 0;

    /// Reposition / resize the widget.
    virtual void setBounds(int16_t x, int16_t y, int16_t w, int16_t h) {
        x_ = x;
        y_ = y;
        w_ = w;
        h_ = h;
        setDirty();
    }

    /// Rectangle accessors.
    int16_t x() const { return x_; }
    int16_t y() const { return y_; }
    int16_t width() const { return w_; }
    int16_t height() const { return h_; }

    /// Show or hide the widget; changing visibility dirties it.
    void setVisible(bool visible) {
        if (visible_ != visible) setDirty();
        visible_ = visible;
    }
    bool visible() const { return visible_; }

    /// Mark the widget as needing a redraw.
    void setDirty() { dirty_ = true; }
    bool dirty() const { return dirty_; }
    void clearDirty() { dirty_ = false; }

protected:
    int16_t x_, y_, w_, h_;
    bool visible_;
    bool dirty_;
};

// ---------------------------------------------------------------------------
// Text renderer (custom bitmap fonts)
// ---------------------------------------------------------------------------

/**
 * @brief Measure the width of a string in the small 5x7 font.
 */
int16_t textWidthSmall(const char* s);

/**
 * @brief Measure the width of a string in the large 8x16 font.
 * Glyphs not present fall back to the small font.
 */
int16_t textWidthLarge(const char* s);

/**
 * @brief Measure the width of a string in the compact 4x6 font
 * (used by the footer). Unknown glyphs fall back to small.
 */
int16_t textWidthCompact(const char* s);

/**
 * @brief Draw a small (5x7) text string.
 * @return Width consumed in pixels.
 */
int16_t drawTextSmall(Adafruit_GFX& gfx, int16_t x, int16_t y,
                      const char* s, bool inverted = false);

/**
 * @brief Draw a large (8x16) text string. Unknown glyphs fall back to small.
 * @return Width consumed in pixels.
 */
int16_t drawTextLarge(Adafruit_GFX& gfx, int16_t x, int16_t y,
                      const char* s, bool inverted = false);

/**
 * @brief Draw a compact (4x6) text string. Unknown glyphs fall back to small.
 * @return Width consumed in pixels.
 */
int16_t drawTextCompact(Adafruit_GFX& gfx, int16_t x, int16_t y,
                        const char* s);

/**
 * @brief Format a value stored scaled-by-ten as "[-]d.d".
 * @param valueX10  e.g. -123 for -12.3.
 * @param out       Buffer to write into (must hold at least 8 chars).
 * @return Number of characters written.
 */
uint8_t formatX10(int16_t valueX10, char* out);

/**
 * @brief Convert a float (e.g. 23.6) to the scaled-by-ten integer used
 * internally (236). Rounds to the nearest 0.1.
 */
int16_t floatToX10(float v);

/**
 * @brief Pointer to the 4x4 ordered Bayer dither matrix (row-major, 16 values
 * 0..15) used by the fade graph. Stored in PROGMEM.
 */
const uint8_t* ditherMatrix();

// ---------------------------------------------------------------------------
// Concrete widgets
// ---------------------------------------------------------------------------

/// Unit annotation drawn after a sensor value.
enum ValueUnit : uint8_t {
    kUnitNone = 0,
    kUnitDegC,     ///< Draws the degree glyph + 'C'.
    kUnitPercent,  ///< Draws '%'.
};

/**
 * @brief One sensor row: a small label on the left and a large right-aligned
 * value on the right (temperature or humidity).
 */
class SensorValueWidget : public Widget {
public:
    SensorValueWidget(int16_t x, int16_t y, int16_t w, int16_t h,
                      const char* label, ValueUnit unit);

    /// Set the value (scaled by ten, e.g. 236 == 23.6).
    void setValue(int16_t valueX10) {
        if (value_ != valueX10) setDirty();
        value_ = valueX10;
    }

    /// Fluent alias accepting a float (e.g. 23.6).
    void value(float v) { setValue(floatToX10(v)); }

    /// Select the font used for the value.
    void setFont(FontSize font) {
        if (font_ != font) setDirty();
        font_ = font;
    }

    void update() override;
    void draw(Adafruit_GFX& gfx) override;

private:
    int16_t value_;
    const char* label_;
    ValueUnit unit_;
    FontSize font_;
};

/**
 * @brief The divider frame: two horizontal lines (between rows and above the
 * graph) plus the vertical panel separator. All lines end at the vertical
 * divider column.
 */
class DividerWidget : public Widget {
public:
    DividerWidget();

    void update() override;
    void draw(Adafruit_GFX& gfx) override;
};

/**
 * @brief WiFi signal indicator (right status panel).
 */
class WifiWidget : public Widget {
public:
    WifiWidget();

    /// Set signal strength 0..4.
    void setStrength(uint8_t level) {
        if (level > 4) level = 4;
        if (level_ != level) setDirty();
        level_ = level;
    }
    uint8_t strength() const { return level_; }

    /// Fluent alias.
    void strength(uint8_t level) { setStrength(level); }

    void update() override;
    void draw(Adafruit_GFX& gfx) override;

private:
    uint8_t level_;
};

/**
 * @brief Battery indicator: outline + proportional fill + optional charging
 * bolt and automatic low-battery warning.
 */
class BatteryWidget : public Widget {
public:
    BatteryWidget();

    /// Set battery percent 0..100.
    void setPercent(uint8_t percent) {
        if (percent > 100) percent = 100;
        if (percent_ != percent) setDirty();
        percent_ = percent;
    }
    /// Flag the battery as charging (draws the bolt).
    void setCharging(bool charging) {
        if (charging_ != charging) setDirty();
        charging_ = charging;
    }
    /// Enable/disable the automatic low-battery warning (< 20%).
    void setLowWarningEnabled(bool enabled) { lowWarning_ = enabled; }

    /// Fluent alias.
    void percent(uint8_t p) { setPercent(p); }
    /// Fluent alias.
    void charging(bool c) { setCharging(c); }

    void update() override;
    void draw(Adafruit_GFX& gfx) override;

private:
    uint8_t percent_;
    bool charging_;
    bool lowWarning_;
};

/**
 * @brief Footer statistics row: Min, Max, Avg from the graph history.
 */
class FooterWidget : public Widget {
public:
    FooterWidget();

    /// Feed the statistics the footer should summarise.
    void setStatistics(const Statistics* stats) { stats_ = stats; }

    void update() override;
    void draw(Adafruit_GFX& gfx) override;

private:
    const Statistics* stats_;
    int16_t minX10_;
    int16_t maxX10_;
    int32_t avgX10_;
};

}  // namespace OledDashboard

#endif  // OLED_DASHBOARD_WIDGETS_H
