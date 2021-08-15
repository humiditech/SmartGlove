#include <Arduino.h>
#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <SoftwareSerial.h>
#include <Adafruit_MLX90614.h>

#define Debug false

unsigned long readrequesttime = 0;

SoftwareSerial BLSerial(3,2);
PulseOximeter oximeter;
Adafruit_MLX90614  tempSensor = Adafruit_MLX90614();
unsigned long tsLastReport = 0;

uint8_t SPO2 = 0;
double temp = 0;
String Data;

void Sensorinit();
void ReadSensorData();

void setup() {
  if(Debug){
    Serial.begin(9600);
  }
  BLSerial.begin(9600);
  Sensorinit();
}

void loop() {
  if(millis() - readrequesttime > 50){
    readrequesttime = millis();
    ReadSensorData();
  }
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
  if(BLSerial.available()){
    Data = BLSerial.readString();
  }

  if(Data == "Req"){

    SPO2 = oximeter.getSpO2();
    while(SPO2 >= 100){
      SPO2 = oximeter.getSpO2();
    }

    temp = tempSensor.readObjectTempC();

    String dataTosend = String(SPO2) + "," + String(temp,2) + "\n";
    if(Debug){
      Serial.println(dataTosend);
    }

    BLSerial.print(dataTosend);

    Data = "";
  }
}
