//by McGreg 2020


#include "Midi.h"
#include "SugarCube.h"


byte gianna[] = {50,55,50,57,50,58,50,57,50,55,50,60,50,58,50,57,50,55,50,57,50,58,50,60,50,62,50,60,50,58,50,57,50,55,50,57,50,58,50,57,50,55,50,60,50,58,50,57,50,54,50,55,50,57,50,58,50,60,50,62,50,58,50,57,50,55,50,57,50,58,50,57,50,55,50,60,50,58,50,57};
byte marioc[] = {76, 76, 72, 72, 76, 76, 76, 76, 72, 72, 72, 72, 76, 76, 72, 72, 72, 72, 76, 76, 79, 79, 79, 79, 71, 71, 71, 71, 67, 67, 67, 67, 59, 59, 59, 59, 72, 72, 72, 72, 67, 67, 67, 67, 64, 64, 64, 64, 69, 69, 69, 69, 71, 71, 71, 71, 70, 70, 69, 69, 67, 67, 67, 76, 76, 76, 79, 79, 79, 81, 81, 81, 81, 77, 77, 79, 79, 76, 76, 76, 76, 72, 72, 74, 74, 71, 71, 72, 72, 72, 72, 67, 67, 67, 67, 64, 64, 64, 64, 69, 69, 69, 69, 71, 71, 71, 71, 70, 70, 69, 69, 67, 67, 67, 76, 76, 76, 79, 79, 79, 81, 81, 81, 81, 77, 77, 79, 79, 76, 76, 76, 76, 72, 72, 74, 74, 71, 71, 60, 60, 79, 79, 78, 78, 77, 77, 75, 75, 76, 76, 72, 72, 68, 68, 69, 69, 72, 72, 69, 69, 72, 72, 74, 74, 60, 60, 79, 79, 78, 78, 77, 77, 75, 75, 76, 76, 72, 72, 84, 84, 76, 76, 84, 84, 76, 76, 84, 84, 76, 76, 67, 67, 60, 60, 79, 79, 78, 78, 77, 77, 75, 75, 76, 76, 72, 72, 68, 68, 69, 69, 72, 72, 69, 69, 72, 72, 74, 74, 75, 75, 68, 68, 74, 74, 67, 67, 72, 72, 64, 64, 67, 67, 67, 67, 60, 60, 60, 60, 79, 79, 78, 78, 77, 77, 75, 75, 76, 76, 72, 72, 68, 68, 69, 69, 72, 72, 69, 69, 72, 72, 74, 74, 60, 60, 79, 79, 78, 78, 77, 77, 75, 75, 76, 76, 72, 72, 84, 84, 76, 76, 84, 84, 76, 76, 84, 84, 76, 76, 67, 67, 60, 60, 79, 79, 78, 78, 77, 77, 75, 75, 76, 76, 72, 72, 68, 68, 69, 69, 72, 72, 69, 69, 72, 72, 74, 74, 75, 75, 68, 68, 74, 74, 67, 67, 72, 72, 64, 64, 67, 67, 67, 67, 60, 60, 72, 72, 64, 64, 72, 72, 72, 72, 72, 72, 74, 74, 76, 76, 72, 72, 69, 69, 67, 67, 60, 60, 72, 72, 64, 64, 72, 72, 72, 72, 72, 72, 74, 74, 76, 76, 67, 67, 60, 60, 60, 60, 72, 72, 64, 64, 72, 72, 72, 72, 72, 72, 74, 74, 76, 76, 72, 72, 69, 69, 67, 67, 60, 60, 76, 76, 72, 72, 76, 76, 72, 72, 76, 76, 72, 72, 72, 72, 76, 76, 79, 79, 79, 79, 71, 71, 71, 71, 67, 67, 67, 67, 59, 59, 59, 59, 72, 72, 72, 72, 67, 67, 67, 67, 64, 64, 64, 64, 69, 69, 69, 69, 71, 71, 71, 71, 70, 70, 69, 69, 67, 67, 67, 76, 76, 76, 79, 79, 79, 81, 81, 81, 81, 77, 77, 79, 79, 76, 76, 76, 76, 72, 72, 74, 74, 71, 71, 72, 72, 72, 72, 67, 67, 67, 67, 64, 64, 64, 64, 69, 69, 69, 69, 71, 71, 71, 71, 70, 70, 69, 69, 67, 67, 67, 76, 76, 76, 79, 79, 79, 81, 81, 81, 81, 77, 77, 79, 79, 76, 76, 76, 76, 72, 72, 74, 74, 71, 71, 76, 76, 72, 72, 67, 67, 67, 67, 68, 68, 68, 68, 69, 69, 77, 77, 77, 77, 69, 69, 60, 60, 71, 71, 71, 67, 67, 67, 81, 81, 81, 77, 77, 77, 81, 81, 81, 77, 77, 77, 81, 81, 81, 77, 77, 77, 79, 79, 79, 71, 71, 71, 77, 77, 77, 76, 76, 72, 72, 69, 69, 67, 67, 60, 60, 76, 76, 72, 72, 67, 67, 67, 67, 68, 68, 68, 68, 69, 69, 77, 77, 77, 77, 69, 69, 60, 60, 71, 71, 67, 67, 77, 77, 69, 69, 77, 77, 69, 69, 77, 77, 77, 69, 69, 69, 76, 76, 76, 67, 67, 67, 74, 74, 74, 65, 65, 65, 72, 72, 64, 64, 67, 67, 67, 67, 60, 60, 60, 60, 76, 76, 72, 72, 67, 67, 67, 67, 68, 68, 68, 68, 69, 69, 77, 77, 77, 77, 69, 69, 60, 60, 71, 71, 71, 67, 67, 67, 81, 81, 81, 77, 77, 77, 81, 81, 81, 77, 77, 77, 81, 81, 81, 77, 77, 77, 79, 79, 79, 71, 71, 71, 77, 77, 77, 76, 76, 72, 72, 69, 69, 67, 67, 60, 60, 76, 76, 72, 72, 67, 67, 67, 67, 68, 68, 68, 68, 69, 69, 77, 77, 77, 77, 69, 69, 60, 60, 71, 71, 67, 67, 77, 77, 69, 69, 77, 77, 69, 69, 77, 77, 77, 69, 69, 69, 76, 76, 76, 67, 67, 67, 74, 74, 74, 65, 65, 65, 72, 72, 64, 64, 67, 67, 67, 67, 60, 60, 60, 60, 72, 72, 64, 64, 72, 72, 72, 72, 72, 72, 74, 74, 76, 76, 72, 72, 69, 69, 67, 67, 60, 60, 72, 72, 64, 64, 72, 72, 72, 72, 72, 72, 74, 74, 76, 76, 67, 67, 60, 60, 60, 60, 72, 72, 64, 64, 72, 72, 72, 72, 72, 72, 74, 74, 76, 76, 72, 72, 69, 69, 67, 67, 60, 60, 76, 76, 72, 72, 76, 76, 72, 72, 76, 76, 72, 72, 72, 72, 76, 76, 79, 79, 79, 79, 71, 71, 71, 71, 67, 67, 67, 67, 59, 59, 59, 59, 76, 76, 72, 72, 67, 67, 67, 67, 68, 68, 68, 68, 69, 69, 77, 77, 77, 77, 69, 69, 60, 60, 71, 71, 71, 67, 67, 67, 81, 81, 81, 77, 77, 77, 81, 81, 81, 77, 77, 77, 81, 81, 81, 77, 77, 77, 79, 79, 79, 71, 71, 71, 77, 77, 77, 76, 76, 72, 72, 69, 69, 67, 67, 60, 60, 76, 76, 72, 72, 67, 67, 67, 67, 68, 68, 68, 68, 69, 69, 77, 77, 77, 77, 69, 69, 60, 60, 71, 71, 67, 67, 77, 77, 69, 69, 77, 77, 69, 69, 77, 77, 77, 69, 69, 69, 76, 76, 76, 67, 67, 67, 74, 74, 74, 65, 65, 65, 72, 72, 64, 64, 67, 67, 67, 67, 60, 60, 60, 60};
     

