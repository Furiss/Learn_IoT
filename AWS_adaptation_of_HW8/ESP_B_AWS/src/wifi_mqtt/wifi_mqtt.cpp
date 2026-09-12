#include "wifi_mqtt.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "../config.h"
#include "../secrets.h"

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

        mqttClient.subscribe(TOPIC_SENSORS, 1);
        Serial.print("Subscribed to: ");
        Serial.println(TOPIC_SENSORS);

        mqttClient.subscribe(TOPIC_COMANDS, 1);
        Serial.print("Subscribed to: ");
        Serial.println(TOPIC_COMANDS);

        return true;
    }

    Serial.print("Error: ");
    Serial.println(mqttClient.state());

    net.stop();

    return false;
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