#pragma once
#include <Arduino.h>
#include "config.h"
#include "DHT.h"
#include <WiFi.h>


unsigned long lastReadDHT = 0;
unsigned long lastReadLDR = 0;
unsigned long lastCollection = 0;



struct DHTTData {
    float temperature;
    float humidity;
};
struct LDRData{
    int raw;
    float lux;
};
struct SensorData{
    DHTTData dhtt;
    LDRData ldr;
    unsigned long timestamp;
    uint8_t statuscheck;
};

DHT dht(DHTT_PIN, DHTT_TYPE);
SensorData sensorpayload;
LDRData    ldrpayload;
DHTTData   dhttpayload;


#define GAMMA 0.7f      
#define RL10  50.0f     
#define RDIV  10000.0f  
#define VCC   3.3f      

float adcToLux(int adcValue) {
    float voltage    = adcValue / 4096.0f * VCC;
    float resistance = RDIV * voltage / (VCC - voltage);
    float lux        = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1.0f / GAMMA));
    return lux;
}

uint8_t validateSensors() {
    uint8_t status = STATUS_OK;

    if (ldrpayload.raw < 0 || ldrpayload.raw > 4095) {
        status |= STATUS_LDR_ERR;
    }

    if(isnan(dhttpayload.temperature) || isnan(dhttpayload.humidity)
    || dhttpayload.temperature > 80
    || dhttpayload.temperature <-40
    || dhttpayload.humidity > 100
    || dhttpayload.humidity < 0){
        status |= STATUS_DHT_ERR;
    }
    if(!WiFi.isConnected()){
        status |= STATUS_WIFI_ERR;
    }
    return status;
}

void printStatus(){
    Serial.print("Errors:   0x");
    if (sensorpayload.statuscheck < 16) Serial.print("0");
    Serial.println(sensorpayload.statuscheck, HEX);
    if (sensorpayload.statuscheck & STATUS_LDR_ERR)  Serial.println("  LDR error");
    if (sensorpayload.statuscheck & STATUS_DHT_ERR)  Serial.println("  DHT22 error");
    if (sensorpayload.statuscheck & STATUS_WIFI_ERR) Serial.println("  No WiFi connection");
}
