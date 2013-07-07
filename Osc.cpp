/*
    Osc.h - Library for controlling a speakjet oscillator
    Released into the public domain.
*/

#include "Arduino.h"
#include "Osc.h"

/*! \brief Default constructor for Osc. */
Osc::Osc()
{
    
}


void Osc::begin(int frequency_register, int volume_register, int tx, int rx)
{
    frequency.begin(frequency_register, 3999, tx, rx);
    volume.begin(volume_register, 31, tx, rx);
}