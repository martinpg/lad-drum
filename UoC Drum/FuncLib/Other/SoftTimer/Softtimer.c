

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


   
SoftTimer_16  SoftTimer1[TIMER1B_COUNT] = {{10000, 0, 0},
														 {15, 0, 1},
														 {10000, 0, 0}	 };



SoftTimer_16  SoftTimer2[TIMER2B_COUNT] = {{10, 0, 0},
                                           {100, 0, 0}};


interrupt (TIMERB1_VECTOR) timerb1_int(void)
{
   dint();   
   
   uint8_t intVec = TBIV;
   uint8_t i;
   
   if( intVec & TBIV_CCR1 )
   {
		TBCCR1 += SAMPLE_100US;
      	
		if(SoftTimerInterrupt(SoftTimer1[SC_SecondDelay]))
		{
         if( UI_INT_PORT & UI_INT_PIN )
         {
            UI_Activate();
            UI_SetRegister(UI_INTERRUPT, 0);  
         }
         //UART_TxString("1 Sec");            
         SoftTimerReset(SoftTimer1[SC_SecondDelay]);      
      }
	}
					
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
         MIDI_Output();
			ResetValues();	
         SoftTimerReset(SoftTimer1[SC_MIDIOutput]);     
      }	    	   
         
		if(SoftTimerInterrupt(SoftTimer2[SC_VUMeterUpdate]))
		{
			/* Print 3 A's */
			//VUSetPosition(1,0);
			//VUTest();
			SoftTimerReset(SoftTimer2[SC_VUMeterUpdate]); 
		
         /* Do the VU Meter*/
         uint16_t i;
            
         /* Set to Row1, Col 0 */
         VUSetPosition(1,0);  
			/* Decay the VU Meters here */
   	   VULevelDecay(ALL_METERS);                
         for( i = 0 ; i < ANALOGUE_INPUTS; i++ )
         {

            int8_t Gain =  GetChannelGain(i);
            int16_t maxVal;
            
            /* if the gain is an attenuation by shifting down
             * 5 bits (ie Most Sig. 7 bits of the 12bit ADC value 
             * then the maximum value that can be used to normalise
             * will be 4095 */
            if( Gain <= (int8_t)(MAX_GAIN + MIN_GAIN) )
            {
               maxVal = 2047;
            }
            else
            {
               maxVal = (1 << ((MAX_GAIN) - Gain)) - 1;  
            }
            if(maxVal == 0)
            {
               maxVal = 1;  
            }
            /* maxVal should have a maximum of 2047 */

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
					
	            /* Normalise with 3 rows */
	            VUSetLevel(i, VUNormalise((conditionedSignal), 127, 1), 1);
	            
	            uint8_t outputString[10];
	            /*
	            utoa(maxVal, outputString, 10);
	            UART_TxString("Max Val = ");
	            UART_TxString(outputString);
	            UART_TxString("\r\n");
	            
	            utoa((conditionedSignal >> 1), outputString, 10);
	            UART_TxString("vs Cond.:");
	            UART_TxString(outputString);
	            UART_TxString("\r\n");	             
	            
	            utoa(VUNormalise((conditionedSignal >> 1), maxVal, 3), outputString, 10);
	            UART_TxString("Norm Val = ");
	            UART_TxString(outputString);
	            UART_TxString("\r\n");*/
	            
				}
            
         }
         VUMeterPrint(ALL_METERS, 1);
         
         ResetVUValues();
      }         
         
         
         
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
}
