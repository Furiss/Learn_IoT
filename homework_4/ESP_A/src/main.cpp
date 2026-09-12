#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "sensor.h"
#include "wifi_mqtt.h"

unsigned long lastDHTread = 0;
unsigned long lastPublish = 0;
unsigned long lastReconnect = 0;
int reconnectAttempts = 1;


void setup() {
Serial.begin(115200);
delay(1000);
Serial.println("starting sessionn");
dht.begin();
pinMode(LED_PIN, OUTPUT);
pinMode(BUTTON_PIN, INPUT_PULLUP);
pinMode(DHTT_PIN, INPUT);
connectWiFi();
mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
mqttClient.setKeepAlive(60);
mqttClient.setSocketTimeout(30);
connectMQTT();

}


void loop(){
unsigned long now = millis();
if(now - lastDHTread > DHT_INTERVAL){
    dhtpayload.temperature = dht.readTemperature();
    dhtpayload.humidity = dht.readHumidity();
    lastDHTread = now;
}
if(mqttClient.connected()){
    reconnectAttempts = 1;
    mqttClient.loop();
buttonHandler();
if(now - lastPublish > PUBLISH_INTERVAL){
    lastPublish = now;
    publishData(dhtpayload.temperature, dhtpayload.humidity);
    
}
}
else{
    if(reconnectAttempts < 4){
    if(now - lastReconnect > RECONNECT_INTERVAL){
        lastReconnect = now;
        Serial.print(" MQTT lost connection. Reconnecting attempt:  "); Serial.println(reconnectAttempts);
        connectMQTT();
        reconnectAttempts++;
    }
    }
    else {
        Serial.println("To many reconnection attempts. Restartin device");
        delay(1000);
        ESP.restart();//я був не впевнений що краще: перезапускати ESP після 3 спроб, чи просто зупиняти код. Зупинився на перезавантаженні.
    }
}
}