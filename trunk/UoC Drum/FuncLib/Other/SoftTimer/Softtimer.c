

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

#define NUMBER_OF_TIMERS   (4)

/* Holds functions for the timers and is an example of implementation*/


   
SoftTimer_16  SoftTimer1[TIMER1B_COUNT] = {{10000, 0, 0},
														 {15, 0, 1},
														 {10000, 0, 0}	 };



SoftTimer_16  SoftTimer2[TIMER2B_COUNT] = {{10, 0, 0}};


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
         /* Update the Menu */
         ThresholdBar();
         SoftTimerReset(SoftTimer2[SC_AutoMenuUpdate]);     
      }
         
      /* RIDI output is in 1ms steps */
		if(SoftTimerInterrupt(SoftTimer1[SC_MIDIOutput]))
		{
         MIDI_Output();
         ResetValues();
         SoftTimerReset(SoftTimer1[SC_MIDIOutput]);     
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
