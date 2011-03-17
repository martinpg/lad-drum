#ifndef _SOFTTIMERS_H
#define _SOFTTIMERS_H


#define SOFTTIMER_ENABLED (1)
#define SOFTTIMER_DISABLED (0)

typedef struct _softTimer8
{
	uint8_t timeCompare;
	uint8_t timerCounter;
	uint8_t timerEnable;
	void (*timerCallback)(void);
} SoftTimer_8;


typedef struct _softTimer16
{
	uint16_t timeCompare;
	uint16_t timerCounter;
	uint8_t timerEnable;	
	void (*timerCallback)(void);
} SoftTimer_16;

typedef struct _softTimer32
{
	uint32_t timeCompare;
	uint32_t timerCounter;
	uint8_t timerEnable;	
	void (*timerCallback)(void);
} SoftTimer_32;


#define SoftTimerDec(x)          x.timerCounter--
#define SoftTimerReset(x)        x.timerCounter = x.timeCompare
#define SoftTimerInterrupt(x)    (x.timerEnable && (!x.timerCounter--))
#define SoftTimerCallback(x)     if(x.timerCallback) (void)(x.timerCallback())


#define SoftTimerStart(x)        x.timerEnable = SOFTTIMER_ENABLED; SoftTimerReset(x)
#define SoftTimerStop(x)         x.timerEnable = SOFTTIMER_DISABLED
#define SoftTimerIsEnabled(x)    (x.timerEnable)

/* 10us Resolution */
#define SAMPLE_10US    (F_CPU / 100000)

/* 100us Resolution */
#define SAMPLE_100US (F_CPU / 10000)

/* 1ms Resoultion @ 8MHz*/
#define SAMPLE_1MS   (F_CPU / 1000) / (1024)


uint8_t SoftTimer_IsTimer2Active(void);
void RunCriticalTimer(void);
void RunAuxTimers(void);

#endif
