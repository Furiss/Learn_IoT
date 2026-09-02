#include <Arduino.h>
#include "sensor.h"
#include "wifi_func.h"

//debounce
unsigned long lastDebounce = 0;
bool lastButtonState = HIGH;
bool buttonState = HIGH;
bool switch_state = LOW;


void handleButton(){
    bool reading = digitalRead(BUTTON_PIN);

    if(reading != lastButtonState){
        lastDebounce = millis();
    }
    if (millis() - lastDebounce > DEBOUNCE) {
        if (reading != buttonState) {
            buttonState = reading;
            if (buttonState == LOW) {
                switch_state = !switch_state;
                Serial.println(switch_state ? "Device turned ON" : "Device turned OFF");
            }
        }
    }

    lastButtonState = reading;
}



void setup() {
    Serial.begin(115200);
    delay(1000);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LDR_PIN, INPUT);
    pinMode(DHTT_PIN, INPUT);
    dht.begin();
    Serial.println("started");
    connectWiFi();
}

void loop() {
handleButton();

if(switch_state){
    unsigned long now = millis();
    static int readNumber = 1;

    if ((now - lastReadDHT) > DHT_INTERVAL) {
        dhttpayload.humidity = dht.readHumidity();
        dhttpayload.temperature = dht.readTemperature();
        Serial.println("-----------------------------------------------");
        Serial.println("   [DHT]");
        Serial.print("Temperature: "); Serial.print(dhttpayload.temperature, 1); Serial.println(" C");
        Serial.print("Humidity:    "); Serial.print(dhttpayload.humidity, 1); Serial.println(" %");
        lastReadDHT = now;
    }

    if ((now - lastReadLDR) > LDR_INTERVAL) {
        ldrpayload.raw = analogRead(LDR_PIN);
        ldrpayload.lux = adcToLux(ldrpayload.raw);
        Serial.println("   [LDR]");
        Serial.print("raw data: "); Serial.println(ldrpayload.raw);
        Serial.print("lux:      "); Serial.print(ldrpayload.lux, 1); Serial.println(" lux");
        Serial.print("READ NUMBER  ["); Serial.print(readNumber); Serial.println("]  COMPLETE");
            readNumber++;
        lastReadLDR = now;
        Serial.println("-----------------------------------------------");

        if (ldrpayload.lux < LED_ON_THRESHOLD) {
            digitalWrite(LED_PIN, HIGH);
        } else {
            digitalWrite(LED_PIN, LOW);
        }

    }
    
        if ((now - lastCollection) > COLECTION_INTERVAL) {
            sensorpayload.dhtt = dhttpayload;
            sensorpayload.ldr = ldrpayload;
            sensorpayload.timestamp = millis();
            sensorpayload.statuscheck = validateSensors();
            lastCollection = now;
            Serial.println("Data collected");
            printStatus();
        }
            if ((now - lastSend) > SEND_INTERVAL) {
            sendData(dhttpayload.temperature, dhttpayload.humidity, ldrpayload.lux);
            lastSend = now;
        }
}
 if (!switch_state) {
    digitalWrite(LED_PIN, LOW);
        return;
    }

}