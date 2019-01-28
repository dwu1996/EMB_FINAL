/*
開發備註：序列阜監控和LCD輸出會衝突= = 所以不要開序列阜...
*/
#include <Arduino_FreeRTOS.h>
#include <Wire.h>  
#include <queue.h>
#include <Stepper.h>
#include <TroykaDHT.h>
#define STEPS 2048
#define LandShouldWet 400
Stepper stepper(STEPS, 8, 10, 9, 11);
DHT dht(6, DHT11);

int LandWetSensor = A2;
int LandWet = 0;
int LightSensor = A0;
int Light = 0;
int timestamp = millis();
float AirWet = 0, Ctemp = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  stepper.setSpeed(15); // set the speed to 15 RPMs
  pinMode(LandWetSensor, INPUT); 
}

void loop() {
  //stepper.step(512);
  dht.read();

  switch(dht.getState()) {
    case DHT_OK:
      break;
    case DHT_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;
    case DHT_ERROR_TIMEOUT:
      Serial.println("Time out error");
      break;
    case DHT_ERROR_NO_REPLY:
      Serial.println("Sensor not connected");
      break;
  }
  
  AirWet = dht.getHumidity();//讀取濕度
  Ctemp = dht.getTemperatureC();//讀取攝氏溫度
  LandWet = analogRead(LandWetSensor);
  Light = analogRead(LightSensor);
  Serial.println("================================");
  Serial.print("= Land Wet = ");
  Serial.println(LandWet);
  Serial.print("= Air Wet = ");
  Serial.print(AirWet);
  Serial.println("%");
  Serial.print("= Light = ");
  Serial.println(Light);
  Serial.print("= Celsius Temp. = ");
  Serial.print(Ctemp);
  Serial.println("*C");
  Serial.println("================================");
  if(LandWet>LandShouldWet){stepper.step(512);}
  delay(2000);
}
