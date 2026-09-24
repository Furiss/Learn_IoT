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
    mqttClient.setCallback(onMessage);
    mqttClient.setKeepAlive(60);
    mqttClient.setSocketTimeout(30);
    connectMQTT();
}

void loop(){


    unsigned long now = millis();

    if (now - lastDHTread > DHT_INTERVAL)
    {
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if (isnan(temperature) || isnan(humidity))
        {
            dhtpayload.valid = false;
            Serial.println("DHT read error");
        }
        else
        {
            dhtpayload.temperature = temperature;
            dhtpayload.humidity = humidity;
            dhtpayload.valid = true;
        }

        lastDHTread = now;
    }

    if (mqttClient.connected())
    {
        reconnectAttempts = 1;

        mqttClient.loop();

        handleCommand();
 

        if (now - lastPublish > PUBLISH_INTERVAL)
        {
            lastPublish = now;

            if (dhtpayload.valid)
            {
                publishData(
                    dhtpayload.temperature,
                    dhtpayload.humidity
                );
            }
            else
            {
                Serial.println("Publish skipped: invalid DHT data");
            }
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