#pragma once
#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

unsigned long lastSend = 0;

bool connectWiFi(){
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

void sendData(float temperature, float humidity, float lux) {
if(WiFi.status() != WL_CONNECTED){
    Serial.println("No connecton. Skip sending");
    return;
}
HTTPClient http;
http.begin(SERVER_URL);
http.addHeader("Content-Type", "application/json");

char tBuf[16], hBuf[16];

if (sensorpayload.statuscheck & STATUS_DHT_ERR) {
strcpy(tBuf, "null");
strcpy(hBuf, "null");
} else {
snprintf(tBuf, sizeof(tBuf), "%.1f", dhttpayload.temperature);
snprintf(hBuf, sizeof(hBuf), "%.1f", dhttpayload.humidity);
}

char payload[160];
snprintf(payload, sizeof(payload),
"{\"temperature\":%s,\"humidity\":%s,\"lux\":%.1f,\"status\":%u}",
tBuf, hBuf, ldrpayload.lux, sensorpayload.statuscheck);
Serial.print("[HTTP] sending:"); Serial.println(payload);
int httpCode = http.POST((uint8_t*)payload, strlen(payload));

if(httpCode == 200){
    Serial.println("[HTTP] data delivered successfully");
}
else{
    Serial.print("Error code:"); Serial.println(httpCode);
}
http.end();
}
