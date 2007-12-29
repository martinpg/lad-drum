

#include <io.h>
#include <stdint.h>
#include <signal.h>

#include "main.h"
#include "Softtimer.h"
#include "UI/UI.h"
#include "MIDI/midi.h"
#include "Sample/sample.h"
#include "Menu/Menu.h"
#include "UserFunctions/userFunctions.h"
#include "VUMeter/vumeter.h"

#define NUMBER_OF_TIMERS   (4)

/* Holds functions for the timers and is an example of implementation*/


   
SoftTimer_16  SoftTimer1[TIMER1B_COUNT] = {{10000, 0, 0},
														 {15, 0, 1},
														 {10000, 0, 0}	 };



SoftTimer_16  SoftTimer2[TIMER2B_COUNT] = {{10, 0, 0},
                                           {10, 0, 0}};


interrupt (TIMERB1_VECTOR) timerb1_int(void)
{
   dint();   
   
   uint8_t intVec = TBIV;
   uint8_t i;
   
   if( intVec & TBIV_CCR1 )
   {
		TBCCR1 += SAMPLE_100US;
		
		/* Decay the VU Meters here */
      VULevelDecay(ALL_METERS);
      	
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
         /* Do the VU Meter*/
         
         uint16_t i;
            
         /* Set to Row1, Col 0 */
         VUSetPosition(1,0);  
         
                
         for( i = 0 ; i < ANALOGUE_INPUTS; i++ )
         {

            int8_t Gain =  GetChannelGain(i);
            uint16_t maxVal;
            
            /* if the gain is an attenuation by shifting down
             * 5 bits (ie Most Sig. 7 bits of the 12bit ADC value 
             * then the maximum value that can be used to normalise
             * will be 4095 */
            if( Gain <= (MAX_GAIN + MIN_GAIN) )
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

            /* Normalise with 3 rows */
            VUSetLevel(i, VUNormalise((SignalPeak[i] >> 2), maxVal, 3), 3);
         }
         
         VUMeterPrint(ALL_METERS, 3);
         SoftTimerReset(SoftTimer2[SC_VUMeterUpdate]);     
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
