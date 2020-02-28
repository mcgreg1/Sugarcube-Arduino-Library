//by Amanda Ghassaei 2012, modifications/adaptions by McGreg 2020

    
#include "SugarCube.h"
      
Arp::Arp()
{
#ifdef DEBUG
  Serial.println("Welcome to Arp");
#endif
  this->clearAllStorage();
  _tempoTimer = 0;
  _maxTempo = this->maxTempoFromPotVal(getPot2Val());
  pitchbendFromAnalogVal(getPot1Val(), currentMidiChannel);
  _baseNoteTimer = 0;
}

void Arp::pot1HasChanged(int val)
{
  pitchbendFromAnalogVal(val, currentMidiChannel);
}

void Arp::pot2HasChanged(int val)
{
  _maxTempo = this->maxTempoFromPotVal(val);
}
void Arp::instrumentHasChanged(byte val)
{
  this->wasShaken();
}
void Arp::wasShaken()
{
  this->clearAllStorage();
  clearLEDs();
}
void Arp::buttonPressed(byte xPos, byte yPos)
{
  if (_states[xPos]==0){
    turnOnLED(xPos, yPos);
    _states[xPos] = 1<<(3-yPos);
  } else {
    if (1<<(3-yPos)>_states[xPos]){
      _states[xPos] = _states[xPos]<<1;
      setLEDCol(xPos, _states[xPos]&15);
    } else if (1<<(3-yPos)<_states[xPos]){
      _states[xPos] = _states[xPos]>>1;
      setLEDCol(xPos, _states[xPos]&15);
    } else {
      byte numNotes = this->notesActive();
      if (numNotes<3) return;
      if (xPos == 0 || (xPos == 1 && _states[0]==0)) {
        this->setIncreasingPattern();
      } else if (xPos == 3 || (xPos == 2 && _states[3]==0)) {
        this->setDecreasingPattern();
      } else {
        this->setRandomPattern();
      }
    }
  }
}

void Arp::routine100Hz()
{
  if (this->notesActive()==0) return;
  if (_tempoTimer++>_maxTempo){
    _tempoTimer = 0;
    this->updateCurrentCol();//increment _currentCol
    
    //update LEDs
    for (byte i=0;i<4;i++){
      setLEDCol(i, _states[i]&15);
    }
    if (this->notesActive()!=1) setLEDCol(_currentCol, 0);//turn off current note
    
    //MIDI
    noteOff(_lastNote, currentMidiChannel);
    _lastNote = createMIDINoteInFourths(_currentCol, yCoordFromColState(_states[_currentCol]), _basenote);
    noteOn(_lastNote, velocity, currentMidiChannel);
    
    _baseNoteTimer++;
    if (_baseNoteTimer>=2*(this->notesActive())) this->checkForAcc();
  }
}

void Arp::setIncreasingPattern()
{
  for (byte i=0;i<4;i++){
    _arpPattern[i] = i;
  }
}

void Arp::setDecreasingPattern()
{
  for (byte i=0;i<4;i++){
    _arpPattern[i] = 3-i;
  }
}

void Arp::setRandomPattern()
{
  byte newPattern[] = {4, 4, 4, 4};
  for (byte i=0;i<4;i++){
    newPattern[i] = this->getRandomNumber(newPattern, random(4));
  }
  memcpy(_arpPattern, newPattern, 4*sizeof(byte));
}

byte Arp::getRandomNumber(byte pattern[4], byte newNumber)
{
  for (byte i=0;i<4;i++){
    if (pattern[i] == newNumber){
      newNumber++;
      if (newNumber>3) newNumber = 0;
      return this->getRandomNumber(pattern, newNumber);
    }
  }
  return newNumber;
}

void Arp::updateCurrentCol()
{
  _arpPatternIndex++;
  if (_arpPatternIndex>3)  _arpPatternIndex = 0;
  _currentCol = _arpPattern[_arpPatternIndex];
  if (_states[_currentCol] != 0) return;
  return this->updateCurrentCol();
}

byte Arp::notesActive()
{
  byte numNotes = 0;
  for (byte i=0;i<4;i++){
    if (_states[i] != 0) numNotes++;
  }
  return numNotes;
}

void Arp::checkForAcc()
{
  //TODO: Add proper acc value

}
byte Arp::maxTempoFromPotVal(int val)//10 bit val
{
  return ((val>>6) + 1)*5;
}

void Arp::clearAllStorage()
{
  for (byte i=0;i<4;i++){
    _states[i] = 0;
    _arpPattern[i] = i;
  }
  _arpPatternIndex = 0;
  _currentCol = _arpPattern[_arpPatternIndex];
  _basenote = 50;
}
