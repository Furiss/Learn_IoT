#include <Arduino.h>

#include "config.h"
#include "sensor/sensor.h"
#include "wifi_mqtt/wifi_mqtt.h"

unsigned long lastDHTread = 0;
unsigned long lastPublish = 0;
unsigned long lastReconnect = 0;

int reconnectAttempts = 1;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("starting session");

    dht.begin();

    pinMode(LED_PIN, OUTPUT);
    pinMode(DHTT_PIN, INPUT);

    connectAWS();

    mqttClient.setKeepAlive(60);
    mqttClient.setSocketTimeout(30);

    connectMQTT();
}

void loop()
{
    unsigned long now = millis();

if (now - lastDHTread > DHT_INTERVAL)
{
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h))
    {
        Serial.println("DHT read error, skipping");
    }
    else
    {
        dhtpayload.temperature = t;
        dhtpayload.humidity = h;
    }

    lastDHTread = now;
}

    if (mqttClient.connected())
    {
        reconnectAttempts = 1;

        mqttClient.loop();

        if (now - lastPublish > PUBLISH_INTERVAL)
        {
            lastPublish = now;

            publishData(
                dhtpayload.temperature,
                dhtpayload.humidity
            );
        }
    }
    else
    {
        if (reconnectAttempts < 4)
        {
            if (now - lastReconnect > RECONNECT_INTERVAL)
            {
                lastReconnect = now;

                Serial.print(
                    "MQTT lost connection. Reconnecting attempt: "
                );
                Serial.println(reconnectAttempts);

                connectMQTT();
                reconnectAttempts++;
            }
        }
        else
        {
            Serial.println(
                "Too many reconnection attempts. Restarting device"
            );

            delay(1000);
            ESP.restart();
        }
    }
}