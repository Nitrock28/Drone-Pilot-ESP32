#include "test.h"

namespace test{
    // public fields
    void init(){
        digitalWrite(LED_BUILTIN,LOW);
        delay(1000);
        digitalWrite(LED_BUILTIN,HIGH);
        Serial.println("in test");
    }
};
