

#include <stdint.h>
#include <stdlib.h>

#include "hardwareSpecific.h"
#include "Softtimer.h"
#include "TimerCallbacks/TimerCallbacks.h"


#define NUMBER_OF_TIMERS   (4)

/* Holds functions for the timers and is an example of implementation*/


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
														 {10000, 10000, 0, Callback_LCDBacklight}}; //LCD Backlight



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
