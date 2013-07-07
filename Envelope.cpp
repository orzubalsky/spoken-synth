/*
    Envelope.h - Library for controlling a speakjet envelope registers
    Released into the public domain.
*/

#include "Arduino.h"
#include "Envelope.h"

/*! \brief Default constructor for Envelope. */
Envelope::Envelope()
{
    
}


void Envelope::begin(int frequency_register, int control_register, int tx, int rx)
{
    frequency = Register();
    frequency.begin(frequency_register, 3999, tx, rx);
    
    control = Register();
    control.begin(control_register, 255, tx, rx);   
}

void Envelope::set_saw()
{
    _set_wave(SAW);
}
void Envelope::set_sine()
{
    _set_wave(SINE);
}
void Envelope::set_tri()
{
    _set_wave(TRI);
}
void Envelope::set_sq()
{
    _set_wave(SQ);
}

void Envelope::group_a_on()
{
    int value = _wave_value + A_DIFF;
    
    if (b_is_on)
    {
        value = value + B_DIFF;   
    }
    
    control.set(value);   
}

void Envelope::group_a_off()
{
    int value = _wave_value;

    if (b_is_on)
    {
        value = value + B_DIFF;   
    }    

    control.set(value);   
}

void Envelope::group_b_on()
{
    int value = _wave_value + B_DIFF;
    
    if (a_is_on)
    {
        value = value + A_DIFF;   
    }
        
    control.set(value);   
}

void Envelope::group_b_off()
{
    int value = _wave_value;
    
    if (a_is_on)
    {
        value = value + A_DIFF;   
    }    
    control.set(value);   
}

void Envelope::_set_wave(int value)
{
    _wave_value = value;
    
    if (a_is_on)
    {
        value = value + A_DIFF;
    }
    if (b_is_on)
    {
        value = value + B_DIFF;
    }    
    control.set(value);
}