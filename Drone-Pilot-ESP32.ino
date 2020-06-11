#include <Arduino.h>
#include <WiFi.h>

#define DEBUG_SERIAL


// tasks handles
TaskHandle_t task0_handle = NULL;
TaskHandle_t task1_handle;


// variables accessible in the two task loops
bool task0_running = true;






void setup() {

  WiFi.mode(WIFI_OFF);
  btStop();

#ifdef DEBUG_SERIAL
  Serial.begin(115200);
#endif
  
  pinMode(2,OUTPUT);// led
  digitalWrite(2, HIGH);

  // one loop task on each core.
  xTaskCreatePinnedToCore(run_task0, "Task0", 8192, NULL, 1, &task0_handle, 0);
  xTaskCreatePinnedToCore(run_task1, "Task1", 8192, NULL, 1, &task1_handle, 1);
  vTaskDelete( NULL );
}

void loop() {
#ifdef DEBUG_SERIAL
  Serial.println("inLoop");
#endif
}
