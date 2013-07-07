/*
    Osc.h - Library for controlling a speakjet oscillator
    Released into the public domain.
*/

#ifndef Osc_h
#define Osc_h

#include "Arduino.h"
#include "Register.h"

class Osc
{
    public:
	    Osc();
        
        Register volume;
        Register frequency;    
        
        void begin(int frequency_register, int volume_register, int tx, int rx);
    private:
};

#endif