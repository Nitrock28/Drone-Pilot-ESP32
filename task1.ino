
#include <SPI.h>
#include "src/IMU/IMU.h"
#include "src/servos/Servos.h"

#define PIN_MISO 12
#define PIN_MOSI 13
#define PIN_CLK 14
#define PIN_SEL_IMU 15
#define PIN_SEL_BARO 18
#define PIN_SEL_SD 4

// Task 1 entry point, handles
/// -IMU 
/// -Barometer (not used yet)
/// -SD writes (not used yet)
/// -Receiver
/// -Control algorithms
/// -Servos 
void run_task1(void *pvParameters)
{
  // SPI config
  SPI = SPIClass(HSPI);
  SPI.begin(PIN_CLK,PIN_MISO, PIN_MOSI, PIN_SEL_IMU);


  // IMU init
  IMU::init(&SPI,PIN_SEL_IMU,PIN_SEL_BARO);

  float data[9];
  float angles[3];

  uint32_t Now = micros();
  uint32_t lastTime = Now;
  int count=0;

  // Servos Init

  //Throttle, aux, ailLeft, ailRight
  uint servosPins[4] = {19,21,22,23};
  uint servoMicros[4] = {1500, 1000, 1500, 2000};

  Servos::init(servosPins, servoMicros, 4);


  // Startup code
  IMU::calibrateSensor(LED_BUILTIN);
#ifdef DEBUG_SERIAL
  Serial.println("-----");
  Serial.println(configTICK_RATE_HZ);
  Serial.println(portTICK_PERIOD_MS);
#endif
  // main Loop
  while (task0_running) {
    Now = micros();
    if((Now-lastTime)>5000U){
      // digitalWrite(LED_BUILTIN,LOW);
      IMU::getBMPData(data);
      //IMU::printsyncData(data,&pc);
      // // we must pass -ax, ay, az, gx, -gy, -gz, my, -mx, mz because sensor is not aligned
      IMU::MadgwickAHRSupdate(-data[0],data[1],data[2],data[3],-data[4],-data[5],data[7],-data[6],data[8],Now);
      IMU::getEulerAngles(angles);
      // digitalWrite(LED_BUILTIN,HIGH);
      lastTime = Now;
      count++;
      if(count>198){
#ifdef DEBUG_SERIAL
          Serial.print("Roll Pitch Yaw : ");
          Serial.print(angles[0]);
          Serial.print(" | ");
          Serial.print(angles[1]);
          Serial.print(" | ");
          Serial.println(angles[2]);
#endif



          count=0;
          
      }
      servoMicros[0] = 1000+(int)((angles[1]+90.0)*5.555555);
      
      
    }
    Servos::updateServos(servoMicros);

    // minimal delay in the loop, allows watchdog to perform its tasks
    vTaskDelay(1);
  }
  vTaskDelete( NULL );
}
