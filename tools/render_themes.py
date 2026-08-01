#!/usr/bin/env python3
"""
render_themes.py -- generate screen renders of all built-in OLEDDashboard themes.

Renders the five shipped themes (Professional, Minimal, Industrial, Dark,
Modern) exactly as the library would draw them on a 128x64 SSD1306 panel.
Font and icon data is parsed from src/Fonts.h and src/Icons.h so the output
always matches the firmware. PNGs are written to docs/themes/.

Usage:
    python tools/render_themes.py
"""

import math
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FONTS_PATH = os.path.join(REPO_ROOT, "src", "Fonts.h")
ICONS_PATH = os.path.join(REPO_ROOT, "src", "Icons.h")
OUT_DIR = os.path.join(REPO_ROOT, "docs", "themes")

SCALE = 4  # upscale factor for readable PNG output
SCREEN_W, SCREEN_H = 128, 64

# ---------------------------------------------------------------------------
# C header parsing
# ---------------------------------------------------------------------------


def _balanced_body(text, start):
    """Return the balanced-brace body starting at the first '{' after start."""
    i = text.find("{", start)
    if i < 0:
        raise ValueError("no '{' found")
    depth = 0
    j = i
    while j < len(text):
        c = text[j]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return text[i:j]
        j += 1
    raise ValueError("unbalanced braces")


def parse_int_array(text, name):
    """Extract the flat int array for `static const ... name[...] PROGMEM`."""
    m = re.search(
        r"static\s+const\s+\w+\s+%s\s*(?:\[\s*[^\]]*\])+\s+PROGMEM\s*=" % name,
        text,
    )
    if not m:
        raise ValueError("array %s not found" % name)
    body = _balanced_body(text, m.end())
    body = re.sub(r"/\*.*?\*/", "", body, flags=re.S)
    body = re.sub(r"//.*", "", body)
    return [int(v, 16) for v in re.findall(r"0x[0-9A-Fa-f]+", body)]


def parse_glyph_table(text, name):
    """Return [(code, data-array-name)] in table order."""
    m = re.search(
        r"static\s+const\s+\w+\s+%s\s*\[\s*[^\]]*\]\s+PROGMEM\s*=" % name, text
    )
    if not m:
        raise ValueError("table %s not found" % name)
    body = _balanced_body(text, m.end())
    entries = []
    for quote, dec, hexv, arr in re.findall(
        r"\{\s*(?:'([^']*)'|(\d+)|(0x[0-9A-Fa-f]+))\s*,\s*(s\w+)\s*\}", body
    ):
        if quote != "":
            code = ord(quote)
        elif dec:
            code = int(dec)
        else:
            code = int(hexv, 16)
        entries.append((code, arr))
    return entries


def load_assets():
    fonts = open(FONTS_PATH, encoding="utf-8").read()
    icons = open(ICONS_PATH, encoding="utf-8").read()

    small_raw = parse_int_array(fonts, r"sSmallFont")
    small = [small_raw[i * 5 : (i + 1) * 5] for i in range(95)]

    large_by_name = {}
    for i in range(18):
        name = "sLarge%02d" % i
        data = parse_int_array(fonts, name)
        large_by_name[name] = data

    large_table = parse_glyph_table(fonts, r"sLargeGlyphs")
    large = [large_by_name[name] for _, name in large_table]

    compact_by_name = {}
    for i in range(24):
        name = "sCompact%02d" % i
        data = parse_int_array(fonts, name)
        compact_by_name[name] = data

    compact_table = parse_glyph_table(fonts, r"sCompactGlyphs")
    compact = {code: compact_by_name[name] for code, name in compact_table}

    wifi_raw = parse_int_array(icons, r"sWifiIcons")
    wifi = [wifi_raw[i * 11 : (i + 1) * 11] for i in range(5)]
    outline = parse_int_array(icons, r"sBatteryOutline")
    bolt = parse_int_array(icons, r"sBatteryBolt")
    warn = parse_int_array(icons, r"sBatteryWarn")

    return small, large, compact, wifi, outline, bolt, warn


# ---------------------------------------------------------------------------
# 128x64 pixel canvas
# ---------------------------------------------------------------------------


