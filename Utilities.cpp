//by Amanda Ghassaei 2012, modifications/adaptions by McGreg 2020


#include "Utilities.h"
#include "SugarCube.h"




//translate button location (x,y) to MIDI note, based on fourths: http://www.youtube.com/watch?v=uQm3xbTxJRc
byte createMIDINoteInFourths(byte xPos, byte yPos, byte baseNote)
{
  return 5*xPos+(3-yPos)+baseNote;
}

byte calculateBaseNoteFromPotVal(int val)
{
  return byte(39+(val>>5));//returns #s between 29 and 70
//return constrain(map(val, 0, 1023, 45, 70), 45, 70);
}

  
  
byte xOffsetFromPotVal(int val)
{
  return val>>6;
}

byte yCoordFromColState(byte state)
{
  for (i=0;i<4;i++) {
    if ((state>>i)&1) return 3-i;
  }
  return 4;
}


//---------------------------------------------------------------------
//--------------LOW LEVEL LED/BUTTON MONITORING------------------------
//---------------------------------------------------------------------

void shift(byte i)
{
  buttonLast[i] = buttonCurrent[i];
  byte dataToSend = (1 << (i+4)) | (15 & ~ledData[i]);
  // set latch pin low so the LEDs don't change while sending in bits
  digitalWriteFast(ledLatchPin, LOW);
  // shift out the bits of dataToSend 
  //shiftOut(ledDataPin, ledClockPin, LSBFIRST, dataToSend);  
  for (byte j=0;j<8;j++){
    digitalWriteFast(ledClockPin,LOW);
    if ((dataToSend>>j)&1){
      digitalWriteFast(ledDataPin,HIGH);
    }
    else{
      //PORTD&=B01111111;//7 to low
      digitalWriteFast(ledDataPin,LOW);
    }
    digitalWriteFast(ledClockPin,HIGH);
  }
  //set latch pin high so the LEDs will receive new data
  digitalWriteFast(ledLatchPin, HIGH);
  
  // SlowDown is put in here to waste a little time while we wait for the state of the output
  // pins to settle.  Without this time wasting loop, a single button press would show up as
  // two presses (the button and its neighbour)
  volatile int SlowDown = 0; 
  while (SlowDown < 15) 
  { 
    SlowDown++; 
  } 
    
  //once one row has been set high, receive data from buttons
  //set latch pin high
  digitalWriteFast(buttonLatchPin, HIGH);
  //shift in data
  //buttonCurrent[i] = shiftIn(buttonDataPin, buttonClockPin, LSBFIRST) >> 3;
  for (byte j=0;j<4;j++){
    digitalWriteFast(buttonClockPin,LOW);
    digitalWriteFast(buttonClockPin,HIGH);
  }
  for (byte j=0;j<4;j++){
   digitalWriteFast(buttonClockPin,LOW);
    if (digitalReadFast(buttonDataPin)){
      buttonCurrent[i]|=1<<j;
    }
    else{
      buttonCurrent[i]&=~(1<<j); 
    }
    digitalWriteFast(buttonClockPin,HIGH);
  }
  //latchpin low
  digitalWriteFast(buttonLatchPin, LOW);
  
  for (byte j=0;j<4;j++){
    buttonCheck(i,j);
    
    if (buttonEvent[i]<<j){
      boolean state = buttonState[i]&1<<j;

      if (state){
        buttonPressed(3-j,i);
      }
      else{
        buttonReleased(3-j,i);
      }
      buttonEvent[i] &= ~(1<<j);
    }
  }    
}


//---------------------------------------------------------------------
//--------------LOW LEVEL LED/BUTTON MONITORING------------------------
//---------------------------------------------------------------------


