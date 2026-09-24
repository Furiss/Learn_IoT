#include "wifi_mqtt.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "config.h"
#include "secrets.h"

static WiFiClientSecure net;
PubSubClient mqttClient(net);

bool connectWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - start > WIFI_TIMEOUT)
        {
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

bool connectMQTT()
{
    Serial.print("Connecting to AWS IoT Core...");

    if (mqttClient.connect(THINGNAME))
    {
        Serial.println("SUCCESS");
        bool subscribed = mqttClient.subscribe(TOPIC_COMMANDS, 1);
        Serial.print("Subscribe to: ");
        Serial.print(TOPIC_COMMANDS);
        Serial.print("...  ");
        Serial.println(subscribed ? "SUCCESS" : "FAILED");
        
        return true;
    }

    Serial.print("Error: ");
    Serial.println(mqttClient.state());

    net.stop();
    return false;
}

void publishData(float temperature, float humidity)
{
    if (!mqttClient.connected())
    {
        Serial.println("No connection, skipping");
        return;
    }

    time_t now;
    time(&now);

    JsonDocument doc;

    doc["device_id"] = THINGNAME;
    doc["timestamp"] = (unsigned long)now;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;

    char payload[160];

    serializeJson(doc, payload, sizeof(payload));

    Serial.print("Publishing: ");
    Serial.println(payload);

    bool ok = mqttClient.publish(TOPIC_SENSORS, payload);

    Serial.println(ok ? "SUCCESS" : "Publishing ERROR");
}


char commandAction[16] = "";
char commandValue[16] = "";
bool commandReceived = false;


void onMessage(char* topic, byte* payload, unsigned int length){

JsonDocument doc;

DeserializationError error = deserializeJson(doc, payload, length);

if(error){
    Serial.print("JSON parsing error: ");
    Serial.println(error.c_str());
    return;
}

strlcpy(commandAction, doc["action"] | "", sizeof(commandAction));
strlcpy(commandValue, doc["value"] | "", sizeof(commandValue));

commandReceived = true;

}

void publishEvent(const char* value){

    time_t now;
    time(&now);

    JsonDocument doc;

    doc["device_id"] = THINGNAME;
    doc["timestamp"] = (unsigned long)now;
    doc["event"] = "led_changed";
    doc["value"] = value;

    char payload[160];

    serializeJson(doc, payload, sizeof(payload));
    bool ok = mqttClient.publish(TOPIC_EVENTS, payload);
    Serial.println(ok ? "LED change published on iot-course/vlasenko/events" : "publish failed");

}

void handleCommand(){
    if(commandReceived){
        if(strcmp(commandAction, "set") == 0 && strcmp(commandValue, "on") == 0){
            digitalWrite(LED_PIN, HIGH);
            Serial.println("LED ON");
            publishEvent(commandValue);


        }
        else if(strcmp(commandAction, "set") == 0 && strcmp(commandValue, "off") == 0){
            digitalWrite(LED_PIN, LOW);
            Serial.println("LED OFF");
            publishEvent(commandValue);

        }
    else{
        Serial.println("Incorrect command");
    }
        commandReceived = false;
    }

}

bool syncTime()
{
    Serial.print("Synchronize time...");

    configTime(0, 0, "pool.ntp.org");

    struct tm timeinfo;
    unsigned long start = millis();

    while (!getLocalTime(&timeinfo))
    {
        if (millis() - start > TIME_SYNC_TIMEOUT)
        {
            Serial.println("TIMEOUT");
            return false;
        }

        delay(500);
        Serial.print(".");
    }

    Serial.println("SUCCESS");
    return true;
}

void connectAWS()
{
    connectWiFi();
    syncTime();

    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);

    mqttClient.setServer(AWS_IOT_ENDPOINT, MQTT_PORT);
    mqttClient.setBufferSize(512);

}