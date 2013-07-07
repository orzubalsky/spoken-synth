#include <Register.h>
#include <Envelope.h>
#include <Osc.h>
#include <Synth.h>

// the address pins will go in order from the first one:
#define firstAddressPin 6 // 6,7,8,9

int sensorValue[16];  // an array to store the sensor values
int analogInput = A0;

Synth synth;

// pins OR multiplex array index
int input_osc_01_light = 1; // sensorValue[i]
int input_osc_02_light = 2; // sensorValue[i]
int input_osc_03_light = 3; // sensorValue[i]
int input_osc_04_light = 9; // sensorValue[i]
int input_osc_05_light = 8; // sensorValue[i]

int input_osc_01_freq = 10; // sensorValue[i]
int input_osc_02_freq = 11; // sensorValue[i]
int input_osc_03_freq = 4; // sensorValue[i]
int input_osc_04_freq = 5; // sensorValue[i]
int input_osc_05_freq = 6; // sensorValue[i]

int input_osc_01_volume = 7; // sensorValue[i]
int input_osc_02_volume = 12; // sensorValue[i]
int input_osc_03_volume = 13; // sensorValue[i]
int input_osc_04_volume = 14; // sensorValue[i]
int input_osc_05_volume = 15; // sensorValue[i]

int input_volume = A4;

int input_noise = A2;
int input_noise_state = 4;

int input_tremeolo = A3;
int input_tremolo_state = 5;

int input_light_state = sensorValue[0];
int input_midi_state = 10;

int input_envelope = A1;
int input_envelope_a = 11;
int input_envelope_b = 12;
int input_envelope_wave = A5;

void setup() 
{
  // start serial for debugging
  Serial.begin(9600);
 
  // set the output pins:
  for (int pinNumber = firstAddressPin; pinNumber < firstAddressPin + 4; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
  }

  // set the input pinks:
  pinMode(input_envelope_a, INPUT);
  pinMode(input_envelope_b, INPUT);
  pinMode(input_noise_state, INPUT);
  pinMode(input_tremolo_state, INPUT);
  pinMode(input_midi_state, INPUT);
 
  synth.begin(3,2);  
}

void loop() 
{
  // read and store analog inputs from the multiplexer
  readMultiplexers();  
    
  // update wave type
  update_wave();
  
  // update boolean states
  update_states();  
    
  // set osc frequencies
  update_frequencies();

  // set noise volume
  update_noise();
  
  // tremolo
  update_tremolo();
  
  // set oscillator and master volumes
  update_volumes();  
  
  //debug();
  
  //delay(500);
}


void update_states()
{
  synth.is_midi = (digitalRead(input_midi_state)) ? true : false;
  synth.is_tremolo = (digitalRead(input_tremolo_state)) ? true : false;
  synth.is_noise = (digitalRead(input_noise_state)) ? true : false;  
  synth.is_light_control = (read_analog_switch(sensorValue[input_light_state])) ? true : false;    
  synth.envelope.a_is_on = (digitalRead(input_envelope_a)) ? true : false;
  synth.envelope.b_is_on = (digitalRead(input_envelope_b)) ? true : false;  
  
  if (synth.envelope.a_is_on)
  {
    synth.envelope.group_a_on();  
  }
  else 
  {
    synth.envelope.group_a_off();
  }
  
  if (synth.envelope.b_is_on)
  {
    synth.envelope.group_b_on();  
  }
  else 
  {
    synth.envelope.group_b_off();
  }
  
  // debug_states();
}


void update_tremolo()
{
  if (synth.is_tremolo)
  {
    int pace = map(analogRead(input_tremeolo), 1023, 0, 0, 3000);
    synth.volume.tremolo(pace);
  }
  else
  {
    synth.volume.set(synth.volume.get());
  }
}


