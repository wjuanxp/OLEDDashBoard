# Host verification tests

These tests compile the library and run it **on the host PC** (no Arduino
hardware) against a small software renderer that emulates an SSD1306
framebuffer. They are used to verify rendering, statistics and layout during
development.

Requirements: a C++11 compiler (`g++`) and, optionally, `python3`.

## Run

```bash
g++ -std=gnu++11 -Wall -Wextra -I mock -I ../src \
    test_dashboard.cpp mock/mock.cpp \
    ../src/Statistics.cpp ../src/Widgets.cpp ../src/Graph.cpp \
    ../src/DashboardTheme.cpp ../src/OLEDDashboard.cpp \
    -o test_dashboard

./test_dashboard          # prints ASCII frames of every state/theme
```

```bash
g++ -std=gnu++11 -Wall -Wextra -I mock -I ../src \
    test_stats.cpp mock/mock.cpp ../src/Statistics.cpp -o test_stats

./test_stats              # "ALL STATISTICS TESTS PASSED"
```

The `.ino` examples can be compile-checked the same way using
`sketch_main.cpp` as a linker `main()`:

```bash
g++ -std=gnu++11 -I mock -I ../src \
    ../examples/BasicDashboard/BasicDashboard.ino sketch_main.cpp mock/mock.cpp \
    ../src/Statistics.cpp ../src/Widgets.cpp ../src/Graph.cpp \
    ../src/DashboardTheme.cpp ../src/OLEDDashboard.cpp -o check_example
```

## What the mock covers

- `Arduino.h` — core macros (`PROGMEM`, `pgm_read_*`), `millis`, pin I/O.
- `Wire.h`, `Adafruit_GFX.h`, `Adafruit_SSD1306.h` — a real 128x64 monochrome
  software framebuffer with a `printFrame()` ASCII dump.
- `DHT.h`, `Adafruit_AHTX0.h`, `Adafruit_BME280.h`, `PubSubClient.h`,
  `ESP8266WiFi.h` — minimal stubs so every example sketch compiles.

The mock is **only** for host testing and is never part of the compiled
Arduino library (Arduino builds `src/` only).
