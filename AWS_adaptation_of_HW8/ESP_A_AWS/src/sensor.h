#pragma once
#include "config.h"
#include "DHT.h"
#include "wifi_mqtt.h"

struct DHTread
{
    float temperature;
    float humidity;
};

DHT dht(DHTT_PIN, DHTT_TYPE);
DHTread dhtpayload;



//debounce
unsigned long lastDebounce = 0;
bool lastButtonState = HIGH;
bool buttonState = HIGH;

void ButtonAction(){
    bool ok = mqttClient.publish(TOPIC_COMANDS, "manual_read");
    Serial.println(ok ? "Comand sended" : "Comand failed");
}
void buttonHandler() {
    bool reading = digitalRead(BUTTON_PIN);
   
    if (reading != lastButtonState) {
        lastDebounce = millis();
    }
    if ((millis() - lastDebounce) > DEBOUNCE) {
        if (reading != buttonState) {
            buttonState = reading;
            if (buttonState == LOW) {
                ButtonAction();
            }
        }
    }

    lastButtonState = reading;
}


