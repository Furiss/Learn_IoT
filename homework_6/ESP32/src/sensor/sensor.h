#pragma once

#include <Arduino.h>
#include <DHT.h>

struct DHTread
{
    float temperature;
    float humidity;
    bool valid = false;
};

extern DHT dht;
extern DHTread dhtpayload;
