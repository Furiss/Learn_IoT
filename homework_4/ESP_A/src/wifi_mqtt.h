#pragma once
#include "config.h"
#include "sensor.h"
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


bool connectWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);
unsigned long start = millis();
while(WiFi.status() != WL_CONNECTED){
if(millis() - start > WIFI_TIMEOUT){
Serial.println("timeout!");
 return false;
}
delay(500);
Serial.print(".");
}
Serial.println("SUCCESS");
Serial.print("[Wi-Fi] IP: ");
Serial.println(WiFi.localIP());
return true;
}

bool connectMQTT() {
Serial.print("Connecting to "); Serial.print(MQTT_BROKER); Serial.print("...");
    if (mqttClient.connect(MQTT_CLIENT_ID)){
        Serial.println("SUCCESS");
        return true;
    }
    Serial.print("Error: "); Serial.println(mqttClient.state());
    return false;
}

void publishData(float temperature, float humidity){
    if(!mqttClient.connected()){
        Serial.println("No connectiont, skipping");
        return;
    }
    char payload[80];
    snprintf(payload, sizeof(payload),
    "{\"temperature\":%.1f,\"humidity\":%.1f}",
     temperature, humidity);

    Serial.print("Publishing: "); Serial.println(payload);

    bool ok = mqttClient.publish(TOPIC_SENSORS, payload);
    Serial.println(ok ? "SUCCESS" : "Publishing ERROR");
}
