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

#define SoftTimerInc(x)          x.timerCounter++
#define SoftTimerReset(x)        x.timerCounter = 0
#define SoftTimerInterrupt(x)    (x.timerCounter >= x.timeCompare && x.timerEnable)
#define SoftTimerStart(x)        x.timerEnable = 1
#define SoftTimerStop(x)         x.timerEnable = 0


#endif
