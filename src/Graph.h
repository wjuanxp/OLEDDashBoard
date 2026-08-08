/**
 * @file Graph.h
 * @brief Historical graph engine for OLEDDashboard.
 *
 * The graph renders a rolling history of sensor samples as a smooth,
 * auto-scaled trace. The default style is the signature "fade" graph: under
 * each data point a dithered column fades downward, producing a soft glow
 * that simulates grayscale on the monochrome SSD1306.
 *
 * Key properties:
 *   - No dynamic allocation; history lives in a fixed ring buffer.
 *   - Samples are int16_t scaled by ten (23.6 -> 236).
 *   - Auto scaling with margin: data always fills the graph height, never
 *     clipped.
 *   - Smooth scrolling: the newest sample always enters at the right edge and
 *     the whole trace shifts left by one pixel per sample.
 *   - Five styles: line, fade, bars, filled, dots.
 */

#ifndef OLED_DASHBOARD_GRAPH_H
#define OLED_DASHBOARD_GRAPH_H

#include <stdint.h>

#include <Adafruit_GFX.h>

#include "DashboardTheme.h"
#include "Layout.h"
#include "Statistics.h"
#include "Widgets.h"

namespace OledDashboard {

/**
 * @brief A scrolling, auto-scaling history graph widget.
 */
class Graph : public Widget {
public:
    /**
     * @brief Construct a graph.
     * @param historySize  Number of samples retained (<= Statistics::kMaxCapacity).
     *                     Defaults to the full screen width (128) so each new
     *                     sample shifts the trace by exactly one pixel.
     */
    explicit Graph(uint16_t historySize = Layout::kDefaultHistory);

    /// Append one sample (already scaled by ten).
    void addSample(int16_t valueX10);

    /// Clear all history.
    void clear();

    /// Change the retained history length (1..Statistics::kMaxCapacity).
    /// Clears existing data.
    void setHistorySize(uint16_t size) {
        if (size != stats_.capacity()) {
            stats_.setCapacity(size);
            setDirty();
        }
    }

    /// Select the rendering style.
    void setStyle(GraphStyle style) {
        if (style_ != style) setDirty();
        style_ = style;
    }

    /// Current rendering style.
    GraphStyle style() const { return style_; }

    /// Access to the underlying history buffer (read-only statistics).
    const Statistics& statistics() const { return stats_; }

    // -- Widget interface ---------------------------------------------------
    void update() override;  // nothing to do: history is pushed externally
    void draw(Adafruit_GFX& gfx) override;

    /// Configure graph bounds. Overrides Widget::setBounds to keep the ring
    /// buffer aligned with the visible width.
    void setBounds(int16_t x, int16_t y, int16_t w, int16_t h) override;

private:
    // Map a normalized value [0..1] onto a pixel row within the graph area.
    int16_t valueToY(int32_t value) const;

    // X position (screen column) of sample idx when the n samples are
    // stretched across the full graph width.
    int16_t sampleX(uint16_t idx, uint16_t n) const;

    // Linearly interpolated sample value at a given screen column, so a
    // history shorter than the graph width still spans the whole panel.
    int16_t valueAtColumn(int16_t x) const;

    void drawLine(Adafruit_GFX& gfx);
    void drawFade(Adafruit_GFX& gfx);
    void drawBars(Adafruit_GFX& gfx);
    void drawDots(Adafruit_GFX& gfx);

    Statistics stats_;
    GraphStyle style_;
    int16_t minScaled_;   ///< Auto-scaled minimum (with margin).
    int16_t maxScaled_;   ///< Auto-scaled maximum (with margin).
};

}  // namespace OledDashboard

#endif  // OLED_DASHBOARD_GRAPH_H