class Canvas:
    def __init__(self, w=SCREEN_W, h=SCREEN_H):
        self.w = w
        self.h = h
        self.px = [[0] * w for _ in range(h)]

    def drawPixel(self, x, y, c=1):
        if 0 <= x < self.w and 0 <= y < self.h:
            self.px[y][x] = c

    def fillRect(self, x, y, w, h, c=1):
        for yy in range(y, min(y + h, self.h)):
            if yy < 0:
                continue
            for xx in range(x, min(x + w, self.w)):
                if xx >= 0:
                    self.px[yy][xx] = c

    def hline(self, x, y, w, c=1):
        if 0 <= y < self.h:
            for xx in range(max(x, 0), min(x + w, self.w)):
                self.px[y][xx] = c

    def vline(self, x, y, h, c=1):
        if 0 <= x < self.w:
            for yy in range(max(y, 0), min(y + h, self.h)):
                self.px[yy][x] = c

    def line(self, x0, y0, x1, y1, c=1):
        dx = abs(x1 - x0)
        sx = 1 if x0 < x1 else -1
        dy = -abs(y1 - y0)
        sy = 1 if y0 < y1 else -1
        err = dx + dy
        while True:
            self.drawPixel(x0, y0, c)
            if x0 == x1 and y0 == y1:
                break
            e2 = 2 * err
            if e2 >= dy:
                err += dy
                x0 += sx
            if e2 <= dx:
                err += dx
                y0 += sy


# ---------------------------------------------------------------------------
# Text renderer
# ---------------------------------------------------------------------------

DEGREE = chr(0xB0)


def text_width_small(s):
    return 6 * len(s)


def text_width_large(s):
    return sum(9 if large_glyph_index(ch) >= 0 else 6 for ch in s)


def text_width_compact(s):
    return sum(4 if ch in COMPACT else 6 for ch in s)


def draw_text_small(canvas, x, y, s, inverted=False):
    for ch in s:
        code = ord(ch)
        if not (0x20 <= code <= 0x7E):
            code = 0x3F  # '?'
        glyph = SMALL[code - 0x20]
        for col in range(5):
            line = glyph[col]
            for row in range(7):
                on = (line >> row) & 1
                if on != inverted:
                    canvas.drawPixel(x + col, y + row, 0 if inverted else 1)
        x += 6


def large_glyph_index(c):
    if "0" <= c <= "9":
        return ord(c) - 0x30
    return {'.': 10, '-': 11, '%': 12, ':': 13, 'C': 14,
            DEGREE: 15, '+': 16}.get(c, -1)


def draw_text_large(canvas, x, y, s, inverted=False):
    for ch in s:
        idx = large_glyph_index(ch)
        if idx < 0:
            draw_text_small(canvas, x, y + 4, ch, inverted)
            x += 6
            continue
        glyph = LARGE[idx]
        for row in range(16):
            bits = glyph[row]
            for col in range(8):
                on = (bits >> (7 - col)) & 1
                if on != inverted:
                    canvas.drawPixel(x + col, y + row, 0 if inverted else 1)
        x += 9


def draw_text_compact(canvas, x, y, s):
    for ch in s:
        code = ord(ch)
        if code not in COMPACT:
            draw_text_small(canvas, x, y + 1, ch, False)
            x += 6
            continue
        glyph = COMPACT[code]
        for col in range(4):
            bits = glyph[col]
            for row in range(6):
                if (bits >> row) & 1:
                    canvas.drawPixel(x + col, y + row, 1)
        x += 4