//---------------------------------------------------------------------
  //--------------VS1053 specific functions------------------------
  //---------------------------------------------------------------------

void midiSetChannelBank(byte chan, byte bank) 
{
  if (chan<16 && bank <128)
  {  
    VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
    VS1053_MIDI.write((uint8_t)MIDI_CHAN_BANK);
    VS1053_MIDI.write(bank);
  }
}

void sendMIDI(byte command, byte param1, byte param2) 
{
  VS1053_MIDI.write(command);
  VS1053_MIDI.write(param1);
  VS1053_MIDI.write(param2);

}

void midiSetInstrument(uint8_t chan, uint8_t inst) 
{
  if (chan<16 && inst <128)
  {  
  
    VS1053_MIDI.write(MIDI_CHAN_PROGRAM | chan);  
    //delay(10);
    VS1053_MIDI.write(inst);
    //delay(10);
  }
}

void midiSetChannelVolume(uint8_t chan, uint8_t vol) 
{
  if (chan<16 && vol <128)
  {  
    VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
    VS1053_MIDI.write(MIDI_CHAN_VOLUME);
    VS1053_MIDI.write(vol);
  }
}

void noteOn(byte note, byte velocity, byte channel)
{
  sendMIDI(MIDI_NOTE_ON|channel, note, velocity);
  #ifdef DEBUG
        Serial.print("Note On: ");
        Serial.print(note);
        Serial.print(" velocity: ");
        Serial.print(velocity);
        Serial.print(" channel: ");
        Serial.println(channel);
 #endif
}

