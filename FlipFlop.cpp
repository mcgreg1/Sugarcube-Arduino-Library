//by McGreg 2020
//this is a replacement for Boiing


#include "SugarCube.h"

FlipFlop::FlipFlop()
{
#ifdef DEBUG
  Serial.println("Welcome to FlipFlop");
#endif
  this->clearAllStorage();

  xOffset = getOffsetFromPotVal(getPot1Val());
  byte t_notes[] = {55, 57, 60, 62, 65, 67, 69, 72, 74, 77, 79, 81, 84, 86, 89, 91};
  for (byte i = 0; i < 16; i++) {
    notes[i] = t_notes[i];
  }
}

void FlipFlop::pot1HasChanged(int val)
{
  xOffset = getOffsetFromPotVal(getPot1Val());
#ifdef DEBUG
  Serial.print("xOffset: ");
  Serial.println(xOffset);
#endif

}

void FlipFlop::pot2HasChanged(int val)
{
  //currentTempo=maxTempoFromPotVal(int val);
          currentTempo = constrain(map(val, 0, 1024, 0,8), 0, 8);
          #ifdef DEBUG
        Serial.println((String)"Change tempo to: "+ currentTempo);
        #endif
}
void FlipFlop::instrumentHasChanged(byte val)
{
  //switch off current colummn

}
void FlipFlop::wasShaken()
{
  this->clearAllStorage();
  clearLEDs();
}
void FlipFlop::buttonPressed(byte xPos, byte yPos)
{
  byte absPosition = xPos+xOffset;

  if (column[absPosition].activated)//switch off
  {
    turnOffLED(column[absPosition].currentHeight, yPos);
    if (column[absPosition].playedTone) //switch of playing tone
    {
      noteOff(column[absPosition].playedTone, column[absPosition].channel);
    }
    column[absPosition].activated = false;
  }
  else //switch on
  {
    turnOnLED(xPos, yPos);


    column[absPosition].activated = true;
    column[absPosition].goingUp = false;
    column[absPosition].height = column[absPosition].currentHeight = 1 << (3 - yPos);
    column[absPosition].tempo = 100; //1 second tempo
    column[absPosition].channel = currentMidiChannel;
    column[absPosition].playedTone = 0;

#ifdef DEBUG
    Serial.println((String)"Button " + xPos + "/" + yPos + " absolutePosition: " + absPosition + " Height: " + (1 << (3 - yPos)));
#endif
  }
}

void FlipFlop::updateColumn(byte colNbr)
{
  if (column[colNbr].activated)
  {
        //Serial.println(currentTempo%(column[colNbr].tempo/2));
        
    //check if tempo elapsed
    if (currentTempo%column[colNbr].tempo==0)//to keep all columns in sync
    //if (column[colNbr].tempo == currentTempo)
    {
      //Serial.println((String)"Current Tempo: "+currentTempo);
      //column[colNbr].tempo = 0;
      if (column[colNbr].goingUp)
      { //we are at top, play tone and go down
        if (column[colNbr].currentHeight == column[colNbr].height)
        {
          column[colNbr].goingUp = false;
          column[colNbr].playedTone = notes[colNbr] + 1;
          noteOn(column[colNbr].playedTone, velocity, column[colNbr].channel);
  
        }
        else//we are on the way up
        {
          column[colNbr].currentHeight++;
          #ifdef DEBUG
          Serial.print("We are going UP ");
          Serial.println(column[colNbr].currentHeight);
          #endif
        }
  
      }
      else//going down
      { //we are at bottom, play tone and go up

        if (column[colNbr].currentHeight == 0)
        {
          if (column[colNbr].height) //no direction change if height==0
          {
            column[colNbr].goingUp = true;
          }
          column[colNbr].playedTone = notes[colNbr];
          noteOn(column[colNbr].playedTone, velocity, column[colNbr].channel);
        }
        else//we are on the way down
        {
          column[colNbr].currentHeight--;
          #ifdef DEBUG
          Serial.print("We are going DOWN ");
          Serial.println(column[colNbr].currentHeight);
          #endif
        }
      }
    }
    else if (currentTempo%(column[colNbr].tempo/2)==0) //half tempo to switch off
    {
      //Serial.println((String)"Current HalfTempo: "+currentTempo);
      if (column[colNbr].playedTone)
      {
        noteOff(column[colNbr].playedTone, column[colNbr].channel);
        column[colNbr].playedTone = 0;
      }
  
    }
  }//if activated

}

void FlipFlop::updateLED(byte colNbr)
{
byte colLED[]={0,1,2,4,8};
  if (column[colNbr+xOffset].activated)
    setLEDCol(colNbr, colLED[column[colNbr + xOffset].currentHeight+1]);
}

void FlipFlop::routine100Hz()
{

  if (++currentTempo==21000)//kgV between tempos
      currentTempo=0;
  for (byte i = 0; i < 16; i++)
  {
    updateColumn(i);
  }
  for (byte i = 0; i < 4; i++)
  {
    updateLED(i);
  }
}

/*
      byte FlipFlop::maxTempoFromPotVal(int val)//10 bit val
      {
      byte tempos[]={25,50,75,100,125,150,175,200};
      return tempos[constrain(map(val, 0, 1023, 0, 8),0,8)];
        
        return ((val>>6) + 1)*5;
      }
*/

void FlipFlop::clearAllStorage()
{
  for (byte i = 0; i < 16; i++)
  {
    if (column[i].playedTone)
    {
      noteOff(column[i].playedTone, column[i].channel);
      column[i].playedTone = 0;
    }
    column[i].activated = false;
    column[i].goingUp = false;
    column[i].height = column[i].currentHeight = 0;
    column[i].tempo = 99; //1 second tempo
    column[i].channel = 0;

  }
  clearLEDs();
}

byte FlipFlop::getOffsetFromPotVal(byte pos)
{
  return constrain(map(pos, 0, 1023, 0, 12), 0, 12);//0-12 represent 0-15 columns
  //return (pos+_xOffset)&15;
}
