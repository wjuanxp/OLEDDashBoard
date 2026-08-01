#include "OLEDDashboard.h"

namespace OledDashboard {

OLEDDashboard::OLEDDashboard(Adafruit_SSD1306& display)
    : display_(display),
      theme_(defaultTheme()),
      temp_(0, 0, Layout::kLeftPanelW, Layout::kTempRowH, "Temp",
            kUnitDegC),
      humidity_(0, Layout::kHumidityRowY, Layout::kLeftPanelW,
                Layout::kHumidityRowH, "Humidity", kUnitPercent),
      divider_(),
      wifi_(),
      battery_(),
      graph_(Layout::kDefaultHistory),
      footer_() {}

void OLEDDashboard::begin(uint8_t i2cAddr) {
    display_.begin(SSD1306_SWITCHCAPVCC, i2cAddr);
    display_.clearDisplay();
    footer_.setStatistics(&graph_.statistics());
    applyTheme();  // marks every widget dirty for the first frame
    update();
}

void OLEDDashboard::setTheme(const Theme& theme) {
    theme_ = theme;
    applyTheme();
    display_.clearDisplay();  // wipe pixels of widgets that are now hidden
}

void OLEDDashboard::applyTheme() {
    divider_.setVisible(theme_.showDividers);
    wifi_.setVisible(theme_.wifiIconEnabled);
    battery_.setVisible(theme_.batteryIconEnabled);
    battery_.setLowWarningEnabled(theme_.batteryLowWarning);
    temp_.setFont(theme_.valueFont);
    humidity_.setFont(theme_.valueFont);
    graph_.setStyle(theme_.graphStyle);

    // Every widget must repaint after a theme change.
    temp_.setDirty();
    humidity_.setDirty();
    divider_.setDirty();
    wifi_.setDirty();
    battery_.setDirty();
    graph_.setDirty();
    footer_.setDirty();
}

void OLEDDashboard::update() {
    Widget* const widgets[] = {
        &temp_, &humidity_, &wifi_, &battery_, &divider_, &graph_, &footer_,
    };
    const uint8_t n = sizeof(widgets) / sizeof(widgets[0]);

    for (uint8_t i = 0; i < n; ++i) {
        Widget* w = widgets[i];
        w->update();
        if (w->visible() && w->dirty()) {
            w->draw(display_);
            w->clearDirty();
        }
    }

    display_.display();
}

void OLEDDashboard::draw() {
    Widget* const widgets[] = {
        &temp_, &humidity_, &wifi_, &battery_, &divider_, &graph_, &footer_,
    };
    const uint8_t n = sizeof(widgets) / sizeof(widgets[0]);
    for (uint8_t i = 0; i < n; ++i) {
        widgets[i]->setDirty();
    }
    display_.clearDisplay();  // wipe any stale pixels before a full redraw
    update();
}

}  // namespace OledDashboard
