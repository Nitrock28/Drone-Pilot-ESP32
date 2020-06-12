#include <Arduino.h>
#include <TelnetStream.h>
#include <WiFi.h>
#include "OTA.h"
#include "src/GPS/GPS.h"

#define PIN_GPS_RX 16
#define PIN_GPS_TX 17

unsigned long entry;
unsigned long exit1;
char buffer[100];
bool RunMode = true;

int button_pin = 27;
bool led = false;

// slow task to handle Serial for GPS (1Hz),
// all waypoint navigation scrips
// OTA/EERPOM configuration through Telnet after button press.
void run_task0(void *pvParameters)
{
  //setup
  WiFi.mode(WIFI_OFF);


#ifdef DEBUG_SERIAL
  Serial.begin(115200);
#endif
  
  
  entry=micros();
  pinMode(button_pin, INPUT_PULLUP);
  Serial1.begin(9600,SERIAL_8N1,PIN_GPS_RX,PIN_GPS_TX);
  GPS::init(&Serial1);

  int32_t target[]={507871260, 29582710,1000000};

  GPS::RegisterHome(target);
  target[0]+=1000;
  GPS::RegisterTarget(-1, -1, target);
  GPS::activateNextTarget();

  int32_t gpsOutput[10];
  uint32_t LASTiN = 0;

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
      GPS::handleSerial();
      if(GPS::dataAvailable()){
        uint in = micros();
        // GPS::getData(gpsOutput);
        GPS::getNav(in, gpsOutput);
        uint out = micros();
#ifdef DEBUG_SERIAL
        Serial.print("data from gps : ");
        for(int i=0;i<10;i++){
          Serial.print(gpsOutput[i]);
          Serial.print(", ");
        }
        Serial <<(out-in)<< "micros\r\n";
#endif  
//         LASTiN = in;
      }
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
