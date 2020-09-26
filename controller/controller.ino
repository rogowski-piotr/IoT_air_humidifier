// PROPERTIES:
#define FOGGER_PIN 8
#define MEASUREMENT_FREQUENCY 5      
#define DHTPIN 4
#define DHTTYPE DHT11
#define TRANSMITION_TIMEOUT 5000

#include "DHT.h"
#include <ArduinoJson.h>

DHT dht(DHTPIN, DHTTYPE); 

float typedHumidity, measuredHumidity, measuredTemperature;
int timeInterval = 2000, workTime = 1000, workCounter = 0;
const int transmitionLength = 3;
unsigned long timestart = 0;
String measurmentMsg, jsonResponse="json";

void setup() {
  initialize();
}

void loop() {
  serialRead();
  humidityMeasurement();
  setDelayAndWorkTime();
  waitAndSpray();
}

void initialize(){
  Serial.begin(9600);
  pinMode(FOGGER_PIN, OUTPUT);
  dht.begin(); 
}

void serialRead() {
  if (Serial.available() != 0){
    String request = Serial.readStringUntil('\r');    // read client/server request 
    String transmitionMsg;

    if(request[0] != 0x7E){         // '~'
      transmitionMsg = "ERROR: Wrong beginning of the transmition message";
    } else if(request[2] != 0x7F){  // DEL key
      transmitionMsg = "ERROR: Wrong ending of the transmition message";
    } else {
      transmitionMsg = "OK: Transmition received succesfully";
      typedHumidity = (float)request[1];
    }
    const size_t capacity = JSON_OBJECT_SIZE(4);
    DynamicJsonDocument doc(capacity);
    
    doc["temp"] = measuredTemperature;
    doc["humi"] = measuredHumidity;
    doc["transmitionMsg"] = transmitionMsg.c_str();
    doc["measurmentMsg"] = measurmentMsg.c_str();
    
    serializeJson(doc, Serial);
  }
}

void humidityMeasurement(){
  if(workCounter == MEASUREMENT_FREQUENCY){             // check if counter has achieved MEASUREMENT_FREQUENCY value
      workCounter = 0;                                  // zeroing the counter
      
      measuredTemperature = dht.readTemperature();
      measuredHumidity = dht.readHumidity();
    
      if (isnan(measuredTemperature) || isnan(measuredHumidity)) {
        measurmentMsg = "ERROR";                      
      } else {
        measurmentMsg = "OK"; 
      }
   }
}


void setDelayAndWorkTime(){
  //timeInterval = 2000; 
  //workTime = 1000,
}

void waitAndSpray() {
  delay(timeInterval);
  digitalWrite(FOGGER_PIN, HIGH);
  delay(workTime);
  digitalWrite(FOGGER_PIN, LOW);
  workCounter++;
}