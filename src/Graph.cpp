#include "Graph.h"

namespace OledDashboard {

Graph::Graph(uint16_t historySize) : Widget(0, Layout::kGraphY, Layout::kScreenW,
                                            Layout::kGraphH),
                                     stats_(historySize),
                                     style_(GraphStyle::kFade),
                                     minScaled_(0),
                                     maxScaled_(1) {}

void Graph::addSample(int16_t valueX10) {
    stats_.add(valueX10);
    setDirty();
}

void Graph::clear() {
    stats_.clear();
    setDirty();
}

void Graph::setBounds(int16_t x, int16_t y, int16_t w, int16_t h) {
    Widget::setBounds(x, y, w, h);
}

void Graph::update() {}

int16_t Graph::valueToY(int32_t value) const {
    const int32_t range = (int32_t)maxScaled_ - minScaled_;
    if (range <= 0) return y_ + h_ - 1;
    const int32_t t = ((int32_t)maxScaled_ - value) * (h_ - 1);
    return y_ + (int16_t)(t / range);
}

void Graph::draw(Adafruit_GFX& gfx) {
    gfx.fillRect(x_, y_, w_, h_, 0);

    const uint16_t n = stats_.count();
    if (n == 0) return;

    // Auto scale: data range plus a 12.5% margin, minimum 1 unit.
    const int16_t mn = stats_.minimum();
    const int16_t mx = stats_.maximum();
    const int16_t range = (int16_t)((int32_t)mx - mn);
    const int16_t margin = (int16_t)(range / 8) + 1;
    minScaled_ = (int16_t)((int32_t)mn - margin);
    maxScaled_ = (int16_t)((int32_t)mx + margin);
    if (maxScaled_ <= minScaled_) maxScaled_ = (int16_t)(minScaled_ + 2);

    // Right aligned: the newest sample always sits at the right edge.
    const uint16_t visible = (n < (uint16_t)w_) ? n : (uint16_t)w_;
    const int16_t xStart = (int16_t)(x_ + w_ - visible);

    switch (style_) {
        case GraphStyle::kFade:
            drawFade(gfx);
            break;
        case GraphStyle::kLine:
            drawLine(gfx);
            break;
        case GraphStyle::kBars:
            drawBars(gfx);
            break;
        case GraphStyle::kFilled:
            drawBars(gfx);
            break;
        case GraphStyle::kDots:
            drawDots(gfx);
            break;
    }

    // Fade and filled styles get a crisp one-pixel polyline on top of the
    // data, mirroring the reference "line graph + fading columns" look.
    if (style_ == GraphStyle::kFade || style_ == GraphStyle::kFilled) {
        int16_t prevY = 0;
        for (int16_t cx = xStart; cx < x_ + w_; ++cx) {
            const int16_t v = stats_.at((uint16_t)(cx - xStart));
            const int16_t cy = valueToY(v);
            if (cx > xStart) gfx.drawLine(cx - 1, prevY, cx, cy, 1);
            prevY = cy;
        }
    }
}

void Graph::drawLine(Adafruit_GFX& gfx) {
    const uint16_t n = stats_.count();
    const uint16_t visible = (n < (uint16_t)w_) ? n : (uint16_t)w_;
    const int16_t xStart = (int16_t)(x_ + w_ - visible);

    int16_t prevY = 0;
    bool havePrev = false;
    for (int16_t cx = xStart; cx < x_ + w_; ++cx) {
        const int16_t v = stats_.at((uint16_t)(cx - xStart));
        const int16_t cy = valueToY(v);
        if (havePrev) gfx.drawLine(cx - 1, prevY, cx, cy, 1);
        prevY = cy;
        havePrev = true;
    }
}

void Graph::drawFade(Adafruit_GFX& gfx) {
    const uint16_t n = stats_.count();
    const uint16_t visible = (n < (uint16_t)w_) ? n : (uint16_t)w_;
    const int16_t xStart = (int16_t)(x_ + w_ - visible);
    const uint8_t* dither = ditherMatrix();
    const int16_t bottom = y_ + h_ - 1;

    for (int16_t cx = xStart; cx < x_ + w_; ++cx) {
        const int16_t v = stats_.at((uint16_t)(cx - xStart));
        const int16_t top = valueToY(v);
        const int16_t colH = bottom - top;  // >= 0
        if (colH < 1) {
            gfx.drawPixel(cx, top, 1);
            continue;
        }
        for (int16_t yy = top; yy <= bottom; ++yy) {
            const int16_t depth = yy - top;
            // Solid near the data point, fading toward the baseline.
            const uint8_t intensity = (uint8_t)(15u - (uint8_t)((13u * (uint16_t)depth) / (uint16_t)colH));
            const uint8_t thresh = pgm_read_byte(&dither[((yy - y_) & 3) * 4 + ((cx - x_) & 3)]);
            if (thresh < intensity) gfx.drawPixel(cx, yy, 1);
        }
    }
}

void Graph::drawBars(Adafruit_GFX& gfx) {
    const uint16_t n = stats_.count();
    const uint16_t visible = (n < (uint16_t)w_) ? n : (uint16_t)w_;
    const int16_t xStart = (int16_t)(x_ + w_ - visible);
    const int16_t bottom = y_ + h_ - 1;

    for (int16_t cx = xStart; cx < x_ + w_; ++cx) {
        const int16_t v = stats_.at((uint16_t)(cx - xStart));
        const int16_t top = valueToY(v);
        gfx.drawFastVLine(cx, top, (uint16_t)(bottom - top + 1), 1);
    }
}

void Graph::drawDots(Adafruit_GFX& gfx) {
    const uint16_t n = stats_.count();
    const uint16_t visible = (n < (uint16_t)w_) ? n : (uint16_t)w_;
    const int16_t xStart = (int16_t)(x_ + w_ - visible);

    for (int16_t cx = xStart; cx < x_ + w_; ++cx) {
        const int16_t v = stats_.at((uint16_t)(cx - xStart));
        const int16_t cy = valueToY(v);
        gfx.drawPixel(cx, cy, 1);
        gfx.drawPixel(cx + 1, cy, 1);
        gfx.drawPixel(cx, cy + 1, 1);
    }
}

}  // namespace OledDashboard
