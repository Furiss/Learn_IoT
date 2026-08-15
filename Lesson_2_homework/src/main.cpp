#include <Arduino.h>

#define LED 4
#define DOT_MS 200
#define DASH_MS 600

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  
}

void loop() {


for (int i=0; i<3; i++){
  Serial.print(".");
  digitalWrite(LED, HIGH);
  delay(DOT_MS);
  digitalWrite(LED, LOW);
  delay(DOT_MS);
}
for (int i=0; i<3; i++){
  Serial.print("-");
  digitalWrite(LED, HIGH);
  delay(DASH_MS);
  digitalWrite(LED, LOW);
  delay(DOT_MS);//встановив інтервал між тере довжиною як крапка. Сподіваюсь так і треба було
}
for (int i=0; i<3; i++){
  Serial.print(".");
  digitalWrite(LED, HIGH);
  delay(DOT_MS);
  digitalWrite(LED, LOW);
  delay(DOT_MS);
}
Serial.println();
}