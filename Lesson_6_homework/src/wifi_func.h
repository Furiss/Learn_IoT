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

char payload[128];
snprintf(payload, sizeof(payload),
"{\"temperature\":%.1f,\"humidity\":%.1f,\"lux\":%.1f}", temperature, humidity, lux);
Serial.print("[HTTP] sending:"); Serial.println(payload);
int httpCode = http.POST((u_int8_t*)payload, strlen(payload));

if(httpCode == 200){
    Serial.println("[HTTP] data delivered successfully");
}
else{
    Serial.print("Error code:"); Serial.println(httpCode);
}
http.end();
}
