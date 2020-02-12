  #include "SugarCube.h"
  
  SugarCube sugarcube;

  void setup(){
    
//    default pin connections are given below:
//
//    Analog
//  
//    0 - Gyroscope Y (Y4.5)
//    1 - Potentiometer 1
//    2 - Gyroscope X (X4.5)
//    3 - Accelerometer Y (YAcc)
//    4 - Accelerometer X (XAcc)
//    5 - Potentiometer 2
//    
//    Digital
//    
//    0 - serial in - this much remain unconnected
//    1 - serial out - this is hooked up to the MIDI output
//    2 - 74HC165 data pin (Q7)
//    3 - 74HC165 clock pin (CP)
//    4 - 74HC165 latch pin (PL)
//    5 - 74HC595 clock pin (SH_CP)
//    6 - 74HC595 latch pin (ST_CP)
//    7 - 74HC595 data pin (DS)
//    
//    set custom pin connections using the following commands, this must happen before sugarcube.init()
//    leave digital pins 0 and 1 (RX/TX Serial pins) empty
//    sugarcube.setLedLatchPin(6);
//    sugarcube.setLedClockPin(5);
//    sugarcube.setLedDataPin(7);
//    sugarcube.setButtonLatchPin(4);
//    sugarcube.setButtonClockPin(3);
//    sugarcube.setButtonDataPin(2);
//    sugarcube.setXAccPin(A4);
//    sugarcube.setYAccPin(A3);
//    sugarcube.setPot1Pin(A1);
//    sugarcube.setPot2Pin(A5);
//    sugarcube.setXGyroPin(A2);
//    sugarcube.setYGyroPin(A0);
    
    byte patchNum = sugarcube.init();
    switch(patchNum) {
      case 0:
        {
          StepSequencer stepSequencer;
          sugarcube.setDelegate(&stepSequencer);
          for (;;) {}
        }
      case 1:
        {
          Flin flin;
          sugarcube.setDelegate(&flin);
          for (;;) {}
        }
      case 2:
        {
          Boiing boiing;
          sugarcube.setDelegate(&boiing);
          for (;;) {}
        }
      case 3:
        {
          Arp arp;
          sugarcube.setDelegate(&arp);
          for (;;) {}
        }
      case 4:
        {
          SimpleMIDIKeyboard simpleMIDIKeyboard;
          sugarcube.setDelegate(&simpleMIDIKeyboard);
          for (;;) {}
        }
      case 5:
        {
          PixelTilt pixelTilt;
          sugarcube.setDelegate(&pixelTilt);
          for (;;) {}
        }
      case 6:
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
  }
  
  void loop(){
    
  }
  
  //---------------------------------------------------------------------
  //--------------------INTERRUPT ROUTINES-------------------------------
  //---------------------------------------------------------------------
  
  ISR(TIMER1_COMPA_vect) {//time 1 interrupt, at freq of 1kHz
      sugarcube.timer1Routine();
  }
