#pragma once

// Pins
#define LED_PIN 2
#define DHTT_PIN 4
#define DHTT_TYPE DHT22

// Intervals
#define DHT_INTERVAL 10000
#define PUBLISH_INTERVAL 30000
#define RECONNECT_INTERVAL 5000

// WiFi
#define WIFI_TIMEOUT 10000

// Time
#define TIME_SYNC_TIMEOUT 15000

// MQTT
#define MQTT_PORT 8883
#define TOPIC_SENSORS "iot-course/vlasenko/data"
