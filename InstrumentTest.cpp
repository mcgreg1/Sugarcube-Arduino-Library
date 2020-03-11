//by McGreg 2020


#include "SugarCube.h"
#include "Utilities.h"

#ifdef INSTRUMENT_TEST

InstrumentTest::InstrumentTest()
{
#ifdef DEBUG
  Serial.println("Welcome to Instrument Test");
#endif
  baseNote = calculateBaseNoteFromPotVal(getPot1Val());
  pitchbendFromAnalogVal(getPot2Val(), currentMidiChannel);
  selected=false;
  tempoCounter=0;
  currentInstrument=0;
  
}
void InstrumentTest::buttonPressed(byte xPos, byte yPos)
{
  if (!selected)
  {
    instOffset=(yPos*4+xPos);
    selected=true;
    Serial.println((String)"Selected Octet: "+instOffset);
    currentInstrument=instOffset*8+1;//start counting at 1!
    setInstrument(currentInstrument);
    turnOnLED(xPos,yPos);

  }

}

void InstrumentTest::buttonReleased(byte xPos, byte yPos)
{

}

void InstrumentTest::pot1HasChanged(int val)
{
  baseNote = calculateBaseNoteFromPotVal(val);
  #ifdef DEBUG
  Serial.println((String)"baseNote: "+baseNote);
  #endif
}


void InstrumentTest::pot2HasChanged(int val)
{
  byte tempos[]={25,50,75,100,125,150,175,200};
  tempo = tempos[val>>7];//10 bit to 3 bit
  Serial.println((String)"Change tempo to: "+ tempos[tempo]);

}
void InstrumentTest::playCurrentTone()
{

    //play six tones
    if (currentTone && currentTone<=5)
    {
         noteOff(60+currentTone, currentMidiChannel); 
    }
    currentTone++;
    if (currentTone<5)
    {
        
        noteOn(60+currentTone, velocity, currentMidiChannel);
    }
    else if (currentTone==10) //switch instrument
    {
      currentInstrument++;
      if (currentInstrument%8!=0) //we did not all
      {
          Serial.print("Switch instrument to: "),
          Serial.println(currentInstrument);
          setInstrument(currentInstrument);
          currentTone=0;
      }
      else//go back to select menu
      {
        selected=false;
      }
  
    }
   
}

void InstrumentTest::routine100Hz()
{
  if (selected)
  {
    if (++tempoCounter == 21000) //kgV between tempos
      tempoCounter = 0;
    if (tempoCounter%tempo==0)
    {
     playCurrentTone(); 
    }
  }
  else
  {
    if (tempoCounter%tempo==0)
    {
      clearLEDs();
      turnOnLED(random(0,4), random(0,4));
    }
    
  }

}
#endif
