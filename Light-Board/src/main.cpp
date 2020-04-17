// light board
#include <Arduino.h>
#include<SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);
int LDR_pin = A0;
float lux=0.00,ADC_value=0.0048828125,LDR_value;

void setup() {
  Serial.begin(9600); 
  mySerial.begin(9600);
}

void loop() {
  LDR_value = analogRead(LDR_pin); 
  lux=(250.000000/(ADC_value*LDR_value))-50.000000;
  char luxStr[16];
  itoa(lux, luxStr, 10);
  Serial.write(luxStr);
  mySerial.write(lux);

  delay(1000);
}