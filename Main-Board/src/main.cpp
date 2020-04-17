// main board
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <EasyTransfer.h>
#include <string.h>

struct TempHumidity {
  float temperature ;
  float humidity ;
};

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
SoftwareSerial Bluetooth(2, 3); // for conecting to th-board
SoftwareSerial mySerial(9, 10); // for connecting to light board
TempHumidity thData;
EasyTransfer ETin; // for receiving tempHumidity data struct from th-board
int lightData = 0;

void Water(int flow);
int getLight();


void setup() {
  Bluetooth.begin(9600);
  mySerial.begin(9600);
  lcd.begin(16, 2);
  lcd.println("hello Garden!");
  ETin.begin(details(thData), &Bluetooth);
  delay(100);
}


void loop() {
  if(ETin.receiveData()){
    float temp = thData.temperature;
    float humidity = thData.humidity;

    if (humidity > 80){
      Water(0);
    }
    else if (humidity < 50){
      Water(15);
    }
    else {
      int light = getLight();
      if (temp < 25 && light < 600)
        Water(10);
      else if (temp < 25 && light > 600)
        Water(5);
      else if (temp > 25)
        Water(10);
    }
  }

  else {
    lcd.clear();
    lcd.println("Lost Bluetooth Connection");
  }

  getLight();

  delay(100);
}


void Water(int flow){
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Watering with flow " + String(flow));
  delay(500);
}

int getLight(){
  if (Serial.available()) {
    lcd.clear();
    lcd.println("available");
    String serialData = Serial.readString();
    lightData = serialData.toInt();
    lcd.clear();
    lcd.print("Light: " + serialData);
    return lightData;
  }
  else{
    lcd.clear();
    lcd.println("Lost Connection to Light-board");
    return -1;
  }
}
