
#include <stdint.h>
#include <string.h>
#include "Sample/sample.h"
#include "UART/uart.h"
#include "midi.h"
#include "VUMeter/vumeter.h"

#define MIDI_Tx(x)   UART_Tx(x)

const char MIDI_NOTES[] = "C C#D D#E F F#G G#A A#B ";
const char MIDI_BAUD[][11] = {"31.25k",
									  "38.4k",
									  "115.2kUSB",
									  "1.0M USB"};


MidiSettings_t MIDISettings = {	
	/* 15ms output rate */
	15,
	DEFAULT_BAUD_RATE,
	/* MIDI Channel Instrument # */
	(0x09 | MIDI_NOTE_ON)
};


void MIDI_Output(void)
{
   uint8_t i;
   /* do analogue inputs then do digital inputs  */
   for( i = 0; i < ANALOGUE_INPUTS; i++)
   {      
      if( GetChannelStatus(i) && 
          (RetriggerPeriod[i].timerEnable == 0) && 
          (SignalPeak[i] > GetChannelThresh(i)) )
      {
			uint16_t conditionedSignal = (SignalPeak[i] - GetChannelThresh(i));
			
			conditionedSignal = GainFunction(i, conditionedSignal);
			
			if( conditionedSignal )
			{
	         /* Send a NOTE ON | Channel */
	         MIDI_Tx(MIDISettings.MIDI_ChannelCode);
	         
	         /* Output the correct Closed or Open Key */
	         if( GetDualMode(i) && 
					 GetDigitalState(GetDigitalTrigger(i)) == GetActiveState(GetDigitalTrigger(i)) )
				{
	         	MIDI_Tx(GetChannelKeyClosed(i));
				}
				else
				{
					MIDI_Tx(GetChannelKey(i));
				}
	         
				if( conditionedSignal > 127 )
	         {
	            MIDI_Tx( 127 );   
	         }
	         else
	         {
	            MIDI_Tx( conditionedSignal );
	         } 
	         SoftTimerStart(RetriggerPeriod[i]); 
	         if( SoftTimer2[SC_VUMeterUpdate].timerEnable )
	         {
	         	VUValues[i] = SignalPeak[i];
				}
			}
      }  
   }
}


void MIDI_DigitalOutput(void)
{
   uint8_t i;
   for( i = ANALOGUE_INPUTS; i < NUMBER_OF_REAL_INPUTS; i++)
   {      
      if( GetChannelStatus(i) && 
          (RetriggerPeriod[i].timerEnable == 0))
      {
   		if( SignalPeak[i] )
   		{	
	         /* Send a NOTE ON | Channel */
	         MIDI_Tx(MIDISettings.MIDI_ChannelCode);
	         MIDI_Tx(GetChannelKey(i));
	         MIDI_Tx( GetDigitalVelocity(i - ANALOGUE_INPUTS) );
				SoftTimerStart(RetriggerPeriod[i]);
				
				
				if( SoftTimer2[SC_DigitalVUUpdate].timerEnable )
	         {
	         	VUValues[i-ANALOGUE_INPUTS] = GetDigitalVelocity(i - ANALOGUE_INPUTS);
				}
				   
			}
      }
   }
}



void MIDI_MetronomeOutput(void)
{
   uint8_t i;
   for( i = NUMBER_OF_REAL_INPUTS; i < NUMBER_OF_INPUTS; i++)
   {      
      if( GetChannelStatus(i) && 
          (RetriggerPeriod[i].timerEnable == 0))
      {
	      /* Send a NOTE ON | Channel */
	      MIDI_Tx(MIDISettings.MIDI_ChannelCode);
	      MIDI_Tx(GetChannelKey(i));
	      MIDI_Tx( GetDigitalVelocity(i - ANALOGUE_INPUTS) );
			SoftTimerStart(RetriggerPeriod[i]);
			
			if( SoftTimer2[SC_DigitalVUUpdate].timerEnable )
	      {
	         VUValues[i-ANALOGUE_INPUTS] = GetDigitalVelocity(i - ANALOGUE_INPUTS);
			}
      }
   }
}

uint16_t MIDI_GetRate(void)
{
   return MIDISettings.MIDI_OutputRate;
}

void MIDI_SetRate(uint16_t newRate)
{
	MIDISettings.MIDI_OutputRate = newRate;
   SoftTimer1[SC_MIDIOutput].timeCompare = newRate;
}

void MIDI_SetBaud(uint16_t newBaud)
{

   MIDISettings.MIDI_BaudRate = newBaud; 
   UART_SetBaudRate( newBaud >> 8, newBaud & 0xFF );
}


uint16_t MIDI_GetBaud(void)
{
   return MIDISettings.MIDI_BaudRate;
}

uint8_t MIDI_GetChannelCode(void)
{
	return (MIDISettings.MIDI_ChannelCode & 0x0F);
}

void MIDI_SetChannelCode(uint8_t newCode)
{
   if( newCode < 0x0F )
   {
		MIDISettings.MIDI_ChannelCode = MIDI_NOTE_ON | newCode;
   }
}




/* Pass any number from 0->127 and obtain the corresponding note in a string format */
void MIDI_NoteString(uint8_t note, char* buffer)
{
   note = note - (NOTE_COUNT * MIDI_Octave(note));
   strncpy(buffer, &MIDI_NOTES[note*2] , 2);
   /* Append a NULL */
   buffer[2] = '\0';
}

/* Returns the octave of the passed note */
uint8_t MIDI_Octave(uint8_t note)
{
  return (note / NOTE_COUNT);
}





