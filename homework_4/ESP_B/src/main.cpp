#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "wifi_mqtt.h"



unsigned long lastReconnect = 0;
int reconnectAttempts = 1;
bool blinkReq = false;
bool ledStateBefore = LOW;

int blinksLeft = 0;
unsigned long lastBlink = 0;

void onMessage(char* topic, byte* payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';


    if(strcmp(topic, TOPIC_SENSORS) == 0){
    Serial.print("Topic: "); Serial.println(topic);
    Serial.print("Payload: "); Serial.println(message);

    char* tempPtr = strstr(message, "\"temperature\":");
    if (tempPtr == NULL) {
        Serial.println("Temperature not found");
        return;
    }

    float temperature = atof(tempPtr + 14);
    Serial.print("Temperature: "); Serial.println(temperature);

    if (temperature > 26) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED ON - temperature too high!");
    } else if (temperature < 20) {
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED OFF - temperature too low");
    } else {
        Serial.println("No LED changes. Temperature in normal");
    }
}
    else if(strcmp(topic, TOPIC_COMMANDS) == 0){
        if(strcmp(message, "manual_read") == 0){
            Serial.println("Manual trigger received");

            blinkReq = true;

            }
        }
        else{
            Serial.println("Comand recived, but its not a trigger");
        }
    }



void setup() {
Serial.begin(115200);
delay(1000);
Serial.println("starting sessionn");
pinMode(LED_PIN, OUTPUT);
connectWiFi();
mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
mqttClient.setCallback(onMessage);
mqttClient.setKeepAlive(60);
mqttClient.setSocketTimeout(30);
connectMQTT();

}


void loop(){
    if (mqttClient.connected()) {
        reconnectAttempts = 1;
        mqttClient.loop();
    } else {

        unsigned long now = millis();
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
        ESP.restart();
    }
}
if (blinkReq){
    if (blinksLeft == 0){
        ledStateBefore = digitalRead(LED_PIN);
        blinksLeft = BLINK_SWITCHES;
        lastBlink = millis();
    }
    if(millis() - lastBlink >= BLINK_INTERVAL){
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        lastBlink = millis();
        blinksLeft --;
    }
    if(blinksLeft == 0){
        digitalWrite(LED_PIN, ledStateBefore);
        blinkReq = false;
    }
}
}

