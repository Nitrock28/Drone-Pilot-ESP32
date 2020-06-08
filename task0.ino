
#include <SPI.h>
#include "src/IMU/IMU.h"

#define PIN_MISO 12
#define PIN_MOSI 13
#define PIN_CLK 14
#define PIN_SEL_IMU 15
#define PIN_SEL_BARO 18
#define PIN_SEL_SD 4


void run_task0(void *pvParameters)
{
  SPI = SPIClass(HSPI);
  SPI.begin(PIN_CLK,PIN_MISO, PIN_MOSI, PIN_SEL_IMU);

  IMU::init(&SPI,PIN_SEL_IMU,PIN_SEL_BARO);

  float data[9];
  float angles[3];

  IMU::calibrateSensor(LED_BUILTIN);

  uint32_t Now = micros();
  uint32_t lastTime = Now;
  int count=0;

  while (task0_running) {
    Now = micros();
    if((Now-lastTime)>5000U){
      digitalWrite(LED_BUILTIN,LOW);
      IMU::getBMPData(data);
      //IMU::printsyncData(data,&pc);
      // // we must pass -ax, ay, az, gx, -gy, -gz, my, -mx, mz because sensor is not aligned
      IMU::MadgwickAHRSupdate(-data[0],data[1],data[2],data[3],-data[4],-data[5],data[7],-data[6],data[8],Now);
      digitalWrite(LED_BUILTIN,HIGH);
      lastTime = Now;
      count++;
      if(count>198){
          IMU::getEulerAngles(angles);
          Serial.print("Roll Pitch Yaw : ");
          Serial.print(angles[0]);
          Serial.print(" | ");
          Serial.print(angles[1]);
          Serial.print(" | ");
          Serial.println(angles[2]);
          count=0;
      }
    }



    delay(1);
  }
  vTaskDelete( NULL );
}
