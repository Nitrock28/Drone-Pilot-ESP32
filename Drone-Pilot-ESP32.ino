#include "OTA.h"
#include <TelnetStream.h>
// #include "task0.ino"
// #include "task1.ino"


static TaskHandle_t task1_handle;
static TaskHandle_t task2_handle = NULL;

unsigned long entry;
char buffer[100];
bool OTAMode = false;

int button_pin = 27;
bool led = false;


void setup() {
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(2,OUTPUT);
  digitalWrite(2, HIGH);

  xTaskCreatePinnedToCore(run_task0, "Task0", 2048, NULL, 1, &task1_handle, 0);
  xTaskCreatePinnedToCore(run_task1, "Task1", 2048, NULL, 1, &task2_handle, 1);


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

    TelnetStream.print("on core : ");
    TelnetStream.println(xPortGetCoreID());

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
