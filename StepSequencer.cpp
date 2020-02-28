//by Amanda Ghassaei 2012, modifications/adaptions by McGreg 2020


#include "SugarCube.h"

StepSequencer::StepSequencer()
{
#ifdef DEBUG
  Serial.println("Welcome to StepSequencer");
#endif
  _tempoTimer = 0;
  _maxTempo = this->maxTempoFromPotVal(getPot2Val());
  _xOffset = xOffsetFromPotVal(getPot1Val());
  _playhead = absolutePosition(15);//start at -1
  
  //change these to change available notes
  _notes[3] = 72;
  _notes[2] = 67;
  _notes[1] = 63;
  _notes[0] = 60;   
  
  this->clearAllStorage();
}

void StepSequencer::routine100Hz()
{
  if (_tempoTimer++>_maxTempo){
    _tempoTimer = 0;
    this->incrementPlayhead();
  }
}

void StepSequencer::pot1HasChanged(int val)
{
  _xOffset = xOffsetFromPotVal(val);
  for (byte i=0;i<4;i++){
    setLEDCol(i, _seqStates[this->absolutePosition(i)]);
  }
  if (this->relativePosition()<4) setLEDCol(this->relativePosition(), 15);//turn on column
}
void StepSequencer::pot2HasChanged(int val)
{
  _maxTempo = this->maxTempoFromPotVal(val);
}
void StepSequencer::buttonPressed(byte xPos, byte yPos)
{
  byte colState = _seqStates[this->absolutePosition(xPos)];
  if (colState & 1<<(3-yPos)){//if already on
    colState &= ~(1<<(3-yPos));//turn off
    turnOffLED(xPos, yPos);
  } else {
    colState |= (1<<(3-yPos));//turn on
    turnOnLED(xPos, yPos);
  }
  _seqStates[this->absolutePosition(xPos)]  = colState;
}
void StepSequencer::instrumentHasChanged(byte val)
{
  this->wasShaken();
}
void StepSequencer::wasShaken()
{
  #ifdef DEBUG
  Serial.println("Was shaken was called!");
  #endif
  this->clearAllStorage();
  for (byte i=0;i<4;i++){
    noteOff(_notes[i], currentMidiChannel);//turn off any notes that might be stuck on
  }
   clearLEDs();
   if (this->relativePosition()<4)setLEDCol(this->relativePosition(), 15);//turn on column
}

byte StepSequencer::absolutePosition(byte pos)
{
  return (pos+_xOffset)&15;
}

byte StepSequencer::relativePosition()
{
  return (_playhead+16-_xOffset)&15;
}


byte StepSequencer::maxTempoFromPotVal(int val)//10 bit val
{
  #ifdef DEBUG
  Serial.print("tempo:");
  Serial.println(((val>>6) + 1)*5);//5-80 in 5er schritten
  #endif
  return ((val>>6) + 1)*5;
}

void StepSequencer::incrementPlayhead()
{
  if (this->relativePosition()<4)setLEDCol(this->relativePosition(), _seqStates[_playhead]);//set col to original state
  this->playNotesForStates(_seqStates[_playhead], false);
  _playhead++;
  if (_playhead>15) _playhead = 0;
  if (this->relativePosition()<4) setLEDCol(this->relativePosition(), 15);//turn on column
  this->playNotesForStates(_seqStates[_playhead], true);
}

void StepSequencer::clearAllStorage()
{
  for (byte i=0;i<16;i++){
      _seqStates[i] = 0;
  }
}
void StepSequencer::playNotesForStates(byte column, boolean noteIsOn)
{
  for (byte i=0;i<4;i++){
    if (column & 1<<i){
      if (noteIsOn){
        noteOn(_notes[i], velocity, currentMidiChannel);
      } else {
        noteOff(_notes[i], currentMidiChannel);
      }
    }
  }
}
  
