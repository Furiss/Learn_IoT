#pragma once
#include "config.h"
#include "sensor.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "secrets.h"

// WiFiClient wifiClient;
// PubSubClient mqttClient(wifiClient);
WiFiClientSecure net;
PubSubClient      mqttClient(net);


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
Serial.print("Connecting to AWS IoT Core...");
    if (mqttClient.connect(THINGNAME)){
        Serial.println("SUCCESS");
        return true;
    }
    Serial.print("Error: "); Serial.println(mqttClient.state());

    net.stop();
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


bool syncTime(){
    Serial.print("synchronize time...");
    configTime(0, 0, "pool.ntp.org");

    struct tm timeinfo;
    unsigned long start = millis();
    while(!getLocalTime(&timeinfo)){
        if(millis()- start > TIME_SYNC_TIMEOUT){
            Serial.println("TIMEOUT");
            return false;
        }
        delay(500);
        Serial.print(".");
    }
    Serial.println("SUCCESS");
    return true;
}

void connectAWS(){
    connectWiFi();
    syncTime();

    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);

    mqttClient.setServer(AWS_IOT_ENDPOINT, MQTT_PORT);

    mqttClient.setBufferSize(512);
}