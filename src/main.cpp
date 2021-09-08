#include <Arduino.h>
#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <SoftwareSerial.h>
#include <Adafruit_MLX90614.h>
#include "SPI.h"

#define Debug true

unsigned long readrequesttime = 0;

SoftwareSerial BLSerial(3,2);
PulseOximeter oximeter;
Adafruit_MLX90614  tempSensor = Adafruit_MLX90614();
unsigned long tsLastReport = 0;

uint8_t SPO2 = 0;
uint8_t heartBit = 0;
double temp = 0;
String Data;
String dataTosend;

void Sensorinit();
void ReadSensorData();

void setup() {
  if(Debug){
    Serial.begin(9600);
  }
  BLSerial.begin(38400);
  Sensorinit();
}

void loop() {
  oximeter.update();
  if(millis() - readrequesttime > 500){
    readrequesttime = millis();
    ReadSensorData();
  }

  // if(BLSerial.available()){
  //   Data = BLSerial.readStringUntil(',');

  //   if(Debug){
  //     Serial.print("Bluetooth Data: ");
  //     Serial.println(Data);
  //   }

  //   if(Data == "Req"){
  //     BLSerial.print(dataTosend);
  //   }
  // }
}

void Sensorinit(){
  if(!oximeter.begin()){
    if(Debug){
      Serial.println("Oxymeter init failed!");
    }
  }
  else{
    if(Debug){
      Serial.println("Oxymeter init succes!");
    }
  }

  if(!tempSensor.begin()){
    if(Debug){
      Serial.println("temp sensor init failed!");
    }    
  }
  else{
    if(Debug){
      Serial.println("temp sensor init succes!");
    }    
  }
}

void ReadSensorData(){
  // if(BLSerial.available()){
  //   Data = BLSerial.readStringUntil(',');
  // }

  // if(Data == "Req"){

    SPO2 = oximeter.getSpO2();
    while(SPO2 >= 100){
      SPO2 = oximeter.getSpO2();
    }

    heartBit = oximeter.getHeartRate();

    temp = tempSensor.readObjectTempC();

    dataTosend = String(SPO2) + "," + String(heartBit) + "," + String(temp,2) + "\n";
    if(Debug){
      Serial.println(dataTosend);
    }

    BLSerial.print(dataTosend);

    Data = "";
  // }
}
