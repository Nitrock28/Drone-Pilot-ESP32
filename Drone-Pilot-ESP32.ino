

#include "OTA.h"
#include <TelnetStream.h>
#include <SPI.h>

extern TaskHandle_t loopTaskHandle;
TaskHandle_t task0_handle = NULL;

unsigned long entry;
unsigned long exit1;
char buffer[100];
bool RunMode = true;

int button_pin = 27;
bool led = false;

bool task0_running = true;


void setup() {

  WiFi.mode(WIFI_OFF);
  btStop();
  Serial.begin(115200);
  
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(2,OUTPUT);
  digitalWrite(2, HIGH);

  // fast task launched on core 0
  xTaskCreatePinnedToCore(run_task0, "Task0", 8192, NULL, 1, &task0_handle, 0);
  entry=micros();
}

void loop() {
  // this runs on core 1, 


  if(!digitalRead(button_pin) and RunMode){
    // switch to OTA mode, stop all processes and wait for update. 
    task0_running = false;
    setupOTA("Drone_Pilot");
    TelnetStream.begin();
    TelnetStream.println("Enabling OTA");
    RunMode = false;
  }

  if(RunMode){
    run_task1();
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
