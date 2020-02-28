//by McGreg 2020
//this is a replacement for Boiing
//by McGreg 2020


#include "SugarCube.h"
    
HeartBeat::HeartBeat()
{
#ifdef DEBUG
  Serial.println("Welcome to HeartBeat");
#endif
  for (i=0; i<16; i++)
  {
    pixel[i].activated = false;
    pixel[i].tempo = 125;

  }
  currentLED=16;//out of range
  heartBeatCounter=0;

}

      
void HeartBeat::buttonPressed(byte xPos, byte yPos)
{
  if (pixel[yPos*4+xPos].activated)//deactivate
  {
    //turnOffLED(xPos,yPos);
    pixel[yPos*4+xPos].activated=false;
    if (i==currentLED)
      currentLED=16;//no currentLED
  }
  else
  {
    pixel[yPos*4+xPos].buttonPressed=true;
    pixel[yPos*4+xPos].activated=true;
    //tempo will be set when released
    pixel[yPos*4+xPos].tempo=0;
    currentLED=yPos*4+xPos;
    pixel[yPos*4+xPos].channel=currentMidiChannel;
  }
  
}
      
void HeartBeat::buttonReleased(byte xPos, byte yPos)
{
  byte tempos[]={25,50,75,100,125,150,175,200};
  pixel[yPos*4+xPos].buttonPressed=false;
  //based on pressTime adjust Tempo
  byte calTemp=constrain(pixel[yPos*4+xPos].tempo/25, 0, 7);
  pixel[yPos*4+xPos].tempo=calTemp;
  noteOff(pixel[yPos*4+xPos].note, pixel[yPos*4+xPos].channel);
  turnOffLED(xPos,yPos);
  
  
#ifdef DEBUG        
  Serial.print((String)"Button tempo: "+pixel[yPos*4+xPos].tempo);
#endif      
}

void HeartBeat::pot1HasChanged(int val)
{
  _baseNote = calculateBaseNoteFromPotVal(getPot1Val());
}

void HeartBeat::pot2HasChanged(int val)
{
  //byte tempos[]={25,50,75,100,125,150,175,200};
  if (currentLED<16)
  {
  //pixel[currentLED].tempo = constrain(map(val, 0, 1024, 0,8), 0, 8);
  //Serial.println((String)"Change tempo to: "+ button[currentLED].tempo);
  }
}
void HeartBeat::instrumentHasChanged(byte val)
{
  if (pixel[currentLED].note)
    noteOff(pixel[currentLED].note, pixel[currentLED].channel);
  pixel[currentLED].channel=val;


}

void HeartBeat::updatePixel(byte pos)
{
  if (pixel[pos].activated)
  {
        //Serial.println(currentTempo%(column[colNbr].tempo/2));
    if (pixel[pos].buttonPressed) //count the ticks as tempo
    {
      pixel[pos].tempo++;
      noteOn(pixel[pos].note, velocity, pixel[pos].channel);
      turnOnLED(pos);
    }
    //check if tempo elapsed
    else if (currentTempo%pixel[pos].tempo==0)//to keep all columns in sync
    {
      //Serial.println((String)"Current Tempo: "+currentTempo);
      //column[colNbr].tempo = 0;
      pixel[pos].note=createMIDINoteInFourths(pos&B011, pos>>2, _baseNote);
      noteOn(pixel[pos].note, velocity, pixel[pos].channel);
      turnOnLED(pos);
    }
    else if (currentTempo%(pixel[pos].tempo/2)==0) //half tempo to switch off
    {
      //Serial.println((String)"Current HalfTempo: "+currentTempo);
      if (pixel[pos].note)
      {
        noteOff(pixel[pos].note, pixel[pos].channel);
        pixel[pos].note=0;
        turnOffLED(pos);
      }
     }
  }//if activated
  if (currentTempo==21000)//kgV between tempos
    currentTempo=0;

}

void HeartBeat::routine100Hz()
{
  if (++currentTempo==21000)//kgV between tempos
      currentTempo=0;
  
  for (byte i = 0; i < 16; i++)
    updatePixel(i);

}
      
