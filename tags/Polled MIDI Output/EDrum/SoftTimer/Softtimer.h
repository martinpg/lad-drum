#ifndef _SOFTTIMERS_H
#define _SOFTTIMERS_H


#define SOFTTIMER_ENABLED (1)
#define SOFTTIMER_DISABLED (0)

typedef struct _softTimer8
{
	uint8_t timeCompare;
	uint8_t timerCounter;
	uint8_t timerEnable;
	
} SoftTimer_8;


typedef struct _softTimer16
{
	uint16_t timeCompare;
	uint16_t timerCounter;
	uint8_t timerEnable;	
} SoftTimer_16;

typedef struct _softTimer32
{
	uint32_t timeCompare;
	uint32_t timerCounter;
	uint8_t timerEnable;	
} SoftTimer_32;

/* 1 for increment and 0 for decrement (decrement is faster?) */
#define timerUpdate  0

#if timerUpdate == 1
   #define SoftTimerInc(x)          x.timerCounter++
   #define SoftTimerReset(x)         x.timerCounter = 0
   #define SoftTimerInterrupt(x)    (x.timerCounter >= x.timeCompare && x.timerEnable)
#else
   #define SoftTimerDec(x)          x.timerCounter--
   #define SoftTimerReset(x)        x.timerCounter = x.timeCompare
   #define SoftTimerInterrupt(x)    (x.timerEnable && (!x.timerCounter--))
#endif




#define SoftTimerStart(x)        x.timerEnable = SOFTTIMER_ENABLED; SoftTimerReset(x)
#define SoftTimerStop(x)         x.timerEnable = SOFTTIMER_DISABLED


/* 10us Resolution */
#define SAMPLE_10US    (F_CPU / 100000)

/* 100us Resolution */
#define SAMPLE_100US (F_CPU / 10000)

/* 1ms Resoultion @ 8MHz*/
#define SAMPLE_1MS   (F_CPU / 1000)

/* CCR1B associated timers */
enum {  

   SC_SecondDelay = 0,
   SC_MIDIOutput,
   SC_RetriggerReset,
   TIMER1B_COUNT 
   
} timer1BIds;

/* CCR2B associated timers */
enum {  

   SC_AutoMenuUpdate = 0,
   SC_VUMeterUpdate,
   SC_DigitalVUUpdate,
   SC_VUDecay,
   SC_AboutUpdate,
   /* LCD_BL_Period should be the last T2 SoftTimer */
   SC_LCD_BL_Period,
   TIMER2B_COUNT 
   
} timer2BIds;

extern SoftTimer_16  SoftTimer1[];
extern SoftTimer_16  SoftTimer2[];

uint8_t SoftTimer_IsTimer2Active(void);

#endif
