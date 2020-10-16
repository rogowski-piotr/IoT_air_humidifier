# ESP8266 Connect Aplication
Thanks to working with two modules, our project behaves as if it was multi-threaded. The server module (ESP8266) is responsible for communication between the client and the server. The comunication between server and controller using serial communication. The controller module (UNO) is responsible for the fogger module operation control to achieve value of humidity set via client application.

## Hardware Requirements
- The server module - [ESP8266](https://www.espressif.com/en/products/socs/esp8266) (wemos d1 r1)
- The controller module - [Arduino UNO](https://store.arduino.cc/arduino-uno-rev3)
- DHT11 Sensor
- Mist maker (fogger)

## Software Requirements
- [Arduino IDE](https://www.arduino.cc/en/Main/software)
- [Arduino ESP8266 library](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
- [ArduinoJson library v6](https://arduinojson.org/)
- [Android application](https://github.com/rogowski-piotr/IoT_android_air_humidifier)

## Setup
Open Arduino IDE and install all nesesary libraries. Open controller.ino and upload to Arduino UNO (controller module) then upload server.ino to ESP8266 (server module). Connect all elements like an below schema.

![schema](https://github.com/rogowski-piotr/IoT_air_humidifier/blob/master/schema.png)

- Set network access in one of the first lines of code server.ino:

```cpp
const char* ssid = " **here ";
const char* password = " **here";
```