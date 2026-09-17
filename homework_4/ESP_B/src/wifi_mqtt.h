#pragma once
#include "config.h"
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

        mqttClient.subscribe(TOPIC_SENSORS, 1);
        Serial.print("subscribed to: "); Serial.println(TOPIC_SENSORS);
        mqttClient.subscribe(TOPIC_COMMANDS, 1);
         Serial.print("subscribed to: "); Serial.println(TOPIC_COMMANDS);

        return true;
    }
    Serial.print("Error: "); Serial.println(mqttClient.state());
    return false;
}
