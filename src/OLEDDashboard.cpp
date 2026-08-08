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
    // All widgets are concrete members, so dispatch is direct (no vtables).
    temp_.update();
    if (temp_.visible() && temp_.dirty()) {
        temp_.draw(display_);
        temp_.clearDirty();
    }

    humidity_.update();
    if (humidity_.visible() && humidity_.dirty()) {
        humidity_.draw(display_);
        humidity_.clearDirty();
    }

    wifi_.update();
    if (wifi_.visible() && wifi_.dirty()) {
        wifi_.draw(display_);
        wifi_.clearDirty();
    }

    battery_.update();
    if (battery_.visible() && battery_.dirty()) {
        battery_.draw(display_);
        battery_.clearDirty();
    }

    divider_.update();
    if (divider_.visible() && divider_.dirty()) {
        divider_.draw(display_);
        divider_.clearDirty();
    }

    graph_.update();
    if (graph_.visible() && graph_.dirty()) {
        graph_.draw(display_);
        graph_.clearDirty();
    }

    footer_.update();
    if (footer_.visible() && footer_.dirty()) {
        footer_.draw(display_);
        footer_.clearDirty();
    }

    display_.display();
}

void OLEDDashboard::draw() {
    temp_.setDirty();
    humidity_.setDirty();
    wifi_.setDirty();
    battery_.setDirty();
    divider_.setDirty();
    graph_.setDirty();
    footer_.setDirty();
    display_.clearDisplay();  // wipe any stale pixels before a full redraw
    update();
}

}  // namespace OledDashboard