// buttonCheck - checks the state of a given button.
//this buttoncheck function is largely copied from the monome 40h firmware by brian crabtree and joe lake
void buttonCheck(byte row, byte index)
{
  if (((buttonCurrent[row] ^ buttonLast[row]) & (1 << index)) &&   // if the current physical button state is different from the
  ((buttonCurrent[row] ^ buttonState[row]) & (1 << index))) {  // last physical button state AND the current debounced state

    if (buttonCurrent[row] & (1 << index)) {                      // if the current physical button state is depressed
      buttonEvent[row] = 1 << index;              // queue up a new button event immediately
      buttonState[row] |= (1 << index);                         // and set the debounced state to down.
  }
    else{
      buttonDebounceCounter[row][index] = 12;
    }  // otherwise the button was previously depressed and now
    // has been released so we set our debounce counter.
  }
  else if (((buttonCurrent[row] ^ buttonLast[row]) & (1 << index)) == 0 &&  // if the current physical button state is the same as
  (buttonCurrent[row] ^ buttonState[row]) & (1 << index)) {        // the last physical button state but the current physical
    // button state is different from the current debounce 
    // state...
    if (buttonDebounceCounter[row][index] > 0 && --buttonDebounceCounter[row][index] == 0) {  // if the the debounce counter has
      // been decremented to 0 (meaning the
      // the button has been up for 
      // kButtonUpDefaultDebounceCount 
      // iterations///

      buttonEvent[row] = 1 << index;    // queue up a button state change event
      //Serial.println("Button state event");

      if (buttonCurrent[row] & (1 << index)){          // and toggle the buttons debounce state.
        buttonState[row] |= (1 << index);
      }
      else{
        buttonState[row] &= ~(1 << index);
      }
    }
  }
}



//---------------------------------------------------------------------
//------------------------SEND LED DATA--------------------------------
//---------------------------------------------------------------------

void updateLEDs()
{ //update the leds to reflect hte state of the buttons
  for (j=0;j<4;j++){
    ledData[j] = buttonState[j];
    //if (ledData[j])
   //Serial.println((String)"Button" + +"x"+ledData[j]);
  }
}


void turnOnLED(byte xPos, byte yPos)
{//turn on one led
  ledData[yPos] |= 1<<(3-xPos);
}
void turnOnLED(byte pos)
{//turn on one led
  turnOnLED(pos&B011, pos>>2);
}

void turnOffLED(byte xPos, byte yPos)
{//turn off one led
  ledData[yPos] &= ~(1<<(3-xPos));
}
void turnOffLED(byte pos)
{//turn off one led
  turnOffLED(pos&B011, pos>>2);
}
  

void toggleLED(byte xPos, byte yPos)
{
  if (ledData[yPos] == 1<<(3-xPos))
    //ledData[yPos] &= ~(1<<(3-xPos));
    turnOffLED(xPos,yPos);
  else
    //ledData[yPos] |= 1<<(3-xPos);
    turnOnLED(xPos,yPos);
}
void setLEDState(byte xPos, byte yPos, boolean state)
{//set state of led -> 1 = on, 0 = off
  if (state) {
    turnOnLED(xPos, yPos);
  } else {
    turnOffLED(xPos,yPos);
  }
}

void setLEDsByArray(boolean * states)
{//set states of all 16 leds with 4x4 boolean array states[x][y] -> 1 = on, 0 = off
  for (byte y=0;y<4;y++){
    byte statesBitmap = 0;
    for (byte x=0;x<4;x++){
      if (*(states + 4*y + x)){
        statesBitmap |= 1<<(3-x);
      }
    }
    setLEDRow(y,statesBitmap);
  }
}

void setLEDRow(byte row, byte states)
{//set entire row of four leds, states is a binary number between 0000 and 1111
  ledData[row] = states;
}

void setLEDCol(byte col, byte states)
{//set entire column of four leds, states is a binary number between 0000 and 1111
  for (byte y=0;y<4;y++)
  {
    boolean state = (states>>(3-y))&1;
    setLEDState(col, y, state);
  }
}

void clearLEDs()
{
  for (i=0;i<4;i++)
  {
    ledData[i] = 0;
  }
}


byte getStateOfButtonCol(byte col)
{
  byte buttonStatesCopy[4];//make a copy in case _buttonStates changes while we are extracting data, things could get confusing
  memcpy(buttonStatesCopy, buttonState, 4*sizeof(byte));
  byte colStates = 0;
  for (byte y=0;y<4;y++)
  {
    if ((buttonStatesCopy[y]>>(3-col))&1)
    {
      colStates |= 1<<(3-y);
    }
  }
  return colStates; 
}




//---------------------------------------------------------------------
//------------------------Basic Proto specific commands--------------------------------
//---------------------------------------------------------------------

float getBattery()
{ 
  float measuredvbat = analogRead(A9);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  Serial.print("VBat: " ); 
  Serial.println(measuredvbat);
}