void update_wave()
{
  // SQ   : 1023
  // SAW  : 814
  // TRI  : 434
  // SINE : 256
  
  int wave = analogRead(input_envelope_wave);
  
  
  if (wave > 0 && wave <= 300)
  {
    synth.envelope.set_sine();
  }
  if (wave > 300 && wave <= 600)
  {
    synth.envelope.set_tri();
  }
  if (wave > 600 && wave <= 900)
  {
    synth.envelope.set_saw();
  }
  if (wave > 900 && wave <= 1023)
  {
    synth.envelope.set_sq();
  }

  Serial.println(synth.envelope.control.get()); 
   
}


void update_noise()
{
  if (synth.is_noise)
  {
    synth.noise.set(map(analogRead(input_noise), 1023, 0, 0, synth.noise.max_value));    
  }
  else
  {
    synth.noise.set(synth.noise.min_value);  
  }
}


void update_volumes()
{
  // set master volume
  synth.volume.set(map(analogRead(input_volume), 1023, 0, 0, synth.volume.max_value));

  // oscillators volumes  
  synth.osc_01.volume.set(map(sensorValue[input_osc_01_volume], 1023, 0, 0, synth.osc_01.volume.max_value));
  synth.osc_02.volume.set(map(sensorValue[input_osc_02_volume], 1023, 0, 0, synth.osc_02.volume.max_value));
  synth.osc_03.volume.set(map(sensorValue[input_osc_03_volume], 1023, 0, 0, synth.osc_03.volume.max_value));
  synth.osc_04.volume.set(map(sensorValue[input_osc_04_volume], 1023, 0, 0, synth.osc_04.volume.max_value));
  synth.osc_05.volume.set(map(sensorValue[input_osc_05_volume], 1023, 0, 0, synth.osc_05.volume.max_value)); 
}


void update_frequencies()
{
  int input_01;
  int input_02;
  int input_03;
  int input_04;
  int input_05;
  
  int freq_01_value;
  int freq_02_value;
  int freq_03_value;
  int freq_04_value;
  int freq_05_value;
    
  if (synth.is_light_control)
  {
    input_01 = sensorValue[input_osc_01_light];
    input_02 = sensorValue[input_osc_02_light];
    input_03 = sensorValue[input_osc_03_light];
    input_04 = sensorValue[input_osc_04_light];
    input_05 = sensorValue[input_osc_05_light];
  }
  else 
  {
    input_01 = sensorValue[input_osc_01_freq];
    input_02 = sensorValue[input_osc_02_freq];
    input_03 = sensorValue[input_osc_03_freq];
    input_04 = sensorValue[input_osc_04_freq];
    input_05 = sensorValue[input_osc_05_freq];  
  }
  
  if (synth.is_midi)
  {
    // this frequency should be parsed from the received midi note    
    int midi_frequency = 440;
    midi_frequency = map(analogRead(input_envelope), 1023, 0, 0, synth.envelope.frequency.max_value);
      
    freq_01_value = map(input_01, 1023, 0, midi_frequency*0.8, midi_frequency*1.2);
    freq_02_value = map(input_02, 1023, 0, midi_frequency*0.8, midi_frequency*1.2);
    freq_03_value = map(input_03, 1023, 0, midi_frequency*0.8, midi_frequency*1.2);
    freq_04_value = map(input_04, 1023, 0, midi_frequency*0.8, midi_frequency*1.2);
    freq_05_value = map(input_05, 1023, 0, midi_frequency*0.8, midi_frequency*1.2);  
  }
  else
  {
    synth.envelope.frequency.set(map(analogRead(input_envelope), 1023, 0, 0, synth.envelope.frequency.max_value));  
    
    freq_01_value = map(input_01, 1023, 0, 0, synth.osc_01.frequency.max_value);
    freq_02_value = map(input_02, 1023, 0, 0, synth.osc_02.frequency.max_value);
    freq_03_value = map(input_03, 1023, 0, 0, synth.osc_03.frequency.max_value);
    freq_04_value = map(input_04, 1023, 0, 0, synth.osc_04.frequency.max_value);
    freq_05_value = map(input_05, 1023, 0, 0, synth.osc_05.frequency.max_value);
  }
  
  // debug_frequencies(freq_01_value, freq_02_value, freq_03_value, freq_04_value, freq_05_value);
  
  synth.osc_01.frequency.set(freq_01_value);
  synth.osc_02.frequency.set(freq_02_value);
  synth.osc_03.frequency.set(freq_03_value);
  synth.osc_04.frequency.set(freq_04_value);
  synth.osc_05.frequency.set(freq_05_value);   
}


