#ifndef _TIMER_CALLBACKS_H
#define _TIMER_CALLBACKS_H


#include "SoftTimer/Softtimer.h"

/* CCR1B associated timers */
enum {  
   SC_MIDIOutput = 0,
   SC_RetriggerReset,
   TIMER1_COUNT 
   
} timer1Ids;

/* CCR2B associated timers */
enum {  

   SC_AutoMenuUpdate = 0,
   SC_VUMeterUpdate,
   SC_DigitalVUUpdate,
   SC_VUDecay,
   SC_AboutUpdate,
   /* LCD_BL_Period should be the last T2 SoftTimer */
   SC_LCD_BL_Period,
   SC_Debug,
   TIMER2_COUNT 
   
} timer2Ids;

extern SoftTimer_16  SoftTimer1[];
extern SoftTimer_16  SoftTimer2[];

/* Critical timers */
void Callback_MIDIOutput(void);
void Callback_RetriggerReset(void);


/* Auxiliary Timers */
void Callback_AutoMenuUpdate(void);
void Callback_VUDecay(void);
void Callback_VUMeterUpdate(void);
void Callback_DigitalVUUpdate(void);
void Callback_AboutUpdate(void);
void Callback_LCDBacklight(void);
void Callback_Debug(void);

#endif
