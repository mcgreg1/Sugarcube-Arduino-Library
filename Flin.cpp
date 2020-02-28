//by Amanda Ghassaei 2012, modifications/adaptions by McGreg 2020

#include "SugarCube.h"

Flin::Flin()
{
#ifdef DEBUG
  Serial.println("Welcome to Flin");
#endif
  this->clearAllStorage();
  _xOffset = xOffsetFromPotVal(getPot1Val());
  byte notes[] = {55, 57, 60, 62, 65, 67, 69, 72, 74, 77, 79, 81, 84, 86, 89, 91};
  for (byte i=0;i<16;i++){
    _notes[i] = notes[i];
  }
}

  
void Flin::pot1HasChanged(int val)
{
  _xOffset = xOffsetFromPotVal(val);
  for (byte i=0;i<4;i++){
    setLEDCol(i, _states[this->absolutePosition(i)]&15);
  }
}
void Flin::instrumentHasChanged(byte val)
{
  //TODO: Intrument per column
  this->wasShaken();
}
void Flin::wasShaken()
{
  for (byte i=0;i<16;i++){
    if (_states[i]&1) noteOff(_notes[i], currentMidiChannel);
  }
  this->clearAllStorage();
  clearLEDs();
}
void Flin::buttonPressed(byte xPos, byte yPos)
{
  turnOnLED(xPos, yPos);
  byte absolutePosition = this->absolutePosition(xPos);
  _states[absolutePosition] |= 1<<(3-yPos);
  if (_columnStepTime[absolutePosition]==0)
  {

    byte yAcc = getPot2Val();

    if (yAcc>50) yAcc = 50;
    _columnStepTime[absolutePosition] = yAcc+3;
  }
}
  
void Flin::routine100Hz()
{
  for (byte i=0;i<16;i++){
    this->incrementCol(i, currentMidiChannel);
  }
  for (byte i=0;i<4;i++){
    setLEDCol(i, _states[this->absolutePosition(i)]&15);
  }
}


byte Flin::absolutePosition(byte pos)
{
  return (pos+_xOffset)&15;
}

byte Flin::relativePosition(byte pos)
{
  return (pos+16-_xOffset)&15;
}

void Flin::incrementCol(byte colNum, byte channel)
{
  if (_states[colNum]==0) return;
  if (_columnTimers[colNum]++>_columnStepTime[colNum]){
      boolean lsb = _states[colNum]&1;
      _states[colNum] = _states[colNum]>>1;
      if (this->relativePosition(colNum)<4) _states[colNum] |= getStateOfButtonCol(this->relativePosition(colNum));
      if (lsb==1) {
        unsigned long msb = 1;
        _states[colNum] |= (msb<<31);
        if (!(boolean)_states[colNum]) noteOff(_notes[colNum], currentMidiChannel);
      } else {
        if (_states[colNum]&1) noteOn(_notes[colNum], constrain(127-_columnStepTime[colNum], 110, 127), currentMidiChannel);
      }
    _columnTimers[colNum] = 0;
  }
}

void Flin::clearAllStorage()
{
  for (byte i=0;i<16;i++){
    _states[i] = 0;
    _columnTimers[i] = 0;
    _columnStepTime[i] = 0;
  }
}
