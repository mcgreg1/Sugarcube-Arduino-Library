
# Work still in progress


# Sugarcube-Arduino-Library
A grid-based midi instrument, with accelerometer and gyroscope for playful interactions

Idea and code based on SugarCube by Amanda Ghassaei:
https://www.instructables.com/id/Sugarcube-MIDI-Controller/

This version is an implementation is for the Adafruit Feather 32u4 Basic Proto Board (Atmega32u4) with the Adafruit MusicMaker Featherwing Hat (VS1053B).


The Goal is to have a portable MIDI Controller device with Audio output.

Since it was not possible to upload the code to the board (board became unresponsive),
following code changes were done: 

- Rework the code Framework
- Timer based on the TimerOne/TimerThree library
- Accelerometer was removed (see below)
- Rotary Encoder added to change the instrument (see VS1053B Spec)
- Boiing was crashing the device, new implementation as FlipFlop
- Memory Game added
- Heartbeat added
- PixelTilt removed, since we don't have accelerometer
- Shake (to clear all) is simulated by pressing the rotary encoder
- 3rd potentiomenter to control velocity(volume)


Still to be done: 

- Build that device!
- Flin is crashing the device, rework needed
- I2C Acceleromater is crashing the device, probably because of ISR
- If I2C is working, re-introduce Accelerometer

Hardware differences: 

- Adafruit Feather 32u4 Basic Proto Board (Atmega32u4) with the Adafruit MusicMaker Featherwing Hat (VS1053B) 
- Using low power LEDs: only one 74HC595 is needed
- I2C Accelerometer, but not functional now
- 3rd Potentiometer for volume
- Rotary encoder with push button to change instruments and clear current mode
- 3W Speakers


