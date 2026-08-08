#include <Arduino.h>

#define LED 4


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  
}

void loop() {
  Serial.println(".");
  digitalWrite(LED, HIGH);
  delay(200);
  Serial.println("-");
  digitalWrite(LED, LOW);
  delay(600);
}