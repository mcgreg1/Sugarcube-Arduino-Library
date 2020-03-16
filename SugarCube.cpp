//by Amanda Ghassaei 2012, modifications/adaptions by McGreg 2020


#include "SugarCube.h"


//Rotary encoder values
byte currentInstrument;
bool instrumentButton, instrumentLast;

int hiddenFunctions=0;
//Analog pot values
int pot1, pot2, vol, volRaw;
byte i=0;
byte j=0;
byte k=0;

byte analogTolerance=15;


//storage for led states, 4 bytes
byte ledData[] = {0, 0, 0, 0};
//storage for buttons, 4 bytes
byte buttonCurrent[] = {0,0,0,0};
byte buttonLast[] = {0,0,0,0};
byte buttonEvent[] = {0,0,0,0};
byte buttonState[] = {0,0,0,0};

//button debounce counter- 16 bytes
byte buttonDebounceCounter[4][4];

//MIDI variables
int velocity = 127;

byte instrumentPerChannel[16];

byte hardwareIter=0;
unsigned int timer100Hz=0, timer1Hz=0;
byte currentMidiChannel=0;

byte selectSig=0;
bool selectSigForward=true;
//unsigned long int counter=0;
Delegate *activeMode = NULL;
unsigned long idleCounter=0;



//Functions which are used by the different modes

void pot1HasChanged(int val)
{
  #ifdef DEBUG
  Serial.println((String)"pot1: "+val);
  #endif
  if (activeMode != NULL)
  {
    activeMode->pot1HasChanged(val);
  }
}
void pot2HasChanged(int val)
{
  #ifdef DEBUG
  Serial.println((String)"pot2: "+val);
  #endif
  if (activeMode != NULL)
  {
    activeMode->pot2HasChanged(val);
  }
}

void volHasChanged(int val)
{
  velocity= (val>>4)*2;
  //velocity=constrain(map(val, 0,1023, 25, 127), 25, 127);

  #ifdef DEBUG
  Serial.println((String)"Velocity: "+velocity);
  #endif

}
void instrumentHasChanged(byte val)
{
  setInstrument(val);
  #ifdef DEBUG
  Serial.println((String)"Instrument: "+val+" midi Channel: "+currentMidiChannel);
  #endif

  if (activeMode != NULL)
  {
    activeMode->instrumentHasChanged(val); 
  }
}
void instrumentButtonHasChanged()
{
  #ifdef DEBUG
  Serial.println("Instrument button pressed!");
  #endif
  delete activeMode;
  activeMode = NULL;
  clearLEDs();
  for (i=0; i<16; i++)
  {
    allNotesOff(i);
    instrumentPerChannel[i]=0;
  }
  //for (i=0; i<16; i++)
    //allNotesOff(i);

    
  //VS1053_MIDI.write(MIDI_CHAN_MSG);
  //VS1053_MIDI.write(MIDI_RESET_ALL_CONTROLLERS);
  //wait for input
}

int getPot1Val()
{//returns value of potentiomenter #1
  return pot1;
}
int getPot2Val()
{//returns value of potentiomenter #1
  return pot2;
}

int analogValFromPin(byte pinNum, int oldVal)
{

  int newVal = analogRead(pinNum);
  //8 values can be off means 2^3
  if ((abs(newVal-oldVal))>>3)
  {
    return newVal;
  }
  return oldVal;
}
  
void checkAnalogPins()
{
  int newVal;
  //check Pot1
    newVal=analogValFromPin(pot1Pin, pot1);
    if (pot1 != newVal)
    {
      pot1 = newVal;
      pot1HasChanged(newVal);
    }

    //check Pot2
    newVal=analogValFromPin(pot2Pin, pot2);
    if (pot2 != newVal)
    {
      pot2 = newVal;
      pot2HasChanged(newVal);
    }
 
    //check Volume: it's not stable somehow
    newVal = analogRead(volPin);
    //16 values can be off means 2^4
    if ((abs(newVal-volRaw))>>4)
    {
      volRaw=newVal;
      volHasChanged(volRaw);
    }
/*
    newVal=analogValFromPin(volPin, volRaw);
    if (volRaw != newVal)
    {
      volRaw=newVal;
      volHasChanged(volRaw);

    }
*/

}
//the state is delayed by ROTARY_IDLE_TIME, so we don't send MIDI commands while it's rotating
void checkRotaryEncoder()
{

      //check Instrument
    bool n = digitalReadFast(instLPin);
    if ((instrumentLast == LOW) && (n == HIGH)) 
    {
      if (digitalReadFast(instRPin) == LOW) 
      {
        if (currentInstrument<=1)             //possible values 1-INSTRUMENT_MAX
          currentInstrument=INSTRUMENT_MAX;
        else
          currentInstrument--;
  
      } 
      else 
      {
        if (currentInstrument>INSTRUMENT_MAX) //possible values 1-INSTRUMENT_MAX
          currentInstrument=1;
          else
        currentInstrument++;
     
      }
      #ifdef DEBUG
      Serial.println((String)"Instrument list entry: "+(currentInstrument));
      #endif
      instrumentHasChanged(instrumentList[currentInstrument-1]);
      //instrumentHasChanged(currentInstrument);
    }
    instrumentLast=n;
    if (!digitalReadFast(instButtonPin))
    {
   
      instrumentButtonHasChanged();
    }

}
/*
void softwareReset()
{
  #ifdef DEBUG
  Serial.println("+++++++++++++++Going to reset the board!!!+++++++++++++++");
  #endif
  //Microchip.com website
    do                          
    {                           
        wdt_enable(WDTO_15MS);  
        for(;;)                 
        {                       
        }                       
    } while(0);
}
*/
