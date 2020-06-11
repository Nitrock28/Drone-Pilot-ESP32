#include <Arduino.h>
#include <TelnetStream.h>
#include "OTA.h"

unsigned long entry;
unsigned long exit1;
char buffer[100];
bool RunMode = true;

int button_pin = 27;
bool led = false;
void run_task0(void *pvParameters)
{
  //setup
  entry=micros();
  pinMode(button_pin, INPUT_PULLUP);


  for(;;){

      // this runs on core 0, 


    if(!digitalRead(button_pin) and RunMode){
      // switch to OTA mode, stop all processes and wait for update. 
      task0_running = false;
      setupOTA("Drone_Pilot");
      TelnetStream.begin();
      TelnetStream.println("Enabling OTA");
      RunMode = false;
    }

    if(RunMode){
      vTaskDelay(1);
    }
    else{
      digitalWrite(2, !digitalRead(2));

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
  }

}
