//by Amanda Ghassaei 2012, modifications/adaptions by McGreg 2020


#ifndef Utilities_h
#define Utilities_h

#include "Arduino.h"

//translate button location (x,y) to MIDI note, based on fourths: http://www.youtube.com/watch?v=uQm3xbTxJRc
byte createMIDINoteInFourths(byte xPos, byte yPos, byte baseNote);
byte calculateBaseNoteFromPotVal(int val);
// byte velocityFromAnalogVal(int val);
byte xOffsetFromPotVal(int val);
byte yCoordFromColState(byte state);

//Low level LED and Button check
void shift(byte i);
void buttonCheck(byte row, byte index);

byte getStateOfButtonCol(byte col);

//LED functions
void turnOnLED(byte xPos, byte yPos);
void turnOnLED(byte pos);
void turnOffLED(byte xPos, byte yPos);
void turnOffLED(byte pos);
void toggleLED(byte xPos, byte yPos);
void setLEDState(byte xPos, byte yPos, boolean state);
void setLEDsByArray(boolean * states);
void setLEDRow(byte row, byte states);
void setLEDCol(byte col, byte states);
void clearLEDs();
void updateLEDs();



#endif
