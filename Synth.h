/*
    Synth.h - Library for controlling a speakjet internal registers
    Released into the public domain.
*/

#ifndef Synth_h
#define Synth_h

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Register.h"
#include "Osc.h"
#include "Envelope.h"

// register addresses
#define REGISTER_ENV_FREQUENCY 0
#define REGISTER_OSC_01_FREQUENCY 1
#define REGISTER_OSC_02_FREQUENCY 2
#define REGISTER_OSC_03_FREQUENCY 3
#define REGISTER_OSC_04_FREQUENCY 4
#define REGISTER_OSC_05_FREQUENCY 5
#define REGISTER_NOISE 6
#define REGISTER_MASTER_VOLUME 7
#define REGISTER_ENV_CONTROL 8
#define REGISTER_OSC_01_VOLUME 11
#define REGISTER_OSC_02_VOLUME 12
#define REGISTER_OSC_03_VOLUME 13
#define REGISTER_OSC_04_VOLUME 14
#define REGISTER_OSC_05_VOLUME 15


class Synth
{
    public:
	    Synth();
	        
        void begin(int txPin, int rxPin);
        
        Osc osc_01; 
        Osc osc_02;
        Osc osc_03;
        Osc osc_04;
        Osc osc_05;

        Register noise;
        Register volume;
        
        Envelope envelope;

        boolean is_midi;
        boolean is_noise;
        boolean is_tremolo;
        boolean is_light_control;        
    
    private:
};

#endif