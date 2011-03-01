

#include "hardwareSpecific.h"
#include "sample/sample.h"
#include "VUMeter/vumeter.h"
#include "MIDI/midi.h"
#include "Sample/sample.h"
#include "Menu/Menu.h"
#include "EDrumAVRMega.h"
#include "MenuSettings.h"
#include "UserFunctions/userFunctions.h"
#include "UI_KP/UI_KP.h"

#include "LCDSettings.h"

#include "TimerCallbacks/TimerCallbacks.h"


/* These are the critical timers, 1ms resolution */
SoftTimer_16  SoftTimer1[TIMER1_COUNT] = { {15, 0, 1, Callback_MIDIOutput},   // MIDI Output
														 {10, 0, 1, Callback_RetriggerReset}};  // Retrigger Reset	 }; 



/* These are non-critical timers, and dont really run during 'play time',
   Functions which start these need to ensure they stop them for optimisation */
SoftTimer_16  SoftTimer2[TIMER2_COUNT] = {{110, 0, 0, Callback_AutoMenuUpdate},  // Threshold Bar
                                           {70, 0, 0, Callback_VUMeterUpdate},  // VU Meter Update 
                                           {70, 0, 0, Callback_DigitalVUUpdate},  // Digital VU Meter Update
														 {25, 0, 0, Callback_VUDecay},  // VU Decay
														 {2500,2500,0, Callback_AboutUpdate}, // AboutUpdate
														 {10000, 10000, 0, Callback_LCDBacklight}, //LCD Backlight
                                           {150, 0, 0, Callback_MonitorChannel},
                                           {10, 0, 0, Callback_Keypress}}; 



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
	//UI_LCD_BL_Off();
	
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
            VUSetLevel(i, VUNormalise(conditionedSignal, MIDI_MAX_DATA, VURows), VURows);            
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
				VUSetLevel(i, VUNormalise(GetDigitalVelocity(i), MIDI_MAX_DATA, VURows), VURows); 
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



void Callback_MonitorChannel(void)
{
   MIDI_Output();

   UF_MenuReset();
   char outputString[10];
   itoa(GetLastMIDIValue(SelectedChannel), outputString, 10);
   UF_MenuPrint_P(PSTR("MIDI Code:"));
   UF_MenuPrint(outputString);

   UF_MenuNewLine();

   itoa(SignalPeak[SelectedChannel], outputString, 10);
   UF_MenuPrint_P(PSTR("ADC Val: "));
   UF_MenuPrint(outputString);

   UF_MenuNewLine();
	/* Display the first slope channel 'gain' */
	itoa( (int8_t)(GetChannelGain(SelectedChannel) - GAIN_OFFSET) , outputString, 10);
	UF_MenuPrint_P(PSTR("Gain1:"));
	UF_MenuPrint(outputString);

	/* Display the channel slope 2 'gain' */
	itoa( (int8_t)(GetSlope2Gain(SelectedChannel) - GAIN_OFFSET), outputString, 10);
	UF_MenuPrint_P(PSTR(" Gain2:"));
	UF_MenuPrint(outputString);
   UF_MenuNewLine();   

	/* Display the gain crossover point */
	itoa(GetCrossover(SelectedChannel), outputString, 10);
	UF_MenuPrint_P(PSTR("Gain Crossover:"));
	UF_MenuPrint(outputString);
   UF_MenuNewLine();

   ResetValues();
}


void Callback_Keypress(void)
{
   static uint8_t result = KP_INVALID;
   uint8_t i;

   if( result == KP_INVALID )
   {
      result = UI_KP_GetPress();
      /* This will generate an interrupt flag */
      GIFR |= (1 << INTF1);
   }

   UDR = result;

   SoftTimerStop(SoftTimer2[SC_Keypress]);
   if( (result != KP_INVALID) && SoftTimer_IsTimer2Active() )
   {
      /* We have (50ms to stop everything we're doing..) */
      
      for( i = 0; i < TIMER2_COUNT; i++)
      {
         if( SoftTimerIsEnabled(SoftTimer2[i]) )
         {
            SoftTimerStop(SoftTimer2[i]);
         }
      }
      SoftTimerStart(SoftTimer2[SC_Keypress]);
      return;
   }

   if( result != KP_INVALID )
   {
      MenuSetInput(ActiveMenu, result);
      MenuUpdate(ActiveMenu, RESET_MENU);
      /* Active the back light */
      UI_LCD_BL_On();
      SoftTimerStart(SoftTimer2[SC_LCD_BL_Period]);
   }

   result = KP_INVALID;
   ENABLE_KEYPAD();
}
