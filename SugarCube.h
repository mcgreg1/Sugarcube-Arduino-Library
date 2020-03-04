//by Amanda Ghassaei 2012, modifications/adaptions by McGreg 2020


#ifndef SugarCube_h
#define SugarCube_h


#include <TimerOne.h>             //for the timer Interrupt
#include <avr/wdt.h>                //for SW reset


//#include <Wire.h>                 // Must include Wire library for I2C
#include "Delegate.h"
#include "Utilities.h"
#include "Midi.h"
#include "lib/digitalWriteFast/digitalWriteFast.h" //for fast shift handling

//this firmware will cause the buttons to light up momentarily while they are pressed.

//    A0 - 18 - 74HC595 clock pin (SH_CP)
//    A1 - 19 - 74HC595 latch pin (ST_CP)
//    A2 - 20 - 74HC595 data pin (DS)
//    A3 - 21 - Poti1
//    A4 - 22 - Poti2
//    A5 - 23 - Volume
//       - 10 - 74HC165 latch pin (PL)
//       - 11 - 74HC165 clock pin (CP)
//       - 12 - 74HC165 data pin (Q7)

//       - 16 - Insturment Left
//       - 15 - Insturment Right
//       - 05 - Instrument Button

//    SCL- 03 - Accelorometer 
//    SDA- 02 - Accelorometer


#define DEBUG                   //for serial print commands

#define INSTRUMENT_MAX 129      //1-128 Melodic 
#define VELOCITY_MIN 60
#define DEFAULT_INSTRUMENT 60
#define ROTARY_IDLE_TIME 1000   //1,5 seconds
#define BUTTON_RESET_TIME 3000  //3 sec


//pin connections
#define ledLatchPin 19
#define ledClockPin 18
#define ledDataPin 20

#define buttonLatchPin 10
#define buttonClockPin 11
#define buttonDataPin 12

#define pot1Pin 21
#define pot2Pin 22
#define volPin 23

#define instLPin 15
#define instRPin 16
#define instButtonPin 5



//analog storage
extern int pot1, pot2, vol, volRaw;

//Rotary encoder values
extern byte instrument;
extern bool instrumentButton, instrumentLast;
extern int hiddenFunctions;


//looping variables
extern byte i,j,k;

//storage for led states, 4 bytes
extern byte ledData[];
//storage for buttons, 4 bytes
extern byte buttonCurrent[];
extern byte buttonLast[];
extern byte buttonEvent[];
extern byte buttonState[];

//button debounce counter- 16 bytes
extern byte buttonDebounceCounter[4][4];

//MIDI variables
extern int velocity;

extern byte instrumentPerChannel[16];

extern byte hardwareIter;
extern unsigned int timer100Hz, timer1Hz;
extern byte currentMidiChannel;

extern byte selectSig;
extern bool selectSigForward;

extern Delegate *activeMode;

//void initValues();
void buttonPressed(byte xPos, byte yPos);
void buttonReleased(byte xPos, byte yPos);
void xAccHasChanged(int val);
void yAccHasChanged(int val);
void pot1HasChanged(int val);
void pot2HasChanged(int val);
void volHasChanged(int val);
void instrumentHasChanged(byte val);
void instrumentButtonHasChanged();
void updateLEDs();
int getPot1Val();
int getPot2Val();
int analogValFromPin(byte pinNum, int oldVal);
void checkAnalogPins();
void checkRotaryEncoder();
void softwareReset();

#endif
