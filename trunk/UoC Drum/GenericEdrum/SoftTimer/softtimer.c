

#include <stdint.h>
#include <stdlib.h>

#include "hardwareSpecific.h"
#include "EDrumAVRMega.h"
#include "Softtimer.h"
#include "MIDI/midi.h"
#include "Sample/sample.h"
#include "Menu/Menu.h"
#include "UserFunctions/userFunctions.h"
#include "VUMeter/vumeter.h"
#include "UI_LCD/UI_LCD.h"
#include "UI_KP/UI_KP.h"
#include "LCDSettings.h"
#include "MenuSettings.h"

#define NUMBER_OF_TIMERS   (4)

/* Holds functions for the timers and is an example of implementation*/


/* These are the critical timers */
SoftTimer_16  SoftTimer1[TIMER1B_COUNT] = {{100, 0, 0},  // Second Delay...
														 {15, 0, 1},   // MIDI Output
														 {10, 0, 1}};  // Retrigger Reset	 }; 



/* These are non-critical timers */
SoftTimer_16  SoftTimer2[TIMER2B_COUNT] = {{110, 0, 0},  // Threshold Bar
                                           {70, 0, 0},  // VU Meter Update 
                                           {70, 0, 0},  // Digital VU Meter Update
														 {25, 0, 0},  // VU Decay
														 {2500,2500,0}, // AboutUpdate
														 {10000, 10000, 0}}; //LCD Backlight



/* For SMCLK clocked at MCLK = fosc = 8MHz */
void SoftTimer_TimerInit(void)
{
   /* No Divider, 16bit operation @ SMCLK rate, activate Interrupts */
//   TBCTL |= (TBSSEL_SMCLK | MC_CONT);
     
   /* 100us timer */
   /*TBCCTL1 |= (CCIE);   
   TBCCR1 = SAMPLE_100US;*/

   /* 1ms Timer for Auxiliary use */
//   TBCCTL2 |= (CCIE);
//   TBCCR2 = SAMPLE_1MS;
   
   /* 1ms Playmode Timer */
//  TBCCTL0 |= (CCIE);
//   TBCCR0 = (SAMPLE_1MS);

   /* Primary Timer 1024 prescaler */
   TCCR0 |= (0x05);
   OCR0 = (SAMPLE_1MS);

   TIMSK |= (1 << OCIE0);


   

}


ISR(TIMER0_COMP_vect, ISR_NOBLOCK)
{
	//cli(); 

//	TBCCR0 += (SAMPLE_1MS);
   OCR0 += (SAMPLE_1MS);
   /* MIDI output is in 1ms steps */
	if(SoftTimerInterrupt(SoftTimer1[SC_MIDIOutput]))
	{
		/* Update the Digital States */
		ScanDigitalInputs();
      MIDI_Output();
      MIDI_DigitalOutput();
      MIDI_MetronomeOutput();
		ResetValues();	
      SoftTimerReset(SoftTimer1[SC_MIDIOutput]);  
      
      /* Benchmark reporting */
      /*UART_Tx( (uint8_t)(BenchMarkCount>>8) );
      UART_Tx( (uint8_t)(BenchMarkCount) );
      BenchMarkCount = 0;*/
      
         
   }	    
	      
	if(SoftTimerInterrupt(SoftTimer1[SC_RetriggerReset]))
	{   
		SoftTimerReset(SoftTimer1[SC_RetriggerReset]);
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

   RunAuxTimers();

	//sei();
}


//ISR(TIMER1_COMPA_vect, ISR_NOBLOCK)
void RunAuxTimers(void)
{
   //cli();   
   
//   uint8_t intVec = TBIV;
					
//   if( intVec & TBIV_CCR2 )
   {
//		TBCCR2 += SAMPLE_1MS;
		
		if(SoftTimerInterrupt(SoftTimer2[SC_AutoMenuUpdate]))
		{
         /* Update the Threshold and Retrigger bar */
         MenuSetInput(ActiveMenu, KP_UPDATE);
         MenuUpdate(ActiveMenu, RESET_MENU);
         //ThresholdBar();
         SoftTimerReset(SoftTimer2[SC_AutoMenuUpdate]);     
      }
         
		if(SoftTimerInterrupt(SoftTimer2[SC_VUDecay]))
		{
			SoftTimerReset(SoftTimer2[SC_VUDecay]); 
			/* Decay the VU Meters here */
			if( ActiveProcess == PLAY_MODE )
			{
   	   	VULevelDecay(ALL_METERS);
			}
		}

#if 0		
		if( SoftTimerInterrupt(SoftTimer2[SC_LCD_BL_Period]) )
		{
			SoftTimerReset( SoftTimer2[SC_LCD_BL_Period] );
			SoftTimerStop( SoftTimer2[SC_LCD_BL_Period] );
			UI_LCD_BL_Off();
			
			/* If no SoftTimer2's are enabled, then turn off the Timer2 module */
			if( !SoftTimer_IsTimer2Active() )
			{
            /* Stop the Auxuliary Timer */
//            TBCCTL2 &= ~(CCIE);
         }
			
		}
#endif

#if 1         
		if(SoftTimerInterrupt(SoftTimer2[SC_VUMeterUpdate]))
		{
			SoftTimerReset(SoftTimer2[SC_VUMeterUpdate]); 
		
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
		if(SoftTimerInterrupt(SoftTimer2[SC_DigitalVUUpdate]))
		{
			SoftTimerReset(SoftTimer2[SC_DigitalVUUpdate]); 
		
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
#endif

#if 1      
		/* About Strings Update routine */   
      if( SoftTimerInterrupt(SoftTimer2[SC_AboutUpdate]) )
		{
			uint8_t nameIndex = 0;
			nameIndex = ThanksIndex(GET);
			if( ++nameIndex == SIZEOFTHANKS )
			{
				nameIndex = ThanksIndex(MAIN_SCREEN);	
			}
			ThanksIndex(nameIndex);
			aboutScroll(nameIndex);
			SoftTimerReset(SoftTimer2[SC_AboutUpdate]);
		}  
#endif       
	}

	
	//sei();
}


uint8_t SoftTimer_IsTimer2Active(void)
{
   uint8_t i;
   
   for( i = 0; i < TIMER2B_COUNT; i++)
   {
      if( SoftTimer2[i].timerEnable )
      {
         return 1;  
      }
   }
   
   return 0;
}
