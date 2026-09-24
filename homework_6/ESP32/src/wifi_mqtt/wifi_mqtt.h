#pragma once

#include <Arduino.h>
#include <PubSubClient.h>

extern PubSubClient mqttClient;

bool connectWiFi();
bool connectMQTT();
bool syncTime();

void connectAWS();
void publishData(float temperature, float humidity);
void onMessage(char* topic, byte* payload, unsigned int length);
void handleCommand();
