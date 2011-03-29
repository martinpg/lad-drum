#ifndef _TIMER_CALLBACKS_H
#define _TIMER_CALLBACKS_H


#include "SoftTimer/Softtimer.h"

/* CCR1B associated timers */
enum {  
   SC_MIDIScan = 0,
   TIMER1_COUNT 
   
} timer1Ids;

/* CCR2B associated timers */
enum {  
   SC_OneSecond = 0,
   SC_usbPoll,
   SC_AutoMenuUpdate,
   SC_VUMeterUpdate,
   SC_DigitalVUUpdate,
   SC_VUDecay,
   SC_AboutUpdate,
   /* LCD_BL_Period should be the last T2 SoftTimer */
   SC_LCD_BL_Period,
   SC_MonitorChannel,
   SC_Keypress,
   TIMER2_COUNT 
   
} timer2Ids;

extern volatile SoftTimer_8  SoftTimer1[];
extern volatile SoftTimer_16  SoftTimer2[];

#define KP_WAIT          (3)
#define KP_REPEAT_RATE   (3)
#define KP_REPEAT_RATE_FAST   (2)
#define KP_REPEAT_RATE_FASTEST   (1)
#define KP_REPEAT_DELAY  (50)

#define DEFAULT_AUTO_MENU_UPDATE (15)

/* Critical timers */
void Callback_MIDIScan(void);

/* Auxiliary Timers */
void Callback_OneSecond(void);
void Callback_AutoMenuUpdate(void);
void Callback_VUDecay(void);
void Callback_VUMeterUpdate(void);
void Callback_DigitalVUUpdate(void);
void Callback_AboutUpdate(void);
void Callback_LCDBacklight(void);
void Callback_MonitorChannel(void);
void Callback_Keypress(void);

#endif
