# OLEDDashboard

A **professional-grade Arduino dashboard library** for SSD1306 OLED displays
(128x64, I²C). Give the library your sensor data; it handles layout, rendering,
auto-scaling, icons, theming and historical visualisation.

```
+-------------------------------------------------+
| Temp      23.6°C      |  WiFi                   |
+────────────────────────+                        |
| Humidity  45.2%        | Battery 92%           |
+────────────────────────+                        |
|                                                  |
|         Historical Fade Graph (20 px)          |
|                                                  |
| Min:17.0  Max:23.0  Avg:20.0                    |
+-------------------------------------------------+
```

## Features

- Widget-based architecture: temperature, humidity, WiFi, battery, divider,
  graph and footer — each owns its bounds, update and draw.
- **Signature fade graph**: smooth one-pixel line with dithered fading columns
  that simulate grayscale on the monochrome panel. Also ships line, bars,
  filled and dots styles.
- Auto-scaling with margin — data always fills the graph, never clipped.
- Smooth scrolling history (newest sample enters at the right edge).
- Built-in statistics: min / max / average, stored as `int16_t` (×10) in a
  fixed ring buffer — **no floats stored, no dynamic allocation**.
- Custom bitmap fonts: 5x7 small, 8x16 large-bold numeric, 4x6 compact footer.
- Custom WiFi (5 levels) and battery (fill + charging + low warning) icons.
- Theme engine with five shipped themes (Professional, Minimal, Industrial,
  Dark, Modern).
- Runs comfortably on an **Arduino Uno** (2 KB SRAM class).

## Requirements

- Arduino-compatible board (Uno, Nano, Pro Mini; ESP8266/ESP32 supported)
- SSD1306 128x64 I²C OLED display
- Libraries (Arduino Library Manager):
  - **Adafruit GFX Library**
  - **Adafruit SSD1306**

## Installation

1. Copy the `OLEDDashboard` folder into your Arduino `libraries` folder, or
   use the Library Manager (search "OLEDDashboard").
2. Install the two Adafruit dependencies above.
3. Open **File → Examples → OLEDDashboard → BasicDashboard**, select your
   board and upload.

## Quick start

```cpp
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OLEDDashboard.h>
#include <Wire.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

void setup() {
    dashboard.begin();
    dashboard.setWiFi(4);
    dashboard.setBattery(92);
}

void loop() {
    dashboard.setTemperature(23.6);        // °C
    dashboard.setHumidity(45.2);           // %
    dashboard.addTemperatureHistory(23.6); // feeds the graph + stats
    dashboard.update();                    // redraws changed widgets, display()
    delay(1000);                           // 1 Hz sensor update
}
```

That is the whole API for the common case.

## API reference

### Lifecycle

| Method | Description |
| --- | --- |
| `OLEDDashboard(Adafruit_SSD1306&)` | Bind a display instance. |
| `begin(uint8_t addr = 0x3C)` | Initialise the display and draw the first frame. |
| `update()` | Update and redraw only dirty widgets, then call `display()` once. |
| `draw()` | Force a full redraw of every widget. |

### Data (simple API)

| Method | Description |
| --- | --- |
| `setTemperature(float)` | Set temperature in °C (e.g. `23.6`). |
| `setHumidity(float)` | Set relative humidity in % (e.g. `45.2`). |
| `setBattery(uint8_t)` | Battery level, 0..100 %. |
| `setBatteryCharging(bool)` | Draw the charging bolt. |
| `setWiFi(uint8_t)` | Signal strength, 0..4 bars. |
| `addTemperatureHistory(float)` | Append a history sample (graph + footer stats). |

### Configuration

| Method | Description |
| --- | --- |
| `setTheme(ThemeId)` | Apply a built-in theme. |
| `setTheme(const Theme&)` | Apply a custom theme. |
| `setGraphStyle(GraphStyle)` | `kFade` (default), `kLine`, `kBars`, `kFilled`, `kDots`. |
| `setGraphHistorySize(uint16_t)` | Retained history length, 1..128 (default 128). |

### Fluent API

Every widget is reachable for finer control:

```cpp
dashboard.temperature().value(23.6);
dashboard.humidity().value(45.2);
dashboard.wifi().strength(4);
dashboard.battery().percent(92);
dashboard.battery().charging(true);
dashboard.graph().addSample(-34);        // raw ×10 values
dashboard.graph().setStyle(OledDashboard::GraphStyle::kLine);
```

## Data storage and statistics

History is stored as **`int16_t` values scaled by ten** (23.6 → 236). The
`Statistics` class keeps min / max / sum incrementally, so the footer's
`Min:.. Max:.. Avg:..` is always O(1). The default ring buffer holds 128
samples (256 bytes RAM) — one per graph column at 128 px width.

## Themes

Themes control fonts, dividers, graph style, icons and spacing:

```cpp
dashboard.setTheme(OledDashboard::ThemeId::kMinimal);  // line graph, no dividers
```

| Theme | Dividers | Graph | Notes |
| --- | --- | --- | --- |
| `kProfessional` | Yes | Fade | Default. |
| `kMinimal` | No | Line | Clean, flat. |
| `kIndustrial` | Yes | Bars | Bright, bordered graph. |
| `kDark` | Yes | Filled | Dense area fill. |
| `kModern` | No | Dots | No panel borders. |

Custom themes are plain `Theme` structs — see `DashboardTheme.h`.

## Widgets

Each widget is a class derived from `OledDashboard::Widget` with `update()`,
`draw()`, bounds and visibility. Widgets redraw only when dirty, so a
temperature change never repaints the graph. Widget regions are
non-overlapping by design (see `Layout.h`).

## Fonts

- **Small** 5x7 — labels, footer units.
- **Large bold** 8x16 — the big sensor numbers (digits + `. - % : + °C`).
- **Compact** 4x6 — the footer statistics row.

The degree symbol is `0xB0`; the library emits it for you via the unit-aware
widgets, so no Unicode handling is needed in sketches.

## Memory & performance

| Resource | Typical |
| --- | --- |
| History buffer (128 × `int16_t`) | 256 B |
| Widgets + engine state | ≈ 150 B |
| Adafruit SSD1306 framebuffer | 1024 B (external) |
| Flash | a few KB over the Adafruit dependencies |

No `new`/`malloc`/`std::vector` anywhere. Rendering skips unchanged widgets and
`display()` is called once per `update()`.

## Customisation

- **Icons**: bitmap arrays live in `src/Icons.h` (generated by
  `tools/gen_icons.py`).
- **Fonts**: bitmap tables in `src/Fonts.h` (generated by `tools/gen_fonts.py`).
- **Layout**: compile-time constants in `src/Layout.h`.
- **New widgets**: derive from `Widget`, implement `update()`/`draw()`, then
  add the instance to `OLEDDashboard`.

## Board compatibility

Verified targets: Arduino Uno, Nano, Pro Mini, Mega, ESP32, ESP8266.
Negative temperatures, disconnected WiFi, battery edge cases and rapid updates
are covered by the included tests (`test/`).

## Contributing

See the project's implementation plan in the repository root. Milestones are:
scaffolding → layout → sensor widgets → icons → graph engine → themes/fonts →
docs/examples/optimisation. Keep the style: modern C++ (C++11), `const`
correctness, no magic numbers, Doxygen comments, no dynamic allocation.

## License

MIT — see `LICENSE`.
