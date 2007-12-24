

#include <io.h>
#include <stdint.h>
#include <signal.h>

#include "main.h"
#include "Softtimer.h"
#include "UI/UI.h"
#include "MIDI/midi.h"
#include "Sample/sample.h"
#include "Menu/Menu.h"

#define NUMBER_OF_TIMERS   (4)

/* Holds functions for the timers and is an example of implementation*/


   
SoftTimer_16  SoftTimer1[TIMER1B_COUNT];
SoftTimer_16  SoftTimer2[TIMER2B_COUNT];



interrupt (TIMERB1_VECTOR) timerb1_int(void)
{
   dint();   
   
   uint8_t intVec = TBIV;
   uint8_t i;
   
   switch(intVec)
   {
		case TBIV_CCR1: 	
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
			
	      /* Resets every 100us */
			if(SoftTimerInterrupt(SoftTimer1[SC_MIDIOutput]))
			{
	         MIDI_Output();
	         ResetValues();
	         SoftTimerReset(SoftTimer1[SC_MIDIOutput]);     
         }	    			
	      
	      
	      for( i = 0; i < NUMBER_OF_INPUTS ; i++ )
	      {
            if(SoftTimerInterrupt(RetriggerPeriod[i]))
            {
               SoftTimerStop(RetriggerPeriod[i]);
               SoftTimerReset(RetriggerPeriod[i]);      
            }
         }	
		break;
					
		case TBIV_CCR2:
			TBCCR2 += SAMPLE_1MS;
			if(SoftTimerInterrupt(SoftTimer2[SC_AutoMenuUpdate]))
			{
	         /* Update the Menu */
	         MenuSetInput(0x01);   
	         MenuUpdate();  
	         SoftTimerReset(SoftTimer2[SC_AutoMenuUpdate]);     
         }	    	      
	   break;
	}  
 
   eint();    
}
