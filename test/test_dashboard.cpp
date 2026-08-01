// Host-side functional test for OLEDDashboard.
#include <OLEDDashboard.h>

#include <math.h>
#include <stdio.h>

using OledDashboard::celsiusToFahrenheitX10;

static int failures = 0;
#define CHECK(cond, msg)                                     \
    do {                                                     \
        if (!(cond)) {                                       \
            printf("FAIL: %s\n", msg);                       \
            ++failures;                                      \
        }                                                    \
    } while (0)

Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

int main() {
    // Fahrenheit conversion: known points.
    CHECK(celsiusToFahrenheitX10(0) == 320, "0 C -> 32.0 F");
    CHECK(celsiusToFahrenheitX10(236) == 745, "23.6 C -> 74.5 F");
    CHECK(celsiusToFahrenheitX10(-34) == 259, "-3.4 C -> 25.9 F");
    CHECK(celsiusToFahrenheitX10(-400) == -400, "-40 C -> -40 F");
    CHECK(celsiusToFahrenheitX10(100) == 500, "10 C -> 50 F");
    // Rounding: 37.2 C == 98.96 F -> 99.0.
    CHECK(celsiusToFahrenheitX10(372) == 990, "37.2 C -> 99.0 F");

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

    // Fahrenheit temperature unit.
    dashboard.setTemperatureUnit(OledDashboard::kUnitDegF);
    dashboard.update();
    printf("================ temperature in Fahrenheit ==========\n");
    display.printFrame();

    dashboard.setTemperatureUnit(OledDashboard::kUnitDegC);
    dashboard.update();
    printf("================ temperature back to Celsius =======\n");
    display.printFrame();

    if (failures == 0) {
        printf("ALL DASHBOARD TESTS PASSED\n");
        return 0;
    }
    printf("%d FAILURES\n", failures);
    return 1;
}
