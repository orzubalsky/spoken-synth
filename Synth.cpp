/*
    Synth.cpp - Library for controlling a speakjet internal oscillators
    Released into the public domain.
*/

#include "Arduino.h"
#include "Synth.h"


/*! \brief Default constructor for Synth. */
Synth::Synth()
{
    
}


void Synth::begin(int txPin, int rxPin)
{
    // instanciate oscillators
    osc_01.begin(REGISTER_OSC_01_FREQUENCY, REGISTER_OSC_01_VOLUME, rxPin, txPin);
    osc_02.begin(REGISTER_OSC_02_FREQUENCY, REGISTER_OSC_02_VOLUME, rxPin, txPin);
    osc_03.begin(REGISTER_OSC_03_FREQUENCY, REGISTER_OSC_03_VOLUME, rxPin, txPin);
    osc_04.begin(REGISTER_OSC_04_FREQUENCY, REGISTER_OSC_04_VOLUME, rxPin, txPin);
    osc_05.begin(REGISTER_OSC_05_FREQUENCY, REGISTER_OSC_05_VOLUME, rxPin, txPin);

    // instanciate registers
    volume.begin(REGISTER_MASTER_VOLUME, 255, rxPin, txPin);
    noise.begin(REGISTER_NOISE, 255, rxPin, txPin);
    envelope.begin(REGISTER_ENV_FREQUENCY, REGISTER_ENV_CONTROL, rxPin, txPin);    
    
    is_noise = 0;
    is_midi = 0;
    is_tremolo = 0;
    is_light_control = 0;    
}