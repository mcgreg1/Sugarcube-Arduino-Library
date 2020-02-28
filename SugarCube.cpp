//by Amanda Ghassaei 2012, modifications/adaptions by McGreg 2020


#include "SugarCube.h"

//Rotary encoder values
byte instrument;
bool instrumentButton, instrumentLast;
unsigned int currentRotaryTime=0;
unsigned int rotaryButtonTime=0;

//Analog pot values
int pot1, pot2, vol, volRaw;
byte i=0;
byte j=0;
byte k=0;

byte analogTolerance=10;


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
unsigned int counter=0;
Delegate *activeMode = NULL;



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
  #ifdef DEBUG
  Serial.println((String)"Volume: "+val);
  #endif
  //activeMode->volHasChanged(val);
  //TODO: change the MIDI volume
  //change velocity instead of volume
  velocity=val;
  /*
  for (i=0; i<16; i++)
    midiSetChannelVolume(i, val);
    */
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
    allNotesOff(i);

    
  VS1053_MIDI.write(MIDI_CHAN_MSG);
  VS1053_MIDI.write(MIDI_RESET_ALL_CONTROLLERS);
  //wait for input


  //measure the presstime
  rotaryButtonTime=millis();
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
  if (abs(newVal-oldVal)>analogTolerance){
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
 
    //check Volume

    newVal=analogValFromPin(volPin, vol);
    if (volRaw != newVal)
    {
      byte scaledNewVal = constrain(map(newVal, 0, 1023, VELOCITY_MIN, 127), VELOCITY_MIN, 127);
      byte scaledOldVal = constrain(map(volRaw, 0, 1023, VELOCITY_MIN, 127), VELOCITY_MIN, 127);
      if (abs(scaledOldVal-scaledNewVal)>1) //somehow my volume flips between two values
      {
        vol=scaledNewVal;
        volHasChanged(scaledNewVal);
      }
      volRaw=newVal;
    }


}
//the state is delayed by ROTARY_IDLE_TIME, so we don't send MIDI commands while it's rotating
void checkRotaryEncoder()
{
      //check Instrument
    bool n = digitalRead(instLPin);
    if ((instrumentLast == LOW) && (n == HIGH)) 
    {
      if (digitalRead(instRPin) == LOW) 
      {
        if (instrument<=0)
          instrument=INSTRUMENT_MAX;
        else
          instrument--;
  
      } 
      else 
      {
        if (instrument>=INSTRUMENT_MAX)
          instrument=0;
          else
        instrument++;
     
      }
      //Serial.print (encoder0Pos);
      //Serial.print ("/");

      //wait if still rotating
      
      currentRotaryTime=millis();
      
    }
    instrumentLast=n;
    unsigned int diff = millis()-currentRotaryTime;
    if (diff>ROTARY_IDLE_TIME && diff<2*ROTARY_IDLE_TIME)
    {
      instrumentHasChanged(instrument);
      currentRotaryTime=0;
    }
    

    //instrumentButton = digitalRead(instButtonPin);
    if (!digitalRead(instButtonPin))
    {
      instrumentButtonHasChanged();
    }
    else//if BUTTON_RESET_TIME reached, reset the Atmega32u4
    {
      #ifdef DEBUG
      Serial.println((String)"Presstime: "+rotaryButtonTime);
      #endif
      if (rotaryButtonTime>BUTTON_RESET_TIME)
      {
        rotaryButtonTime=0;
        softwareReset();
      }
    }
}

void softwareReset()
{
  /* microcontroller forum 
  cli();            // disable interrupts 
wdt_enable(WDTO_15MS);  // reset via watchdog 
_delay_ms(100);    // wait until reset occurs


*/
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
