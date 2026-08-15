#include <Arduino.h>

unsigned long lastSendorRead = 0;
#define SENSOR_INTERVAL 20000
unsigned long lastHeapRead = 0;
#define FREEHEEP_INTERVAL 60000

struct SensorData{
    float temperature;
    int humidity;
    unsigned long timestamp;    
};
SensorData sensors;


void setup() {
    Serial.begin(115200);

}

void loop() {
sensors.temperature = random(15,30);
sensors.humidity = random(30,65);
sensors.timestamp = millis();


static int readNumber = 1;
unsigned long now = millis();
if(now - lastSendorRead >= SENSOR_INTERVAL) {
    lastSendorRead = now;

    Serial.print("  READ NUMBER ");
    Serial.println(readNumber);
    readNumber++;

    Serial.print("Temperature: ");
    Serial.print(sensors.temperature);
    Serial.println(" C");

    Serial.print("Humidity:    ");
    Serial.print(sensors.humidity);
    Serial.println(" %");

    Serial.print("Time passed: ");
    Serial.print(sensors.timestamp);
    Serial.println(" ms");
 
    if(now - lastHeapRead >= FREEHEEP_INTERVAL) {
    lastHeapRead = now;
    Serial.print("Free heap:");
    Serial.println(ESP.getFreeHeap());
 }
 Serial.println();
}
}