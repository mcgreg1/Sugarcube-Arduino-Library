//by McGreg 2020


#include "SugarCube.h"
#include "Delegate.h"

//modes: userMode, ReplayMode    
Memory::Memory()
{
  #ifdef DEBUG
        Serial.println("Welcome to Memory");
#endif
noteOn(67, 120,currentMidiChannel);
  wrong();
  baseNote = calculateBaseNoteFromPotVal(getPot1Val());
  tempo=5;
  metronom=0;
  counter=0;
  marioMode=false;
  currentTempo=60;
  noteOff(67, currentMidiChannel);
  randomSeed(analogRead(A3)|millis());
}

//super mario: 1/16 sec speed: 0.12, 0.17, 0.25: 1/8, 3/16, 1/4

void Memory::buttonPressed(byte xPos, byte yPos)
{
  if (!playbackMode)
  {  
    turnOnLED(toneMemory[currentTone]);
    currentLED=toneMemory[currentTone];
    noteOn(createMIDINoteInFourths(xPos, yPos, baseNote), velocity,currentMidiChannel);
    if (4*yPos+xPos==toneMemory[currentTone])
    {
      if (currentTone == toneLen-1)//we did all correct
      {
        correct();
        #ifdef DEBUG
        Serial.println("All correct!");
        #endif
      }
      else//check for the next tone
      {
        #ifdef DEBUG
        Serial.print(currentTone);
        Serial.println(" tone was correct!");
        #endif
        currentTone++;

      }
       
    }
    else// wrong
    {
      #ifdef DEBUG
      Serial.println("Wrong!");
      #endif
      wrong();
    }
  }
}
void Memory::pot1HasChanged(int val)
{
  baseNote = calculateBaseNoteFromPotVal(val);
}

void Memory::buttonReleased(byte xPos, byte yPos)
{
        turnOffLED(currentLED);
  noteOff(createMIDINoteInFourths(xPos, yPos, baseNote),currentMidiChannel);

}
//TODO: pot2 changes the speed
void Memory::pot2HasChanged(int val)
{
  currentTempo=constrain(map(val, 0, 1024, 100,20), 40, 160);
  #ifdef DEBUG
  Serial.println((String)"Current delay set to "+currentTempo);
  #endif

}

void Memory::routine100Hz()
{

  if (playbackMode)
  {
      if (marioMode)
      {
        playGianna(counter++);
      }
      else 
      {
        if (++tempo>currentTempo)
        {
          tempo =0; //1 second tempo
          //metronom++;
          playSequenceFromPosition(metronom++, toneMemory, toneLen, 60);
        }
        
        if (metronom>toneLen*2)
        {
            metronom=0;
            playbackMode=false;
        }
      }
    
  }
 
}


byte Memory::getRandomButton()
{
  //randomSeed(analogRead(A3)|millis());
  return random(0,16);
}

void Memory::correct() //play when memory done correctly
{
  
  if (toneLen == 10)
  {
    marioMode=true;
    playbackMode=true;
    toneLen=3;
    counter==0;
  }
  else
  {
    toneLen++;
    for (i=0; i<toneLen; i++)
      toneMemory[i]=getRandomButton();
    currentTone=0;
  }
  metronom=0;
  playbackMode=true;
  
}
void Memory::wrong() //play when it was wrong
{
  
  toneLen=3;
  for (i=0; i<toneLen; i++)
  {
      toneMemory[i]=getRandomButton();
      Serial.print(toneMemory[i]);
      Serial.print(".");
      
  }
  Serial.println("..Memory Created!");
  
  currentTone=0;
  metronom=0;
  playbackMode=true;
  
}

  
