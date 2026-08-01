#include "Widgets.h"

#include <string.h>

#include "Fonts.h"
#include "Icons.h"

namespace OledDashboard {

// ---------------------------------------------------------------------------
// Bitmap text renderer
// ---------------------------------------------------------------------------

namespace {

/// 4x4 ordered Bayer dither matrix used by the fade graph.
const uint8_t kDither4x4[4][4] PROGMEM = {
    {0, 8, 2, 10},
    {12, 4, 14, 6},
    {3, 11, 1, 9},
    {15, 7, 13, 5},
};

}  // namespace

/// Map a large-font glyph code to its index in sLargeGlyphs (or -1).
static int8_t largeGlyphIndex(uint8_t c) {
    if (c >= '0' && c <= '9') return (int8_t)(c - '0');
    switch (c) {
        case '.': return 10;
        case '-': return 11;
        case '%': return 12;
        case ':': return 13;
        case 'C': return 14;
        case 0xB0: return 15;
        case '+': return 16;
        default: return -1;
    }
}

int16_t textWidthSmall(const char* s) {
    int16_t w = 0;
    while (*s) {
        w += 6;  // 5 px glyph + 1 px spacing
        ++s;
    }
    return w;
}

int16_t textWidthLarge(const char* s) {
    int16_t w = 0;
    while (*s) {
        const uint8_t c = (uint8_t)*s;
        if (largeGlyphIndex(c) >= 0) {
            w += 9;  // 8 px glyph + 1 px spacing
        } else {
            w += 6;  // small fallback
        }
        ++s;
    }
    return w;
}

namespace {

/// Look up a compact (4x6) glyph index by char code, or -1.
int8_t compactGlyphIndex(uint8_t c) {
    for (uint8_t i = 0; i < kCompactGlyphCount; ++i) {
        const FontCompactGlyph* g = &sCompactGlyphs[i];
        if (pgm_read_byte(&g->code) == c) return (int8_t)i;
    }
    return -1;
}

}  // namespace

int16_t textWidthCompact(const char* s) {
    int16_t w = 0;
    while (*s) {
        if (compactGlyphIndex((uint8_t)*s) >= 0) {
            w += 4;  // 4 px glyph, no spacing (keeps the footer on screen)
        } else {
            w += 6;
        }
        ++s;
    }
    return w;
}

int16_t drawTextSmall(Adafruit_GFX& gfx, int16_t x, int16_t y,
                      const char* s, bool inverted) {
    int16_t ox = x;
    while (*s) {
        uint8_t c = (uint8_t)*s++;
        if (c < kSmallFirst || c > kSmallLast) c = '?';
        const uint8_t* glyph = &sSmallFont[c - kSmallFirst][0];
        for (int8_t col = 0; col < kSmallGlyphW; ++col) {
            uint8_t line = pgm_read_byte(glyph + col);
            for (int8_t row = 0; row < kSmallGlyphH; ++row) {
                const bool on = (line >> row) & 1;
                if (on != inverted) {
                    gfx.drawPixel(x + col, y + row, inverted ? 0 : 1);
                }
            }
        }
        x += 6;
    }
    return x - ox;
}

int16_t drawTextLarge(Adafruit_GFX& gfx, int16_t x, int16_t y,
                      const char* s, bool inverted) {
    int16_t ox = x;
    while (*s) {
        const uint8_t c = (uint8_t)*s++;
        const int8_t idx = largeGlyphIndex(c);
        if (idx < 0) {
            // Unknown glyph: fall back to the small font.
            char tmp[2] = {(char)c, 0};
            drawTextSmall(gfx, x, y + 4, tmp, inverted);
            x += 6;
            continue;
        }
        const FontLargeGlyph* glyph = &sLargeGlyphs[idx];
        for (int8_t row = 0; row < 16; ++row) {
            uint8_t bits = pgm_read_byte(glyph->data + row);
            for (int8_t col = 0; col < 8; ++col) {
                const bool on = (bits >> (7 - col)) & 1;
                if (on != inverted) {
                    gfx.drawPixel(x + col, y + row, inverted ? 0 : 1);
                }
            }
        }
        x += 9;
    }
    return x - ox;
}

int16_t drawTextCompact(Adafruit_GFX& gfx, int16_t x, int16_t y,
                        const char* s) {
    int16_t ox = x;
    while (*s) {
        const uint8_t c = (uint8_t)*s++;
        const int8_t idx = compactGlyphIndex(c);
        if (idx < 0) {
            char tmp[2] = {(char)c, 0};
            drawTextSmall(gfx, x, y + 1, tmp, false);
            x += 6;
            continue;
        }
        const FontCompactGlyph* glyph = &sCompactGlyphs[idx];
        for (int8_t col = 0; col < kCompactGlyphW; ++col) {
            const uint8_t bits = pgm_read_byte(glyph->data + col);
            for (int8_t row = 0; row < kCompactGlyphH; ++row) {
                if ((bits >> row) & 1) {
                    gfx.drawPixel(x + col, y + row, 1);
                }
            }
        }
        x += kCompactGlyphW;
    }
    return x - ox;
}

uint8_t formatX10(int16_t valueX10, char* out) {
    bool neg = valueX10 < 0;
    if (neg) valueX10 = -valueX10;
    uint16_t whole = (uint16_t)(valueX10 / 10);
    uint8_t frac = (uint8_t)(valueX10 % 10);

    uint8_t n = 0;
    if (neg) out[n++] = '-';
    if (whole >= 100) out[n++] = '0' + (uint8_t)(whole / 100);
    if (whole >= 10) out[n++] = '0' + (uint8_t)((whole / 10) % 10);
    out[n++] = '0' + (uint8_t)(whole % 10);
    out[n++] = '.';
    out[n++] = '0' + frac;
    out[n] = '\0';
    return n;
}

// ---------------------------------------------------------------------------
// SensorValueWidget
// ---------------------------------------------------------------------------

SensorValueWidget::SensorValueWidget(int16_t x, int16_t y, int16_t w, int16_t h,
                                     const char* label, ValueUnit unit)
    : Widget(x, y, w, h),
      value_(0),
      label_(label),
      unit_(unit),
      font_(FontSize::kLargeBold) {}

void SensorValueWidget::update() {}

void SensorValueWidget::draw(Adafruit_GFX& gfx) {
    // Clear own region first.
    gfx.fillRect(x_, y_, w_, h_, 0);

    // Value string (scaled by ten) with unit suffix.
    char text[12];
    uint8_t n = formatX10(value_, text);
    if (unit_ == kUnitDegC) {
        text[n++] = (char)0xB0;
        text[n++] = 'C';
    } else if (unit_ == kUnitPercent) {
        text[n++] = '%';
    }
    text[n] = '\0';

    // Small label, top-left.
    drawTextSmall(gfx, x_ + Layout::kPadding, y_ + 1, label_);

    // Value, right aligned, in the selected font.
    const bool small = (font_ == FontSize::kSmall);
    const int16_t tw = small ? textWidthSmall(text) : textWidthLarge(text);
    const int16_t vx = x_ + w_ - Layout::kValueRightInset - tw;
    const int16_t vy = small ? y_ + 4 : y_;
    if (small) {
        drawTextSmall(gfx, vx, vy, text);
    } else {
        drawTextLarge(gfx, vx, vy, text);
    }
}

// ---------------------------------------------------------------------------
// DividerWidget
// ---------------------------------------------------------------------------

DividerWidget::DividerWidget()
    : Widget(0, 0, Layout::kScreenW, Layout::kSensorAreaH) {}

void DividerWidget::update() {}

void DividerWidget::draw(Adafruit_GFX& gfx) {
    // Horizontal lines end at the vertical divider; the vertical divider spans
    // the whole sensor area. Lines are 1px, drawn at the shared column.
    gfx.drawFastHLine(0, Layout::kDivider1Y, Layout::kDividerX, 1);
    gfx.drawFastHLine(0, Layout::kDivider2Y, Layout::kDividerX, 1);
    gfx.drawFastVLine(Layout::kDividerX, 0, Layout::kSensorAreaH, 1);
}

// ---------------------------------------------------------------------------
// WifiWidget
// ---------------------------------------------------------------------------

WifiWidget::WifiWidget()
    : Widget(Layout::kDividerX + 1, 1, Layout::kRightPanelW - 1,
             Layout::kTempRowH - 1),
      level_(0) {}

void WifiWidget::update() {}

void WifiWidget::draw(Adafruit_GFX& gfx) {
    gfx.fillRect(x_, y_, w_, h_, 0);
    if (level_ == 0) return;  // no signal: icon left blank
    const uint16_t* icon = &sWifiIcons[(uint16_t)level_ * kWifiIconH];
    // Centre horizontally in the panel.
    const int16_t iconX = x_ + (w_ - kWifiIconW) / 2;
    const int16_t iconY = y_ + 2;
    for (int8_t row = 0; row < kWifiIconH; ++row) {
        uint16_t bits = pgm_read_word(icon + row);
        for (int8_t col = 0; col < kWifiIconW; ++col) {
            if ((bits >> (kWifiIconW - 1 - col)) & 1) {
                gfx.drawPixel(iconX + col, iconY + row, 1);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// BatteryWidget
// ---------------------------------------------------------------------------

BatteryWidget::BatteryWidget()
    : Widget(Layout::kDividerX + 1, Layout::kHumidityRowY,
             Layout::kRightPanelW - 1, Layout::kHumidityRowH),
      percent_(0),
      charging_(false),
      lowWarning_(true) {}

void BatteryWidget::update() {}

void BatteryWidget::draw(Adafruit_GFX& gfx) {
    gfx.fillRect(x_, y_, w_, h_, 0);

    const int16_t iconX = x_ + (w_ - kBatteryW) / 2;
    const int16_t iconY = y_ + 1;

    const bool low = lowWarning_ && !charging_ && percent_ <= 20;

    // Fill level (inside the body, inset one pixel).
    // Body is columns 0..11 of the bitmap; fillable inner width is 10.
    const uint8_t fillW = (uint8_t)((uint16_t)percent_ * 10 / 100);
    if (fillW > 0) {
        gfx.fillRect(iconX + 1, iconY + 1, fillW, kBatteryH - 2, 1);
    }

    if (charging_) {
        // Punch out the bolt area so the bolt stays visible over the fill.
        const int16_t bx = iconX + 3;
        const int16_t by = iconY + 1;
        gfx.fillRect(bx, by, kBoltW, kBoltH, 0);
        for (int8_t row = 0; row < kBoltH; ++row) {
            uint8_t bits = pgm_read_byte(&sBatteryBolt[row]);
            for (int8_t col = 0; col < kBoltW; ++col) {
                if ((bits >> (kBoltW - 1 - col)) & 1) {
                    gfx.drawPixel(bx + col, by + row, 1);
                }
            }
        }
    } else if (low) {
        // Exclamation mark over the (empty) body.
        const int16_t wx = iconX + 5;
        const int16_t wy = iconY + 1;
        for (int8_t row = 0; row < kWarnH; ++row) {
            uint8_t bits = pgm_read_byte(&sBatteryWarn[row]);
            for (int8_t col = 0; col < kWarnW; ++col) {
                if ((bits >> (kWarnW - 1 - col)) & 1) {
                    gfx.drawPixel(wx + col, wy + row, 1);
                }
            }
        }
    }

    // Outline over the fill (and any punch-out).
    for (int8_t row = 0; row < kBatteryH; ++row) {
        uint16_t bits = pgm_read_word(&sBatteryOutline[row]);
        for (int8_t col = 0; col < kBatteryW; ++col) {
            if ((bits >> (kBatteryW - 1 - col)) & 1) {
                gfx.drawPixel(iconX + col, iconY + row, 1);
            }
        }
    }

    // Percent text below the icon, centred.
    char pct[4];
    if (percent_ == 100) {
        pct[0] = '1';
        pct[1] = '0';
        pct[2] = '0';
        pct[3] = '\0';
    } else {
        pct[0] = '0' + (uint8_t)(percent_ / 10);
        pct[1] = '0' + (uint8_t)(percent_ % 10);
        pct[2] = '\0';
    }
    const int16_t tw = textWidthSmall(pct);
    drawTextSmall(gfx, x_ + (w_ - tw) / 2, iconY + kBatteryH + 2, pct);
}

// ---------------------------------------------------------------------------
// FooterWidget
// ---------------------------------------------------------------------------

FooterWidget::FooterWidget()
    : Widget(0, Layout::kFooterY, Layout::kScreenW, 8),
      stats_(nullptr),
      minX10_(0),
      maxX10_(0),
      avgX10_(0) {}

void FooterWidget::update() {
    if (stats_ == nullptr) return;
    const int16_t minV = stats_->minimum();
    const int16_t maxV = stats_->maximum();
    const int32_t avgV = stats_->averageX10();
    if (minV != minX10_ || maxV != maxX10_ || avgV != avgX10_) {
        minX10_ = minV;
        maxX10_ = maxV;
        avgX10_ = avgV;
        setDirty();
    }
}

void FooterWidget::draw(Adafruit_GFX& gfx) {
    gfx.fillRect(x_, y_, w_, h_, 0);

    // Build "Min:22.9  Max:24.1  Avg:23.4" into a single small-font line.
    char line[40];
    uint8_t n = 0;

    const char* label = "Min:";
    while (*label) line[n++] = *label++;
    n += formatX10(minX10_, line + n);

    label = "  Max:";
    while (*label) line[n++] = *label++;
    n += formatX10(maxX10_, line + n);

    label = "  Avg:";
    while (*label) line[n++] = *label++;
    {
        // averageX10() is the average of the x10 samples times ten. Dividing by
        // ten (with rounding) gives the average back in x10 units (e.g. 200 ==
        // 20.0), which formatX10 prints as "20.0".
        const int32_t avgRounded =
            (avgX10_ < 0) ? (avgX10_ - 5) / 10 : (avgX10_ + 5) / 10;
        n += formatX10((int16_t)avgRounded, line + n);
    }
    line[n] = '\0';

    const int16_t tw = textWidthCompact(line);
    drawTextCompact(gfx, x_ + (w_ - tw) / 2, y_ + 1, line);
}

const uint8_t* ditherMatrix() { return &kDither4x4[0][0]; }

int16_t floatToX10(float v) {
    if (v >= 0.0f) return (int16_t)(v * 10.0f + 0.5f);
    return (int16_t)(v * 10.0f - 0.5f);
}

}  // namespace OledDashboard

