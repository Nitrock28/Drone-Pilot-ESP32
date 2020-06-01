#include "OTA.h"
#include "src/test/test.h"

// #define ESP32_RTOS  // Uncomment this line if you want to use the code with freertos (only works on the ESP32)

unsigned long entry;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  setupOTA("ESP32_0");
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  entry=micros();
  
  #ifndef ESP32_RTOS
  ArduinoOTA.handle();
  #endif
  
  TelnetStream.println(micros()-entry);
  TelnetStream.println("Loop");
  test::init();
  delay(1000);
}
