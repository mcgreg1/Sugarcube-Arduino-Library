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
  byte tempos[] = {5,10,15,20,25,30,40,50,70,100};//kgv 4200
  //byte tempos[] = {5,10,15,25,35,50,65,80};//kgv 109200
  //byte tempos[] = {5,10,15,20,25,30,35,40,45,50,60,70,80,90,100,110};//kgv 277200 
  
  tempo = tempos[constrain(map(getPot2Val(), 0, 1024, 0,10), 0, 10)];
}

void FlipFlop::pot1HasChanged(int val)
{
  xOffset = getOffsetFromPotVal(val);
#ifdef DEBUG
  Serial.print("xOffset: ");
  Serial.println(xOffset);
#endif

}

void FlipFlop::pot2HasChanged(int val)
{
  //byte tempos[] = {10, 30, 50, 70, 90, 110, 130, 150};
  byte tempos[] = {5,10,15,20,25,30,40,50,70,100};//kgv 4200
  //byte tempos[]={25,50,75,100,125,150,175,200};
  //tempoCounter=maxTempoFromPotVal(int val);
  tempo = tempos[constrain(map(val, 0, 1024, 0,10), 0, 10)];

#ifdef DEBUG
  Serial.println((String)"Change tempo to: " + tempo);
#endif
}
void FlipFlop::instrumentHasChanged(byte val)
{
  //TODO: switch off current colummn
  

}
void FlipFlop::wasShaken()
{
  this->clearAllStorage();
  clearLEDs();
}
void FlipFlop::buttonPressed(byte xPos, byte yPos)
{
  byte absPosition = xPos + xOffset;
  Serial.println((String)"Absolute Position: " + absPosition);

  if (column[absPosition].activated)//switch off
  {
    //turnOffLED(column[absPosition].currentHeight, yPos);
    //TODO
    setLEDCol(xPos, 0);

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
    column[absPosition].height = column[absPosition].currentHeight = 3 - yPos;
    column[absPosition].channel = currentMidiChannel;
    column[absPosition].playedTone = 0;

#ifdef DEBUG
    Serial.println((String)"Button " + xPos + "/" + yPos + " absolutePosition: " + absPosition + " Height: " + (3 - yPos));
#endif
  }
}


void FlipFlop::buttonReleased(byte xPos, byte yPos)
{

}

void FlipFlop::updateColumn(byte colNbr)
{
  if (column[colNbr].activated)
  {
    //check if tempo elapsed
    if (tempoCounter % tempo == 0) //to keep all columns in sync
    {
      //Serial.println((String)"Current Tempo: "+tempoCounter);
      //column[colNbr].tempo = 0;
      if (column[colNbr].goingUp)
      { //we are at top, note off tone and go down
        if (column[colNbr].currentHeight == column[colNbr].height)
        {
          column[colNbr].goingUp = false;
          noteOff(column[colNbr].playedTone, column[colNbr].channel);
          column[colNbr].playedTone = 0;
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
    //toggle pixel, use half tempo to switch off
    else if (!column[colNbr].height && tempoCounter % (tempo / 2) == 0)
    {
      noteOff(column[colNbr].playedTone, column[colNbr].channel);
      column[colNbr].playedTone = 0;

    }
  }//if activated

}

void FlipFlop::updateLED(byte colNbr)
{
  byte colLED[] = {0, 1, 2, 4, 8};
  if (column[colNbr + xOffset].activated)
  {
    if (column[colNbr + xOffset].height)
    {
      setLEDCol(colNbr, colLED[column[colNbr + xOffset].currentHeight + 1]);
    }
    else//toggle Pixel
    {
      if (column[colNbr + xOffset].playedTone)
        turnOffLED(colNbr + xOffset, 3);
      else
        turnOnLED(colNbr + xOffset, 3);
    }
  }
  else
    setLEDCol(colNbr, 0);
}

void FlipFlop::routine100Hz()
{

  if (++tempoCounter == 4200) //kgV between tempos
    tempoCounter = 0;
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
    column[i].channel = 0;

  }
  clearLEDs();
}

byte FlipFlop::getOffsetFromPotVal(int pos)
{

  //Serial.println(pos);
  //return (pos+xOffset)>>6;
  //TODO: bit operations
  return constrain(map(pos, 0, 1023, 0, 12), 0, 12);//0-12 represent 0-15 columns
  //return (pos+xOffset)&15;
}
