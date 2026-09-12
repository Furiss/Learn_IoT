#pragma once

#include <Arduino.h>
#include <DHT.h>

struct DHTread
{
    float temperature;
    float humidity;
};

extern DHT dht;
extern DHTread dhtpayload;

void buttonHandler();