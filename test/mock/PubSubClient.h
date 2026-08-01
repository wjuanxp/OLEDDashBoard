// Mock PubSubClient.h for host compile verification.
#ifndef MOCK_PUBSUBCLIENT_H
#define MOCK_PUBSUBCLIENT_H

#include <Arduino.h>

class WiFiClient {};

class PubSubClient {
public:
    typedef void (*Callback)(char* topic, byte* payload, unsigned int length);

    explicit PubSubClient(WiFiClient&) {}
    void setServer(const char*, uint16_t) {}
    void setCallback(Callback) {}
    bool connect(const char*) { return true; }
    void subscribe(const char*) {}
    bool connected() { return true; }
    void loop() {}
};

#endif  // MOCK_PUBSUBCLIENT_H
