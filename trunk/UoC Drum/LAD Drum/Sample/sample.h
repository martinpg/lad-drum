#ifndef  SAMPLE_H
#define  SAMPLE_H

#include "Profiles/profiles.h"
#include "SoftTimer/Softtimer.h"

#define CH0        (0)
#define CH1        (1)
#define CH2        (2)
#define CH3        (3)
#define CH4        (4)
#define CH5        (5)
#define CH6        (6)
#define CH7        (7)
#define CH8        (8)
#define CH9        (9)
#define CH10       (10)
#define CH11       (11)
#define CH12       (12)
#define CH13       (13)
#define CH14       (14)
#define CH15       (15)


/* The number of bits in the ADC Reading */
#define MAX_GAIN           (12)
#define MAX_THRESHOLD      (4095)
#define DEFAULT_THRESHOLD	(15)
#define MIN_THRESHOLD      (0)
#define THRESHOLD_LEVELS	(6)

/* Retrigger Defines */
#define MAX_RETRIGGER      (200)
#define DEFAULT_RETRIGGER	(15)
#define MIN_RETRIGGER         (0)

#define CHANNEL_ON   (1)
#define CHANNEL_OFF  (0)


extern uint16_t SignalPeak[];
extern Profile_t CurrentProfile;
extern SoftTimer_8   RetriggerPeriod[];


void SampleInit(void);
void ResetValues(void);

/* Changes the sampling channel to channel, CH0 -> CH15 */
void SampleChannel(uint8_t channel);
void TimerInit(void);
void ObtainPeak(uint8_t channel, uint16_t sample);

void ChannelToggle(uint8_t channel);
uint8_t GetChannelStatus(uint8_t channel);
void SetChannelStatus(uint8_t channel, uint8_t status);

/* Channel Key */
uint8_t GetChannelKey(uint8_t channel);
void SetChannelKey(uint8_t channel, uint8_t key);

/* Channel Gain */
uint8_t GetChannelGain(uint8_t channel);
void SetChannelGain(uint8_t channel, uint8_t Gain);

/* Channel Thresholds */
uint16_t GetChannelThresh(uint8_t channel);
void SetChannelThresh(uint8_t channel, uint16_t thresh);

/* Channel Retrigger */
uint8_t GetChannelReTrig(uint8_t channel);
void SetChannelReTrig(uint8_t channel, uint8_t retrig);
void UpdateChannelRetriggers(void);


#endif

