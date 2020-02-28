//by Amanda Ghassaei 2012, modifications/adaptions by McGreg 2020


#include "SugarCube.h"
#include "Utilities.h"
      
SimpleMIDIKeyboard::SimpleMIDIKeyboard()
{
#ifdef DEBUG
  Serial.println("Welcome to MIDI Keybord");
#endif
  baseNote = calculateBaseNoteFromPotVal(getPot1Val());
  pitchbendFromAnalogVal(getPot2Val(), currentMidiChannel);
}
void SimpleMIDIKeyboard::buttonPressed(byte xPos, byte yPos)
{
  turnOnLED(xPos,yPos);
//        Serial.println((String)"Note: "+createMIDINoteInFourths(xPos, yPos, baseNote)+" velocity: "+velocity+" channel: "+currentMidiChannel);
  noteOn(createMIDINoteInFourths(xPos, yPos, baseNote), velocity,currentMidiChannel);
}

void SimpleMIDIKeyboard::buttonReleased(byte xPos, byte yPos)
{
  turnOffLED(xPos,yPos);
  noteOff(createMIDINoteInFourths(xPos, yPos, baseNote),currentMidiChannel);
}

void SimpleMIDIKeyboard::pot1HasChanged(int val)
{
  baseNote = calculateBaseNoteFromPotVal(val);
  #ifdef DEBUG
  Serial.println((String)"baseNote: "+baseNote);
  #endif
}

void SimpleMIDIKeyboard::pot2HasChanged(int val)
{
  //velocity = velocityFromAnalogVal(val);
  
pitchbendFromAnalogVal(val, currentMidiChannel);
}
