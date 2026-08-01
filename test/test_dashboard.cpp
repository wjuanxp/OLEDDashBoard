// Host-side functional test for OLEDDashboard.
#include <OLEDDashboard.h>

#include <math.h>
#include <stdio.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

int main() {
    dashboard.begin();
    dashboard.setTemperature(23.6f);
    dashboard.setHumidity(45.2f);
    dashboard.setBattery(92);
    dashboard.setWiFi(4);

    // Feed a full history of a smooth wave.
    for (int i = 0; i < 128; ++i) {
        float v = 20.0f + 3.0f * (float)sin(i * 0.15f);
        dashboard.addTemperatureHistory(v);
    }
    dashboard.update();
    printf("================ default (fade) frame ================\n");
    display.printFrame();

    // Change a value and verify only the affected region updates.
    dashboard.setTemperature(-3.4f);
    dashboard.update();
    printf("================ after setTemperature(-3.4) ==========\n");
    display.printFrame();

    // Graph styles.
    dashboard.setGraphStyle(OledDashboard::GraphStyle::kLine);
    dashboard.update();
    printf("================ line style =================\n");
    display.printFrame();

    dashboard.setGraphStyle(OledDashboard::GraphStyle::kBars);
    dashboard.update();
    printf("================ bars style =================\n");
    display.printFrame();

    dashboard.setGraphStyle(OledDashboard::GraphStyle::kDots);
    dashboard.update();
    printf("================ dots style =================\n");
    display.printFrame();

    // Themes.
    dashboard.setTheme(OledDashboard::ThemeId::kMinimal);
    dashboard.update();
    printf("================ minimal theme ==============\n");
    display.printFrame();

    dashboard.setTheme(OledDashboard::ThemeId::kIndustrial);
    dashboard.update();
    printf("================ industrial theme ===========\n");
    display.printFrame();

    // Edge cases.
    dashboard.setBattery(100);
    dashboard.setWiFi(0);
    dashboard.setBatteryCharging(true);
    dashboard.update();
    printf("================ battery 100 / charging / wifi 0 ======\n");
    display.printFrame();

    dashboard.setBatteryCharging(false);
    dashboard.setBattery(15);
    dashboard.update();
    printf("================ battery low warning ========\n");
    display.printFrame();

    return 0;
}
