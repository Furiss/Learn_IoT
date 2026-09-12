#include "sensor.h"
#include "../config.h"
#include "../wifi_mqtt/wifi_mqtt.h"

DHT dht(DHTT_PIN, DHTT_TYPE);
DHTread dhtpayload;

static unsigned long lastDebounce = 0;
static bool lastButtonState = HIGH;
static bool buttonState = HIGH;

static void buttonAction()
{
    bool ok = mqttClient.publish(TOPIC_COMANDS, "manual_read");
    Serial.println(ok ? "Comand sended" : "Comand failed");
}

void buttonHandler()
{
    bool reading = digitalRead(BUTTON_PIN);

    if (reading != lastButtonState)
    {
        lastDebounce = millis();
    }

    if ((millis() - lastDebounce) > DEBOUNCE)
    {
        if (reading != buttonState)
        {
            buttonState = reading;

            if (buttonState == LOW)
            {
                buttonAction();
            }
        }
    }

    lastButtonState = reading;
}