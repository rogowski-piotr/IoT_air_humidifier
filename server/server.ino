// PROPERTIES:
#define SERIAL_PORT_TIMEOUT 5000
#define PIN_LED_GREEN D7
#define PIN_LED_RED D8

#include <ESP8266WiFi.h>
 
const char* ssid = "ssid";
const char* password = "password";

char message[3] = {0x7E, 50, 0x7F};
 
WiFiServer server(50007);
 
void setup() {
  initialize();

  connectionStatus(false);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {           // wait until connect
    delay(500); 
  }
  connectionStatus(true);
  server.begin();
}
 
void loop() {  
  WiFiClient client = server.available();           // wait for client
  while (!client) {             
    return; 
  }
  
  String request = client.readStringUntil('\r');    // read client request 

  if (incorrectValidation(request)) {
    client.println("Pojebało cię ??");
    client.flush(); 
    return;
  }

  message[1] = convertRetreivedValue(request);       // convert retrieve data and set it to send

  serialSend(message);                              // send param to controller

  unsigned long timestart = millis();
  while ((millis() - timestart) < SERIAL_PORT_TIMEOUT)
    if (Serial.available() > 0) {
      client.println(Serial.read());                // send the response from controller to client
      client.flush();                               // disconnect client
    }
    
  if ((millis() - timestart) >= SERIAL_PORT_TIMEOUT) {  // send info about timeout
    client.println("SERIAL_PORT_TIMEOUT " + SERIAL_PORT_TIMEOUT);
    client.flush();                                 // disconnect client
  }
}

void initialize() {
  Serial.begin(9600);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
}

bool incorrectValidation(String valueStr) {
  if (valueStr.length() < 1 || valueStr.length() >2) {  // length check-up
    return true;
  }  
  for (int i=0; i<valueStr.length(); i++) {             // digits only check-up
    if (valueStr[i] > 57 || valueStr[i] < 48) {
      return true;
    }  
  }    
  return false;
}

char convertRetreivedValue(String valueStr) {
  return (valueStr.length() < 2) ? (char)(valueStr[0] - 48) : (char)((valueStr[0] - 48) * 10 + valueStr[1] - 48);
}

void serialSend(char msg[3]) {
  for (byte i=0; i<3; i++) { 
    Serial.write(msg[i]); 
  }
}

void connectionStatus(bool isConnected) {
  if (isConnected) {
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_GREEN, HIGH);
  } else {
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
  }
}
