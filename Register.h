/*
    Register.h - representing a speakjet register
    Released into the public domain.
*/

#ifndef Register_h
#define Register_h

#include "Arduino.h"
#include "SoftwareSerial.h"

class Register
{
    public:
	    Register();
	        
        void begin(int address, int max, int tx, int rx);
        
        int max_value;
        int min_value;
                
        void on();
        void off();
        void tremolo(int pace);
        void set(int value);
        int get();
        int map(int value, int istart, int istop, int ostart, int ostop);
        
    private:
        int _address;
        int _value;
        int _rxPin;
        int _txPin;
        long _previous_millis; // a variable to be used in time-based functionality
};

#endif