//by Amanda Ghassaei 2012, modifications/adaptions by McGreg 2020
  
  
#ifndef Delegate_h
#define Delegate_h

#include "Arduino.h"

class Delegate//parent class - allows message to be sent from sugarcube class into individual apps
{
  public:
  
    Delegate(){}//constructor method


    //buttons
    void virtual buttonPressed(byte xPos, byte yPos){}
    void virtual buttonReleased(byte xPos, byte yPos){}
    
    //analog
    void virtual xAccHasChanged(int val){}
    void virtual yAccHasChanged(int val){}
    void virtual pot1HasChanged(int val){}
    void virtual pot2HasChanged(int val){}
    //TODO: Volume and instrument are global
    void virtual volHasChanged(int val){}
    void virtual instrumentHasChanged(byte val){}
    
    void virtual wasShaken(){}
    
    //100Hz timer routine
    void virtual routine100Hz(){}
    
 };

  

class Arp: public Delegate {
  
  public:
  
    Arp();
    
    void pot1HasChanged(int val);//velocity
    void pot2HasChanged(int val);//tempo
    void wasShaken();//clear
    void buttonPressed(byte xPos, byte yPos);
    void instrumentHasChanged(byte val);
    void routine100Hz();

  private:
  
    void setDecreasingPattern();
    void setIncreasingPattern();
    void setRandomPattern();
    byte getRandomNumber(byte pattern[4], byte newNumber);
    void checkForAcc();
    
    byte _currentCol;
    byte _arpPattern[4];
    byte _arpPatternIndex;
    void updateCurrentCol();
    
    byte _states[4];
    byte notesActive();
    
    byte _basenote;
    byte _baseNoteTimer;
    byte _lastNote;//leep track of this so we can turn it off
    
    byte _maxTempo;
    byte _tempoTimer;
    byte maxTempoFromPotVal(int val);
    byte _velocity;
    
    void clearAllStorage();
};



class Flin: public Delegate {
  
  public:
  
    Flin();
    
    void pot1HasChanged(int val);//horizontal scroll
    void wasShaken();//clear
    void buttonPressed(byte xPos, byte yPos);
    void instrumentHasChanged(byte val);
    void routine100Hz();
    
    
  private:
  
    unsigned long _states[16];//16x32
    byte _notes[16];
    
    byte _xOffset;
    byte absolutePosition(byte pos);
    byte relativePosition(byte pos);
    
    byte _columnTimers[16];
    byte _columnStepTime[16];
    void incrementCol(byte colNum, byte channel);
    
    void clearAllStorage();
};

class FlipFlop: public Delegate {

  public:

    FlipFlop();

    void buttonPressed(byte xPos, byte yPos);
    void buttonReleased(byte xPos, byte yPos); //for later: duration = tempo
    void pot1HasChanged(int val);//base note
    void pot2HasChanged(int val);//pitchbend
    void instrumentHasChanged(byte val);
    void wasShaken();
    void routine100Hz();

  private: 
      struct ColInfo {
      bool activated;
      bool goingUp;
      byte absPosition;
      byte height;
      byte tempo;//maybe later
      byte channel;
      byte currentHeight;
      byte playedTone;
      //bool buttonPressed; TODO: 
    };
    byte notes[16];//fixed notes
    ColInfo column[16];
    byte relativePosition(byte pos);
    void updateColumn(byte colNbr);
    void updateLED(byte colNbr);
    void clearAllStorage();
    byte xOffset;
    byte absolutePosition(byte pos);
    byte getOffsetFromPotVal(int pos);
    unsigned int currentTempo;
    byte tempo;
    
};  


class HeartBeat: public Delegate {
    
    public:
      HeartBeat();
      void buttonPressed(byte xPos, byte yPos);
      void buttonReleased(byte xPos, byte yPos);
      void pot1HasChanged(int val);
      void pot2HasChanged(int val);
      void instrumentHasChanged(byte val);
      void routine100Hz();

    private:
      byte _baseNote;
      struct ButtonInfo {
        bool activated;
        byte tempo;
        byte channel;
        byte note;
        bool buttonPressed;
      };
      void updatePixel(byte pos);
      ButtonInfo pixel[16];
      /*
      bool active=false;
      
      bool activeLED[16];
      bool LEDStatus[16];
      byte tempoLED[16];
      */
      byte currentLED;
      byte heartBeatCounter;
      byte metronom;
      unsigned int currentTempo;
};


class Memory: public Delegate {

public:
  Memory();
  void buttonPressed(byte xPos, byte yPos);
  void buttonReleased(byte xPos, byte yPos);
  void pot1HasChanged(int val);
  void pot2HasChanged(int val);
  void routine100Hz();

private:
    byte getRandomButton();
    void correct();
    void wrong();
    void playTone();
    
    byte toneLen;
    bool playbackMode;
    bool wrongCorrectMode;
    bool marioMode;
    byte baseNote;
    
    byte currentLED;
    byte currentTone;
    byte toneMemory[16]; //we remember up to 16 tones
    byte tempo;
    byte metronom;
    unsigned int counter;
    byte currentTempo;

};



class SimpleMIDIKeyboard: public Delegate {
  
  public:

    SimpleMIDIKeyboard();
    
    void buttonPressed(byte xPos, byte yPos);
    void buttonReleased(byte xPos, byte yPos);
    void pot1HasChanged(int val);//base note
    void pot2HasChanged(int val);//pitchbend

    
  private:
  
    byte baseNote;//lowest note in the keyboard

};

class StepSequencer: public Delegate {

public:

  StepSequencer();
  
  void routine100Hz();
  void pot1HasChanged(int val);//horizontal scroll
  void pot2HasChanged(int val);//tempo
  void buttonPressed(byte xPos, byte yPos);
  void instrumentHasChanged(byte val);
  void wasShaken();//clear
  
private:
      
  byte _seqStates[16];//storage array for sequencer
  byte _xOffset;//used for scrolling through steps
  byte absolutePosition(byte pos);
  byte relativePosition();

  byte _tempoTimer;
  byte _maxTempo;
  byte maxTempoFromPotVal(int val);
  
  byte _playhead;//which beat we're currently on
  void incrementPlayhead();
  
  byte _velocity;
  byte _notes[4];
  void clearAllStorage();
  void playNotesForStates(byte column, boolean noteOn);
};

    
#endif
