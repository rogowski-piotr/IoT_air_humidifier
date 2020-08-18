// PROPERTIES:
#define FOGGER_PIN 8
#define MEASUREMENT_FREQUENCY 5         
#define DHTPIN D2                       // numer pinu sygnałowego
#define DHTTYPE DHT11                   // typ czujnika (DHT11). Jesli posiadamy DHT22 wybieramy DHT22
#define TRANSMITION_TIMEOUT 5000

#include <ArduinoJson.h>

//DHT dht(DHTPIN, DHTTYPE); 

float typedValue, measuredHumidityValue, measuredTemperature;
int timeInterval = 5000, workTime = 2000, workCounter = 0;
const int transmitionLength = 3;
unsigned long timestart = 0;
String jsonResponse="json";


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
  //dht.begin(); 
}

void serialRead() {
  if (Serial.available() != 0){
    String request = Serial.readStringUntil('\r');    // read client/server request 
    String responseMsg;

    if(request[0] != 0x7E){         // '~'
      responseMsg = "ERROR: Wrong beginning of the transmition message";
    } 
    else if(request[2] != 0x7E){    // 0x7F DEL key
      responseMsg = "ERROR: Wrong ending of the transmition message";
    } 
    else {
      responseMsg = "SUCCESS: Transmition received succesfully";
      typedValue = (float)request[1];
    }
    /*const size_t capacity = JSON_OBJECT_SIZE(4);
    DynamicJsonDocument doc(capacity);
    
    doc["temp"] = 25.05;
    doc["humi"] = 50.25;
    doc["transmitionMsg"] = "OK";
    doc["measurmentMsg"] = "ERROR";
    
    serializeJson(doc, Serial);*/
    Serial.print(responseMsg);
  }
}


void humidityMeasurement(){
  if(workCounter == MEASUREMENT_FREQUENCY){             // check if counter has achieved MEASUREMENT_FREQUENCY value
      workCounter = 0;                                  // zeroing the counter
      measuredHumidityValue = 60;
      measuredTemperature = 25;
      /*measuredTemperature = dht.readTemperature();
      measuredHumidityValue = dht.readHumidity();
    */
      if (isnan(measuredTemperature) || isnan(measuredHumidityValue)) {
        Serial.write("ERROR: Measurement error");
        measuredHumidityValue = 50;                           
      } else{
        serialSend();
      }
   }
   
}

void serialSend(){

  
  Serial.println(jsonResponse);
}

void setDelayAndWorkTime(){

}

void waitAndSpray() {
  delay(timeInterval);
  digitalWrite(FOGGER_PIN, HIGH);
  delay(workTime);
  digitalWrite(FOGGER_PIN, LOW);
  workCounter++;
}
