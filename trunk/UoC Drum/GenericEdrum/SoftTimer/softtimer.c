

#include <stdint.h>
#include <stdlib.h>

#include "hardwareSpecific.h"
#include "Softtimer.h"
#include "TimerCallbacks/TimerCallbacks.h"


#define NUMBER_OF_TIMERS   (4)

/* Holds functions for the timers and is an example of implementation*/




void RunCriticalTimer(void)
{
   uint8_t i;
   for( i = 0; i < TIMER1_COUNT; i++)
   {
      /* MIDI output is in 1ms steps */
   	if(SoftTimerInterrupt(SoftTimer1[i]))
   	{
         SoftTimerReset(SoftTimer1[i]);
         SoftTimerCallback(SoftTimer1[i]);
      }	   
   }
}



void RunAuxTimers(void)
{
   uint8_t i;
   for( i = 0; i < TIMER2_COUNT; i++)
   {
      /* MIDI output is in 1ms steps */
   	if(SoftTimerInterrupt(SoftTimer2[i]))
   	{
         SoftTimerReset(SoftTimer2[i]);
         SoftTimerCallback(SoftTimer2[i]);
      }	   
   }
}


uint8_t SoftTimer_IsTimer2Active(void)
{
   uint8_t i;
   
   for( i = 0; i < TIMER2_COUNT; i++)
   {
      if( SoftTimer2[i].timerEnable )
      {
         return 1;  
      }
   }
   return 0;
}
