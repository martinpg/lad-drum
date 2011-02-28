

#include "hardwareSpecific.h"
#include "sample/sample.h"
#include "VUMeter/vumeter.h"
#include "MIDI/midi.h"
#include "Sample/sample.h"
#include "Menu/Menu.h"
#include "EDrumAVRMega.h"
#include "MenuSettings.h"
#include "UserFunctions/userFunctions.h"

#include "TimerCallbacks/TimerCallbacks.h"


void Callback_MIDIOutput(void)
{
   /* Update the Digital States */
   ScanDigitalInputs();
   MIDI_Output();
   MIDI_DigitalOutput();
   MIDI_MetronomeOutput();
   ResetValues();	

   /* Benchmark reporting */
   /*UART_Tx( (uint8_t)(BenchMarkCount>>8) );
   UART_Tx( (uint8_t)(BenchMarkCount) );
   BenchMarkCount = 0;*/
}



void Callback_RetriggerReset(void)
{
	uint8_t i;      
   /* Each increment of Retrigger increases the time by 1ms */
   for( i = 0; i < NUMBER_OF_INPUTS ; i++ )
   {
      if(SoftTimerInterrupt(RetriggerPeriod[i]))
      {
         SoftTimerStop(RetriggerPeriod[i]);
         SoftTimerReset(RetriggerPeriod[i]);      
      }
   }
}




void Callback_AutoMenuUpdate(void)
{
   /* Update the Threshold and Retrigger bar */
   MenuSetInput(ActiveMenu, MENU_UPDATE);
   MenuUpdate(ActiveMenu, RESET_MENU);
}

void Callback_VUDecay(void)
{
	/* Decay the VU Meters here */
	if( ActiveProcess == PLAY_MODE )
	{
   	VULevelDecay(ALL_METERS);
   }
}


void Callback_LCDBacklight(void)
{

	SoftTimerStop( SoftTimer2[SC_LCD_BL_Period] );
	UI_LCD_BL_Off();
	
	/* If no SoftTimer2's are enabled, then turn off the Timer2 module, Timer2 to be enabled
    * only after a keypress. */
	if( !SoftTimer_IsTimer2Active() )
	{
      /* Stop the Auxuliary Timer */
      DISABLE_AUXILIARY_TIMER();
   }
}




void Callback_VUMeterUpdate(void)
{
/* Do the VU Meter*/
   uint16_t i;
   uint8_t  VURows = GetVURows();
	   			           
   for( i = 0 ; i < ANALOGUE_INPUTS; i++ )
   {
		if( GetChannelStatus(i) )
		{
			if( VUValues[i] > GetChannelThresh(i) )
			{
				uint16_t conditionedSignal = (VUValues[i] - GetChannelThresh(i));
				
				conditionedSignal = GainFunction(i, conditionedSignal);
				
            /* Normalise with x rows */
            VUSetLevel(i, VUNormaliseMIDI(conditionedSignal, VURows), VURows);            
			}
      }
	}
   VUMeterPrint(SEQUENTIAL_METERS | 0x0F, VURows);
   ResetVUValues();
}



/* Digital & Metronome VU Meter  */
void Callback_DigitalVUUpdate(void)
{
   /* Do the VU Meter*/
   uint16_t i;
   uint8_t  VURows = GetVURows();
	   			           
   for( i = 0 ; i < DIGITAL_INPUTS+METRONOME_INPUTS; i++ )
   {
      uint8_t ActualChannel = i + ANALOGUE_INPUTS;
      
		if( GetChannelStatus(ActualChannel) )
		{	
			if( VUValues[i] )
			{
				VUSetLevel(i, VUNormaliseMIDI(GetDigitalVelocity(i), VURows), VURows); 
			}
		}
	}
   VUMeterPrint(SEQUENTIAL_METERS | 0x0F, VURows);
   ResetVUValues();
}

   
   

void Callback_AboutUpdate(void)
{
	uint8_t nameIndex = 0;
	nameIndex = ThanksIndex(GET);
	if( ++nameIndex == SIZEOFTHANKS )
	{
		nameIndex = ThanksIndex(MAIN_SCREEN);	
	}
	ThanksIndex(nameIndex);
	aboutScroll(nameIndex);
}