def format_x10(value):
    neg = value < 0
    if neg:
        value = -value
    whole = value // 10
    frac = value % 10
    out = []
    if neg:
        out.append("-")
    if whole >= 100:
        out.append(chr(0x30 + whole // 100))
    if whole >= 10:
        out.append(chr(0x30 + (whole // 10) % 10))
    out.append(chr(0x30 + whole % 10))
    out.append(".")
    out.append(chr(0x30 + frac))
    return "".join(out)


# ---------------------------------------------------------------------------
# Widgets
# ---------------------------------------------------------------------------

# Fixed layout (src/Layout.h)
LEFT_PANEL_W = 104
DIVIDER_X = 104
TEMP_ROW_Y, TEMP_ROW_H = 0, 16
DIVIDER1_Y = 16
HUMIDITY_ROW_Y, HUMIDITY_ROW_H = 17, 16
DIVIDER2_Y = 33
GRAPH_Y, GRAPH_H = 34, 20
FOOTER_Y = 56


def draw_sensor(canvas, y, label, value_x10, unit):
    canvas.fillRect(0, y, LEFT_PANEL_W, 16, 0)
    text = format_x10(value_x10)
    if unit == "C":
        text += DEGREE + "C"
    elif unit == "%":
        text += "%"
    draw_text_small(canvas, 2, y + 1, label)
    tw = text_width_large(text)
    draw_text_large(canvas, LEFT_PANEL_W - 3 - tw, y, text)


def draw_dividers(canvas):
    canvas.hline(0, DIVIDER1_Y, DIVIDER_X, 1)
    canvas.hline(0, DIVIDER2_Y, DIVIDER_X, 1)
    canvas.vline(DIVIDER_X, 0, DIVIDER2_Y + 1, 1)


def draw_wifi(canvas, level):
    x, y, w, h = DIVIDER_X + 1, 1, 23, 15
    canvas.fillRect(x, y, w, h, 0)
    if level == 0:
        return
    icon_x = x + (w - 13) // 2
    icon_y = y + 2
    for row, bits in enumerate(WIFI[level]):
        for col in range(13):
            if (bits >> (12 - col)) & 1:
                canvas.drawPixel(icon_x + col, icon_y + row, 1)


def draw_battery(canvas, percent, charging=False, low_warning=True):
    x, y, w, h = DIVIDER_X + 1, HUMIDITY_ROW_Y, 23, 16
    canvas.fillRect(x, y, w, h, 0)
    icon_x = x + (w - 14) // 2
    icon_y = y + 1
    low = low_warning and not charging and percent <= 20

    fill_w = percent * 10 // 100
    if fill_w > 0:
        canvas.fillRect(icon_x + 1, icon_y + 1, fill_w, 5, 1)

    if charging:
        bx, by = icon_x + 3, icon_y + 1
        canvas.fillRect(bx, by, 6, 6, 0)
        for row, bits in enumerate(BOLT):
            for col in range(6):
                if (bits >> (5 - col)) & 1:
                    canvas.drawPixel(bx + col, by + row, 1)
    elif low:
        wx, wy = icon_x + 5, icon_y + 1
        for row, bits in enumerate(WARN):
            for col in range(3):
                if (bits >> (2 - col)) & 1:
                    canvas.drawPixel(wx + col, wy + row, 1)

    for row, bits in enumerate(OUTLINE):
        for col in range(14):
            if (bits >> (13 - col)) & 1:
                canvas.drawPixel(icon_x + col, icon_y + row, 1)

    if percent == 100:
        pct = "100"
    else:
        pct = chr(0x30 + percent // 10) + chr(0x30 + percent % 10)
    tw = text_width_small(pct)
    draw_text_small(canvas, x + (w - tw) // 2, icon_y + 7 + 2, pct)


def draw_footer(canvas, min_x10, max_x10, avg_x10):
    canvas.fillRect(0, FOOTER_Y, SCREEN_W, 8, 0)
    if avg_x10 < 0:
        avg = (avg_x10 - 5) // 10
    else:
        avg = (avg_x10 + 5) // 10
    line = "Min:%s  Max:%s  Avg:%s" % (
        format_x10(min_x10), format_x10(max_x10), format_x10(avg))
    tw = text_width_compact(line)
    draw_text_compact(canvas, (SCREEN_W - tw) // 2, FOOTER_Y + 1, line)


# ---------------------------------------------------------------------------
# Graph
# ---------------------------------------------------------------------------

DITHER = (
    (0, 8, 2, 10),
    (12, 4, 14, 6),
    (3, 11, 1, 9),
    (15, 7, 13, 5),
)


def value_to_y(value, min_scaled, max_scaled, y, h):
    rng = max_scaled - min_scaled
    if rng <= 0:
        return y + h - 1
    t = (max_scaled - value) * (h - 1)
    return y + t // rng


def draw_graph(canvas, samples, style):
    x, y, w, h = 0, GRAPH_Y, SCREEN_W, GRAPH_H
    canvas.fillRect(x, y, w, h, 0)
    n = len(samples)
    if n == 0:
        return

    mn, mx = min(samples), max(samples)
    rng = mx - mn
    margin = rng // 8 + 1
    min_scaled = mn - margin
    max_scaled = mx + margin
    if max_scaled <= min_scaled:
        max_scaled = min_scaled + 2

    visible = min(n, w)
    x_start = x + w - visible
    bottom = y + h - 1

    def sample(cx):
        return samples[cx - x_start]

    if style == "fade":
        for cx in range(x_start, x + w):
            top = value_to_y(sample(cx), min_scaled, max_scaled, y, h)
            col_h = bottom - top
            if col_h < 1:
                canvas.drawPixel(cx, top, 1)
                continue
            for yy in range(top, bottom + 1):
                depth = yy - top
                intensity = 15 - (13 * depth) // col_h
                thresh = DITHER[(yy - y) & 3][(cx - x) & 3]
                if thresh < intensity:
                    canvas.drawPixel(cx, yy, 1)
    elif style == "bars" or style == "filled":
        for cx in range(x_start, x + w):
            top = value_to_y(sample(cx), min_scaled, max_scaled, y, h)
            canvas.vline(cx, top, bottom - top + 1, 1)
    elif style == "dots":
        for cx in range(x_start, x + w):
            cy = value_to_y(sample(cx), min_scaled, max_scaled, y, h)
            canvas.drawPixel(cx, cy, 1)
            canvas.drawPixel(cx + 1, cy, 1)
            canvas.drawPixel(cx, cy + 1, 1)

    # Line graph, or the crisp polyline on top of fade / filled.
    if style in ("line", "fade", "filled"):
        prev = None
        for cx in range(x_start, x + w):
            cy = value_to_y(sample(cx), min_scaled, max_scaled, y, h)
            if prev is not None:
                canvas.line(cx - 1, prev, cx, cy, 1)
            prev = cy


# ---------------------------------------------------------------------------
# Theme assembly
# ---------------------------------------------------------------------------

THEMES = (
    ("professional", "fade", True),
    ("minimal", "line", False),
    ("industrial", "bars", True),
    ("dark", "filled", True),
    ("modern", "dots", False),
)


def render_dashboard(samples, style, show_dividers):
    canvas = Canvas()
    draw_sensor(canvas, TEMP_ROW_Y, "Temp", 236, "C")       # 23.6 C
    draw_sensor(canvas, HUMIDITY_ROW_Y, "Humidity", 452, "%")  # 45.2 %
    if show_dividers:
        draw_dividers(canvas)
    draw_wifi(canvas, 4)
    draw_battery(canvas, 92)
    draw_graph(canvas, samples, style)
    draw_footer(canvas, min(samples), max(samples), sum(samples) * 10 // len(samples))
    return canvas


def sample_history():
    """128 samples ending exactly at 23.6 (matching the live Temp value)."""
    out = []
    for i in range(128):
        p = i / 127.0
        base = 21.0 + 2.6 * p
        wiggle = (
            0.6 * (1 - p) * math.sin(2 * math.pi * i / 21.0)
            + 0.3 * (1 - p) * math.sin(2 * math.pi * i / 5.0 + 1.2)
        )
        out.append(round((base + wiggle) * 10))
    out[127] = 236
    return out


def to_png(canvas, path):
    from PIL import Image

    img = Image.new("L", (SCREEN_W * SCALE, SCREEN_H * SCALE), 0)
    px = img.load()
    for y in range(SCREEN_H):
        for x in range(SCREEN_W):
            if canvas.px[y][x]:
                base_x = x * SCALE
                base_y = y * SCALE
                for dy in range(SCALE):
                    for dx in range(SCALE):
                        px[base_x + dx, base_y + dy] = 255
    img.save(path)
    print("wrote %s (%dx%d)" % (path, img.size[0], img.size[1]))


def main():
    global SMALL, LARGE, COMPACT, WIFI, OUTLINE, BOLT, WARN
    SMALL, LARGE, COMPACT, WIFI, OUTLINE, BOLT, WARN = load_assets()
    os.makedirs(OUT_DIR, exist_ok=True)
    samples = sample_history()
    for name, style, show_dividers in THEMES:
        canvas = render_dashboard(samples, style, show_dividers)
        to_png(canvas, os.path.join(OUT_DIR, "%s.png" % name))


if __name__ == "__main__":
    main()
