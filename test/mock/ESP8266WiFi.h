// Mock ESP8266WiFi.h for host compile verification.
#ifndef MOCK_ESP8266WIFI_H
#define MOCK_ESP8266WIFI_H

#define WL_CONNECTED 3

class MockWiFiClass {
public:
    void begin(const char*, const char*) {}
    int status() { return WL_CONNECTED; }
};

extern MockWiFiClass WiFi;

#endif  // MOCK_ESP8266WIFI_H
