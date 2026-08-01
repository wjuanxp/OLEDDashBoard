/**
 * @file DashboardTheme.h
 * @brief Theme engine for OLEDDashboard.
 *
 * A theme centralises every visual decision of the dashboard:
 *   - fonts used for values, labels and the footer,
 *   - layout spacing,
 *   - whether dividers / panel borders are drawn,
 *   - the default graph style and fade behaviour,
 *   - icon visibility.
 *
 * Themes are plain data structs returned by value, so switching themes costs
 * no heap and no global mutable state.
 */

#ifndef OLED_DASHBOARD_THEME_H
#define OLED_DASHBOARD_THEME_H

#include <stdint.h>

namespace OledDashboard {

/// Font roles available in the theme.
enum class FontRole : uint8_t {
    kValue,      ///< Large, prominent numbers.
    kLabel,      ///< Small row labels ("Temp", "Humidity").
    kFooter,     ///< Footer statistics row.
    kGraphAxis,  ///< Reserved for future graph annotations.
};

/// Font sizes the theme can select.
enum class FontSize : uint8_t {
    kSmall,   ///< 5x7 custom font.
    kMedium,  ///< Medium emphasis (falls back to small for now).
    kLarge,   ///< Large emphasis.
    kLargeBold,  ///< 8x16 large bold custom font.
};

/// Graph rendering styles (see Graph.h).
enum class GraphStyle : uint8_t {
    kLine,    ///< One-pixel interpolated line.
    kFade,    ///< Dithered fading columns (default).
    kBars,    ///< Vertical bars.
    kFilled,  ///< Solid area fill.
    kDots,    ///< Points only.
};

/// Named themes shipped with the library.
enum class ThemeId : uint8_t {
    kProfessional,  ///< Default: crisp dividers, fade graph, all icons.
    kMinimal,       ///< No dividers, line graph.
    kIndustrial,    ///< Bars graph, bright accents.
    kDark,          ///< Inverted emphasis, filled graph.
    kModern,        ///< Dots graph, no panel borders.
};

/**
 * @brief Immutable visual configuration of a dashboard.
 *
 * Monochrome displays only ever use on/off pixels, but foreground/background
 * are kept as explicit fields so a future colour backend needs no API change.
 */
struct Theme {
    /// Foreground pixel value (1 on monochrome).
    uint8_t foreground = 1;
    /// Background pixel value (0 on monochrome).
    uint8_t background = 0;

    /// Font used for big values (temperature / humidity).
    FontSize valueFont = FontSize::kLargeBold;
    /// Font used for small labels.
    FontSize labelFont = FontSize::kSmall;
    /// Font used for the footer statistics.
    FontSize footerFont = FontSize::kSmall;

    /// Default graph style for new graphs.
    GraphStyle graphStyle = GraphStyle::kFade;
    /// Whether the graph uses the dithered fade effect (kFade already does).
    bool graphFade = true;

    /// Draw horizontal dividers and the vertical panel separator.
    bool showDividers = true;
    /// Draw a thin border around the graph area.
    bool showGraphBorder = false;

    /// Show the WiFi status icon.
    bool wifiIconEnabled = true;
    /// Show the battery status icon.
    bool batteryIconEnabled = true;
    /// Show the low-battery warning glyph below 20%.
    bool batteryLowWarning = true;

    /// Append unit glyphs (deg C, percent) to values.
    bool showValueUnits = true;
    /// Horizontal spacing (px) around labels/icons.
    int8_t padding = 2;

    /// Layout row padding adjustment in px (0 keeps the reference layout).
    int8_t rowPadding = 0;
};

/**
 * @brief Return the built-in theme requested by id.
 * @param id  The theme to fetch.
 * @return A value copy of the theme.
 */
Theme themeFor(ThemeId id);

/// Convenience: the default (Professional) theme.
inline Theme defaultTheme() { return themeFor(ThemeId::kProfessional); }

}  // namespace OledDashboard

#endif  // OLED_DASHBOARD_THEME_H
