#pragma once

// Pins
#define LED_PIN 2
#define BUTTON_PIN 5
#define DHTT_PIN 4
#define DHTT_TYPE DHT22

// Intervals
#define DHT_INTERVAL 10000
#define PUBLISH_INTERVAL 10000
#define RECONNECT_INTERVAL 5000

// Debounce
#define DEBOUNCE 50

// WiFi
#define WIFI_TIMEOUT 10000

// Time
#define TIME_SYNC_TIMEOUT 15000

// MQTT
#define MQTT_PORT 8883
#define TOPIC_SENSORS "iot-course/vlasenko/sensors"
#define TOPIC_COMANDS "iot-course/vlasenko/comands"