boolean read_analog_switch(int pin)
{ 
  return (pin == 1023) ? true : false;
}


void readMultiplexers()
{
  // iterate once for every multiplexer (called muxes for short):
  for (int mux = 0; mux < 1; mux++) {
    for (int channelNum = 0; channelNum < 16; channelNum ++) {
      // determine the four address pin values from the channelNum:
      setChannel(channelNum);

      // read the analog input and store it in the value array: 
      sensorValue[channelNum] = analogRead(analogInput+mux);
    }
  }
}

void setChannel(int whichChannel) {
  for (int bitPosition = 0; bitPosition < 4; bitPosition++) {
    // shift value x bits to the right, and mask all but bit 0:
    int bitValue = (whichChannel >> bitPosition) & 1;
    // set the address pins:
    int pinNumber = firstAddressPin + bitPosition;
    digitalWrite(pinNumber, bitValue);
  }
}

void debug()
{
  debug_switches();
  debug_analogInputs();
  debug_multiplexer();
}

void debug_switches()
{
  for(int i=0; i<14; i++)
  {

      Serial.print(i);
      Serial.print(":");
      Serial.print(digitalRead(i), DEC);
      Serial.print("\t");
  }       
  Serial.print("\n");
  Serial.print("\n"); 
}

void debug_analogInputs()
{
  for(int i=1; i<6; i++)
  {

      Serial.print(i);
      Serial.print(":");
      Serial.print(analogRead(i), DEC);
      Serial.print("\t");
  }       
  Serial.print("\n");
  Serial.print("\n");
}

void debug_multiplexer()
{
    for (int channelNum = 0; channelNum < 16; channelNum ++) 
    {
      // print the values as a single tab-separated line:
      Serial.print(channelNum);
      Serial.print(":");
      Serial.print(sensorValue[channelNum], DEC);
      Serial.print("\t");
    }
    
    Serial.println("");
    Serial.println("");
    Serial.println("");    
}


void debug_frequencies(int freq_01_value, int freq_02_value, int freq_03_value, int freq_04_value, int freq_05_value)
{
  Serial.print("\n");    
  Serial.print("freq_01_value");
  Serial.print(":");
  Serial.print(freq_01_value);
  Serial.print("\t\t");  
  Serial.print("freq_02_value");
  Serial.print(":");
  Serial.print(freq_02_value);
  Serial.print("\t\t");  
  Serial.print("freq_03_value");
  Serial.print(":");
  Serial.print(freq_03_value);
  Serial.print("\t\t");  
  Serial.print("freq_04_value");
  Serial.print(":");
  Serial.print(freq_04_value);
  Serial.print("\t\t");  
  Serial.print("freq_05_value");
  Serial.print(":");
  Serial.print(freq_05_value);
  Serial.print("\n");
}

void debug_states()
{
  Serial.print("\n");    
  Serial.print("is_midi");
  Serial.print(":");
  Serial.print(digitalRead(input_midi_state));
  Serial.print("\t\t");  
  Serial.print("is_noise");
  Serial.print(":");
  Serial.print(digitalRead(input_noise_state));
  Serial.print("\t\t");    
  Serial.print("is_tremolo");
  Serial.print(":");
  Serial.print(digitalRead(input_tremolo_state));
  Serial.print("\t\t");  
  Serial.print("is_light_control");
  Serial.print(":");
  Serial.print(read_analog_switch(input_light_state));
  Serial.print("\t\t");  
  Serial.print("a_is_on");
  Serial.print(":");
  Serial.print(digitalRead(input_envelope_a));
  Serial.print("\t\t");  
  Serial.print("b_is_on");
  Serial.print(":");
  Serial.print(digitalRead(input_envelope_b));
  Serial.print("\n");
}
