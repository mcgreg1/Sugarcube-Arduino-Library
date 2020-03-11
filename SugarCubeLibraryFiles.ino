//basic midi test
//based on sugarcube by Amanda Ghassaei 2012 https://github.com/amandaghassaei/Sugarcube-Arduino-Library
//Modifications/adaptions by McGreg 2020
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
*/
#include "SugarCube.h"


#ifdef INSTRUMENT_TEST
#define APPSCOUNT 7
#else
#define APPSCOUNT 6
#endif



void setup() 
{
  #ifdef DEBUG
  delay(1000);
  Serial.begin(115200);
  Serial.println("VS1053 MIDI test");
  #endif
  VS1053_MIDI.begin(31250);//set midi baud 

  VS1053_MIDI.write(MIDI_CHAN_MSG);
  VS1053_MIDI.write(MIDI_RESET_ALL_CONTROLLERS);

  midiSetChannelBank(0, VS1053_BANK_DEFAULT);
  midiSetChannelVolume(0, 127);
  midiSetInstrument(0, DEFAULT_INSTRUMENT);
  currentInstrument=DEFAULT_INSTRUMENT;
  
  pinMode(ledLatchPin,OUTPUT);
  pinMode(ledClockPin,OUTPUT);
  pinMode(ledDataPin,OUTPUT);
  pinMode(buttonLatchPin,OUTPUT);
  pinMode(buttonClockPin,OUTPUT);
  pinMode(buttonDataPin,INPUT);
  
  pinMode (pot1Pin, INPUT);
  pinMode (pot2Pin, INPUT);
  pinMode (volPin, INPUT);
  pinMode (instButtonPin, INPUT);
  pinMode (instRPin, INPUT);
  pinMode (instLPin, INPUT);
  
  initValues();

  Timer1.initialize(2500);
  Timer1.attachInterrupt(timer1Routine);
  
    
}
void initValues()
{
  clearLEDs();
  currentMidiChannel = 0;
  for (i = 0; i < 4; i++)
  {
    ledData[i] = 0;
    buttonCurrent[i] = 0;
    buttonLast[i] = 0;
    buttonEvent[i] = 0;
    buttonState[i] = 0;
  }
  for (i=0; i<16; i++)
  {
    allNotesOff(i);
    instrumentPerChannel[i]=0;
  }
}


void routine100Hz()
{
  if (activeMode != NULL)
    activeMode->routine100Hz();

  else //signal to user for select
  {
    
      clearLEDs();
      turnOnLED(selectSig/20);
      if (selectSig==APPSCOUNT*20-1)
        selectSigForward=false;
      else if (selectSig==0)
        selectSigForward=true;
      if (selectSigForward)
        selectSig++;
      else
        selectSig--;

    }
    
}

  void timer1Routine()//1kHz
  {
    shift(hardwareIter);
   // shift();
    //updateLEDs();
    hardwareIter++;
    if (hardwareIter>3)
    {
      hardwareIter = 0;
      routine100Hz();
    }
    checkAnalogPins();
    
    checkRotaryEncoder();
    /*
    if (++timer100Hz>9) 
    {
      routine100Hz();
      timer100Hz = 0;
    }
    */
  }

void buttonPressed(byte xPos, byte yPos)
{
  #ifdef DEBUG
    Serial.println((String)"Button Pressed: "+xPos+"x"+yPos);
  #endif
    if (activeMode != NULL)
    {
      activeMode->buttonPressed(xPos,yPos);
    }
  //turnOnLED(xPos,yPos);

}

void buttonReleased(byte xPos, byte yPos)
{
    //turnOffLED(xPos,yPos);
    if (activeMode != NULL)
    {
      activeMode->buttonReleased(xPos,yPos);
    }
    else //select current active mode 
    {
      changeActiveMode(yPos*4 + xPos);
    }
}



void changeActiveMode(int sel)
{
  if (sel<APPSCOUNT || sel==14)
  {
    initValues();
#ifdef DEBUG
    Serial.println((String)"Changing active mode to: "+sel);
#endif
    if (activeMode != NULL)
    {
      delete activeMode;
      activeMode=NULL;
    }
    switch (sel)
    {

        case 0:
          {
            activeMode= new SimpleMIDIKeyboard();
            break;
          }
        case 1:
          {
            activeMode= new HeartBeat(); 
            break;
          }
        case 2:
          {
            activeMode = new FlipFlop();
            break;
          }
        case 3:
          {
            activeMode = new StepSequencer ();
            break;
          }
        case 4:
          {
            activeMode = new Arp();
            break;
          }
        case 5:
          {
            activeMode = new Memory();
            break;
          }
        case 6:
          #ifdef INSTRUMENT_TEST
        {
            activeMode = new InstrumentTest();
            break;
        }
        #endif
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:

  
        break;
      }
    }//if sel<7
    
}

void loop() 
{
  //noInterrupts();

  //interrupts();
}

  
