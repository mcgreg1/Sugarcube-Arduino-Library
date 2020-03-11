//by McGreg 2020


#ifndef Midi_h
#define Midi_h


#include "Arduino.h"


#define VS1053_MIDI Serial1

// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 31
#define VS1053_BANK_DEFAULT 0x00
#define VS1053_BANK_DRUMS1 0x78
#define VS1053_BANK_DRUMS2 0x7F
#define VS1053_BANK_MELODY 0x79

// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 32 for more!
#define VS1053_GM1_OCARINA 80
#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0
#define MIDI_ALL_SOUND_OFF 0x78
#define MIDI_RESET_ALL_CONTROLLERS 0x79
#define VS1053_RESET 9

void midiSetChannelBank(byte chan, byte bank);
//void midiSetChannelVolume(uint8_t chan, uint8_t vol);
//void midiSetInstrument(uint8_t chan, uint8_t inst);
byte createMIDINoteInFourths(byte xPos, byte yPos, byte baseNote);

void pitchBend(byte pitchbend, byte channel);
void pitchbendFromAnalogVal(int val, byte channel);
void noteOff(byte note, byte channel);
void noteOn(byte note, byte velocity, byte channel);
void allNotesOff(byte channel);

void sendMIDI(byte command, byte param1, byte param2) ;
byte setInstrument(byte instrument);
void playSequenceFromNotes(byte metronom, byte *toneMem, byte len, byte lowestTone);
void playSequenceFromPosition(byte metronom, byte *toneMem, byte len, byte baseNote);
void playMario(unsigned int counter);
void playGianna(unsigned int counter);

#endif
