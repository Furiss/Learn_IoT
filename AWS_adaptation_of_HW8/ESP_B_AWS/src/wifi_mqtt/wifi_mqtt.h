#pragma once

#include <Arduino.h>
#include <PubSubClient.h>

extern PubSubClient mqttClient;

bool connectWiFi();
bool connectMQTT();
bool syncTime();

void connectAWS();