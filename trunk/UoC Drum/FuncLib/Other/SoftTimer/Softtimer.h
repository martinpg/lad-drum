#ifndef _SOFTTIMERS_H
#define _SOFTTIMERS_H

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




#define SoftTimerStart(x)        x.timerEnable = 1; SoftTimerReset(x)
#define SoftTimerStop(x)         x.timerEnable = 0


/* 10us Resolution */
#define SAMPLE_10US    (F_CPU / 100000)

/* 100us Resolution */
#define SAMPLE_100US (800)

/* 1ms Resoultion */
#define SAMPLE_1MS   (8000)

/* CCR1B associated timers */
enum {  

   SC_SecondDelay = 0,
   SC_MIDIOutput,
   SC_MIDIFastOutput,
   TIMER1B_COUNT 
   
} timer1BIds;

/* CCR2B associated timers */
enum {  

   SC_AutoMenuUpdate = 0,
   TIMER2B_COUNT 
   
} timer2BIds;

extern SoftTimer_16  SoftTimer1[];
extern SoftTimer_16  SoftTimer2[];

#endif
