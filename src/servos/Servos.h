#ifndef SERVOS_h
#define SERVOS_h

#include <Arduino.h>

#define MAX_CHANNELS_COUNT 4


namespace Servos
{
        // init the servos connections:
        // the channel array contains the servo pins,
        // those pins will be setup as output
        void init(uint* servosPins, uint* initMicros, uint numChannels);
        
        
        /// function to call in a fast loop, will update the servos
        /// with the value in the table. if the servos are busy, it returns
        /// immediately. micros is a table with values
        ///between 1000 and 2000, some servos can use timings outside this range.
        void updateServos(uint* servoMicros);
};


#endif