/*
    Register.h - representing a speakjet register
    Released into the public domain.
*/

#include "Arduino.h"
#include "Register.h"

/*! \brief Default constructor for Register. */
Register::Register()
{
    
}


void Register::begin(int address, int max, int tx, int rx)
{
    max_value = max;
    min_value = 0;
    
    _address = address;
    _rxPin = rx;
    _txPin = tx;
}

void Register::tremolo(int pace)
{
    off();

    // store current time
    unsigned long current_millis = millis();

    if(current_millis - _previous_millis > pace) 
    {
        on();

        // save millis for next interval
        _previous_millis = current_millis;
    }
}

void Register::on()
{
    set(_value);
}

void Register::off()
{
    set(0);
}

int Register::get()
{
    return _value;
}
void Register::set(int value)
{
    if (value != _value)
    {
        SoftwareSerial speakjet = SoftwareSerial(_rxPin, _txPin); 
        speakjet.begin(9600);
            
        value = (value <= max_value) ? value : max_value;
        value = (value >= min_value) ? value : min_value;
    
        speakjet.print("\\0");  
        speakjet.print(_address);    
        speakjet.print("J");  
        speakjet.print(value);  
        speakjet.print("N");    
        speakjet.print("X");
    
        _value = value;    
    }
}

int Register::map(int value, int istart, int istop, int ostart, int ostop)
{
    int result = ostart + (ostop - ostart) * ((value - istart) / (istop - istart));

    result = (result > ostop) ? ostop : result;
    result = (result < ostart) ? ostart : result;

    return result;    
}