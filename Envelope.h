/*
    Envelope.h - Library for controlling a speakjet envelope registers
    Released into the public domain.
*/

#ifndef Envelope_h
#define Envelope_h

#include "Arduino.h"
#include "Register.h"

// speakjet understands these values as 
// wavetypes in its envelope control register
#define SAW 0
#define SINE 64
#define TRI 128
#define SQ 192

#define A_DIFF 2
#define B_DIFF 1

class Envelope
{
    public:
	    Envelope();
                
        Register frequency;
        Register control;
        
        boolean a_is_on;
        boolean b_is_on;
        
        void begin(int frequency_register, int control_register, int tx, int rx);
        
        void set_saw();
        void set_sine();
        void set_tri();
        void set_sq();
        
        void group_a_on();
        void group_a_off();
        void group_b_on();
        void group_b_off();                        
    
    private:
        void _set_wave(int type);
        int _wave_value;
};

#endif