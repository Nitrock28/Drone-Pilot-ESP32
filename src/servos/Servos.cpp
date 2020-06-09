
#include "Servos.h"
// Servos pins are fixed, we create the structures in advance



namespace Servos{


    namespace{
        //private variables
        volatile uint _numChannels;
        volatile uint _pins[MAX_CHANNELS_COUNT];
        volatile uint _servoInd = 0;
        volatile bool _busy;
        
        //
        hw_timer_t * timer = NULL;

        //private functions

        //callback to set each pin down
        void ServoCallback() { 
            digitalWrite(_pins[_servoInd],LOW);
            _servoInd = (_servoInd+1)%_numChannels;
            _busy = false;
        }
        
        void scheduleNextCallback(uint servoMicros){
            _busy = true;
            digitalWrite(_pins[_servoInd],HIGH);
            timerAlarmWrite(timer, timerRead(timer)+servoMicros, false);
            timerAlarmEnable(timer);

        }
    }

    //public functions
    
    
    void init(uint* servosPins, uint* initMicros, uint numChannels){

        timer = timerBegin(0, 80, true);
        timerAttachInterrupt(timer, &ServoCallback, true);
        _numChannels = numChannels;
        for (uint i = 0; i < _numChannels; i++)
        {
            _pins[i] = servosPins[i];
            pinMode(_pins[i],OUTPUT);
        }
        
        scheduleNextCallback(initMicros[_servoInd]);
    }
        
    // can be called in a fast loop 
    void updateServos(uint* servoMicros){
        if(!_busy){
            // this is needed, no idea why??
            ulong dummy = timerRead(timer);

            scheduleNextCallback(servoMicros[_servoInd]);
        }
    }
    
}

