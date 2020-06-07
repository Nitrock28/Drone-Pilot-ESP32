#include "OTA.h"
#include <TelnetStream.h>

// #define ESP32_RTOS  // Uncomment this line if you want to use the code with freertos (only works on the ESP32)

unsigned long entry;
char buffer[100];
bool OTAMode = false;

int button_pin = 27;
bool led = false;
void setup() {
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(2,OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  digitalWrite(2, !digitalRead(2));
  
  if(!digitalRead(button_pin) and ! OTAMode){
    // setup OTA
    Serial.begin(115200);
    Serial.println("Enabling OTA");
    setupOTA("ESP32_0");
    TelnetStream.begin();
    OTAMode = true;
  }

  if(OTAMode){
    ArduinoOTA.handle();

    entry=micros();
    int i=0;
    while(TelnetStream.available()){
      buffer[i] = TelnetStream.read();
      i++;
    }
    buffer[i] = '\0';

    TelnetStream.print("Loop time : ");
    TelnetStream.println(micros()-entry);
    TelnetStream.print("received : ");
    TelnetStream.println(buffer);
    delay(1000);
  }
  else{
    
    delay(100);
    
  }

}
