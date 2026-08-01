/**
 * IoT_MQTT_Dashboard
 *
 * ESP8266/ESP32 dashboard driven by MQTT topics (e.g. a home-automation
 * server). Subscribes to temperature, humidity, WiFi RSSI and battery topics.
 *
 * Install via Library Manager:
 *   - PubSubClient by Nick O'Leary
 *   - Adafruit SSD1306 (SPI or I2C)
 *
 * MQTT topics (change to match your broker):
 *   sensor/temp   -> float degrees C
 *   sensor/hum    -> float percent
 *   sys/rssi      -> WiFi RSSI in dBm (converted to 0..4 bars)
 *   sys/battery   -> 0..100 percent
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OLEDDashboard.h>
#include <PubSubClient.h>
#include <Wire.h>

#include <ESP8266WiFi.h>  // or <WiFi.h> on ESP32

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASSWORD";
const char* MQTT_HOST = "192.168.1.10";
const uint16_t MQTT_PORT = 1883;

Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledDashboard::OLEDDashboard dashboard(display);

WiFiClient espClient;
PubSubClient mqtt(espClient);

static float gTemp = 0.0f;
static float gHum = 0.0f;
static uint8_t gBattery = 0;
static uint8_t gWifiBars = 0;

static void publishCallback(char* topic, byte* payload, unsigned int length) {
    char buf[16];
    if (length > sizeof(buf) - 1) length = sizeof(buf) - 1;
    memcpy(buf, payload, length);
    buf[length] = '\0';

    if (strcmp(topic, "sensor/temp") == 0) {
        gTemp = atof(buf);
    } else if (strcmp(topic, "sensor/hum") == 0) {
        gHum = atof(buf);
    } else if (strcmp(topic, "sys/battery") == 0) {
        gBattery = (uint8_t)atoi(buf);
    } else if (strcmp(topic, "sys/rssi") == 0) {
        const int rssi = atoi(buf);
        if (rssi > -55) gWifiBars = 4;
        else if (rssi > -65) gWifiBars = 3;
        else if (rssi > -75) gWifiBars = 2;
        else if (rssi > -85) gWifiBars = 1;
        else gWifiBars = 0;
    }
}

void setup() {
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" WiFi connected");

    mqtt.setServer(MQTT_HOST, MQTT_PORT);
    mqtt.setCallback(publishCallback);

    dashboard.begin();
}

void reconnect() {
    while (!mqtt.connected()) {
        if (mqtt.connect("oled-dashboard")) {
            mqtt.subscribe("sensor/temp");
            mqtt.subscribe("sensor/hum");
            mqtt.subscribe("sys/rssi");
            mqtt.subscribe("sys/battery");
        } else {
            delay(1000);
        }
    }
}

void loop() {
    if (!mqtt.connected()) reconnect();
    mqtt.loop();

    dashboard.setTemperature(gTemp);
    dashboard.setHumidity(gHum);
    dashboard.setBattery(gBattery);
    dashboard.setWiFi(gWifiBars);
    dashboard.addTemperatureHistory(gTemp);
    dashboard.update();

    delay(1000);
}
