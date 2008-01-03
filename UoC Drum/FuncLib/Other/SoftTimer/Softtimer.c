

#include <io.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>

#include "main.h"
#include "Softtimer.h"
#include "UI/UI.h"
#include "MIDI/midi.h"
#include "Sample/sample.h"
#include "Menu/Menu.h"
#include "UserFunctions/userFunctions.h"
#include "VUMeter/vumeter.h"
#include "UART/uart.h"

#define NUMBER_OF_TIMERS   (4)

/* Holds functions for the timers and is an example of implementation*/


   
SoftTimer_16  SoftTimer1[TIMER1B_COUNT] = {{2000, 0, 1},
														 {15, 0, 1},
														 {10000, 0, 0}	 };



SoftTimer_16  SoftTimer2[TIMER2B_COUNT] = {{10, 0, 0},
                                           {50, 0, 0},
														 {25, 0, 0},
														 {10, 0, 1}};


interrupt (TIMERB1_VECTOR) timerb1_int(void)
{
   dint();   
   
   uint8_t intVec = TBIV;
   uint8_t i;
   /* For Timer 1 */
/*   if( intVec & TBIV_CCR1 )
   {

	}*/
					
   if( intVec & TBIV_CCR2 )
   {
		TBCCR2 += SAMPLE_1MS;
		
	
		
		if(SoftTimerInterrupt(SoftTimer2[SC_AutoMenuUpdate]))
		{
         /* Update the Threshold bar */
         ThresholdBar();
         SoftTimerReset(SoftTimer2[SC_AutoMenuUpdate]);     
      }
         
      /* MIDI output is in 1ms steps */
		if(SoftTimerInterrupt(SoftTimer1[SC_MIDIOutput]))
		{
			/* Update the Digital States */
			ScanDigitalInputs();
         MIDI_Output();
         MIDI_DigitalOutput();
			ResetValues();	
         SoftTimerReset(SoftTimer1[SC_MIDIOutput]);     
      }	    	   

		if(SoftTimerInterrupt(SoftTimer2[SC_VUDecay]))
		{
			SoftTimerReset(SoftTimer2[SC_VUDecay]); 
			/* Decay the VU Meters here */
   	   VULevelDecay(ALL_METERS);    				
		}
		
         
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
	            int8_t Gain =  GetChannelGain(i);
	            	
					if( VUValues[i] > GetChannelThresh(i) )
					{
						uint16_t conditionedSignal = (VUValues[i] - GetChannelThresh(i));
						
						if ( GetChannelGain(i) > 0 )
						{
							conditionedSignal = conditionedSignal << GetChannelGain(i);
						}
						else
						{
							conditionedSignal = conditionedSignal >> (-GetChannelGain(i));
						}
						
		            /* Normalise with x rows */
		            VUSetLevel(i, VUNormaliseMIDI(conditionedSignal, VURows), VURows);            
					}
	         }
			}
         VUMeterPrint(ALL_METERS, VURows);
         ResetVUValues();
      }         
         
         
		if(SoftTimerInterrupt(SoftTimer2[SC_RetriggerReset]))
		{   
			SoftTimerReset(SoftTimer2[SC_RetriggerReset]);
			      
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
		
		if(SoftTimerInterrupt(SoftTimer1[SC_SecondDelay]))
		{
         if( (UI_INT_PORT & UI_INT_PIN) )
         {
				UI_SetRegister(UI_INTERRUPT, 0);  
            UI_Activate();
         }
         //UART_TxString("1 Sec");            
         SoftTimerReset(SoftTimer1[SC_SecondDelay]);      
      }
		 
	}
}