void noteOff(byte note, byte channel)
{
  sendMIDI(MIDI_NOTE_OFF|channel, note, velocity);//note on message with zero velocity == noteoff
  #ifdef DEBUG
        Serial.print("\tNote Off: ");
        Serial.print(note);
        Serial.print(" channel: ");
        Serial.print(channel);
                Serial.print(" velocity: ");
        Serial.println(velocity);
 #endif

}

void allNotesOff(byte channel)
{
   VS1053_MIDI.write(MIDI_CHAN_MSG | channel);
   VS1053_MIDI.write(MIDI_ALL_SOUND_OFF);
   noteOn(65, 100, channel);
   noteOff(65, channel);
   #ifdef DEBUG
   Serial.println("All notes OFF!!");
   #endif
}

void pitchBend(byte pitchbend, byte channel)
{
  sendMIDI(224|channel, 0, pitchbend);//command&channel, lsb, msb
}

void pitchbendFromAnalogVal(int val, int channel)
{
  byte pitchbend = constrain(map(val, 0, 1023, 0, 127), 0, 127);
  pitchBend(pitchbend, channel);

}

byte setInstrument(byte instrument)
{
  if (instrument>128 || instrument <1 ) return 0;

  bool channelFound=false;
  for (i=1; i<16; i++)
  {
    if (instrumentPerChannel[i]==0 || instrumentPerChannel[i] == instrument)//it's free, or we have already this instrument assigned
    {
      currentMidiChannel =i;
      channelFound=true;
      break;
    }
  }
  if (!channelFound)//take the oldest channel
  {
    if (currentMidiChannel==15)
      currentMidiChannel=0;
    else 
      currentMidiChannel++; 
      
    //send note OFF to this channel before reusing
    allNotesOff(currentMidiChannel);

  }
  instrumentPerChannel[currentMidiChannel]=instrument; //pos is the channel, value the instrument

  VS1053_MIDI.write(0xC0|currentMidiChannel);//command&channel, lsb, msb
  VS1053_MIDI.write(instrument);
#ifdef DEBUG
Serial.println((String)"Instrument: "+instrument+" uses channel: "+currentMidiChannel+", was free: "+channelFound);
#endif
    
 return currentMidiChannel;

}

void playSequenceFromNotes(byte metronom, byte *toneMem, byte len, byte lowestTone)
{
    if (metronom>0 && metronom<=len)//switch off the last tone
    {
        int xPrev=(toneMem[metronom-1]-lowestTone)&B011;
        int yPrev=(toneMem[metronom-1]-lowestTone)>>2;
        int notePrev = toneMem[metronom-1];
        noteOff(notePrev,currentMidiChannel);
        turnOffLED(xPrev,yPrev);
    }
  
    if (metronom<len)
    {
      int x=(toneMem[metronom]-lowestTone)&B011;
      int y=(toneMem[metronom]-lowestTone)>>2;
      int note = toneMem[metronom];

      noteOn(note, velocity,currentMidiChannel);
      turnOnLED(x,y);
    }
    else
    {
      //if done with play, signal to user
        clearLEDs();

    }
    
}

void playSequenceFromPosition(byte metronom, byte *toneMem, byte len, byte baseNote)
{

    //play tone on odd, pause on even
  if (metronom<2*len)
  {
    byte x=toneMem[metronom/2]&B011;
    byte y=toneMem[metronom/2]>>2;
    byte note=toneMem[metronom/2]+baseNote;
    if (metronom%2==1) //pause
    {
      noteOff(note,currentMidiChannel);
      turnOffLED(x,y);
      #ifdef DEBUG
      Serial.print("\tMetronom:");
      Serial.println(metronom);//: "+metronom+" metronom%2 = "+metronom%2+" metronom/2 = "+metronom/2);
      #endif
    }
    else
    {
      noteOn(note, velocity,currentMidiChannel);
      turnOnLED(x,y);
      #ifdef DEBUG
      Serial.print("\tMetronom:");
      Serial.println(metronom);//: "+metronom+" metronom%2 = "+metronom%2+" metronom/2 = "+metronom/2);
      #endif
    }
  }
  else
  {
    //if done with play, signal to user
    if (metronom==len*2)
    {
      clearLEDs();

    }
  }
}


void playGianna(unsigned int counter)
{
  //50-62: 12 tones
  
    //set a 4 tone per sec tempo
    if (counter%25==0)
    {
      byte metronom=counter/25;
      playSequenceFromNotes(metronom, gianna, sizeof(gianna), 50);
    }
}
          
 
void playMario(unsigned int counter)
{
  //59-84 : 25 tones
  if (counter%10==0) 
  {
    byte metronom=counter/10;
     playSequenceFromNotes(metronom, marioc, sizeof(marioc), 60);
  }
 
}


        
        
