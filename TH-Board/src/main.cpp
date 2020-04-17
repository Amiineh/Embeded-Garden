// copy right: https://create.arduino.cc/projecthub/Dcube/temperature-and-humidity-monitoring-using-sht25-and-arduino-baef40

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <EasyTransfer.h>

struct TempHumidity {
  float temperature ;
  float humidity ;
};


SoftwareSerial Bluetooth(2, 3); 
const int Addr = 0x40; // I2C address for SHT25
TempHumidity data;
EasyTransfer ETout; 

float getHumidity();
float getTemp();
void sendData();

void setup() {  
  Wire.begin();  
  Bluetooth.begin(9600);
  ETout.begin(details(data), &Bluetooth);
}


void loop()
{ 
  if (Bluetooth.available()){
    
    int data = getHumidity();// + "-" + String(getTemp());
    Bluetooth.write(data);
  }
  else
    Serial.println("Bluetooth device not connected");

// if (Bluetooth.available())
//     Serial.write(Bluetooth.read());
//   if (Serial.available())
//     Bluetooth.write(Serial.read());
  delay(300);
}


float getHumidity(){
  float data[2];
  float humidity = -1;  

  Wire.beginTransmission(Addr);  
  Wire.write(0xF5);   // Send humidity measurement command 
  Wire.endTransmission();   // Stop I2C transmission 
  delay(500);
  Wire.requestFrom(Addr, 2);  // Request 2 bytes of data
  if(Wire.available() == 2)  {    
    data[0] = Wire.read();    // Read 2 bytes of data  
    data[1] = Wire.read();    // humidity msb, humidity lsb     
    humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6; // Convert the data
  }
  return humidity;
}


float getTemp(){
  float data[2];
  float temp = -1;

  Wire.beginTransmission(Addr);  
  Wire.write(0xF3);  // Send temperature measurement command 
  Wire.endTransmission();  // Stop I2C transmission 
  delay(500);
  Wire.requestFrom(Addr, 2);
  if(Wire.available() == 2) {    
    data[0] = Wire.read();    
    data[1] = Wire.read();
    temp = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;    
  }  
  return temp;
}


void sendData(){
  data.temperature = getTemp();
  data.humidity = getHumidity();
  ETout.sendData();
}