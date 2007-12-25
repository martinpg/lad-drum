/* Has the Channel sampling code in it */

#include <io.h>
#include <stdint.h>
#include "sample.h"
#include "MSB2LSB/MSB2LSB.h"
#include "MIDI/midi.h"
#include "Profiles/profiles.h"
#include "SoftTimer/Softtimer.h"

#define CHSELOUT  P4OUT
#define CHSELDIR  P4DIR
#define CHSELSEL  P4SEL
#define CHSELIN   P4IN

#define CHSELA     (1<<3)
#define CHSELB     (1<<2)
#define CHSELC     (1<<1)
#define CHSELD     (1<<0)

#define CHSELPINS  ( CHSELA | CHSELB | CHSELC | CHSELD )


Profile_t CurrentProfile;
uint16_t SignalPeak[NUMBER_OF_INPUTS];
uint8_t  Retrigger[NUMBER_OF_INPUTS/2];
/* For Retrigger Timers */
SoftTimer_8   RetriggerPeriod[NUMBER_OF_INPUTS];

/* The functions below all work on the 'current profile */

/* Init the ports as low outputs */
void SampleInit(void)
{
   CHSELDIR |=  (CHSELPINS);
   CHSELSEL &= ~(CHSELPINS);
   CHSELOUT &= ~(CHSELPINS);    
}

/* Changes the sampling channel to channel, CH0 -> CH15 */
void SampleChannel(uint8_t channel)
{
   if( channel > CH15 )
   {
      return;    
   }
   
   channel = MSB2LSB(~channel) >> 4;
   CHSELOUT = (CHSELOUT & ~(CHSELPINS)) | channel;
   return;
}





uint8_t GetChannelStatus(uint8_t channel)
{
   if( channel < 8 )
   {
      return ((CurrentProfile.ChannelStatus[0] & (1 << channel)) != 0);
   }
   else
   {
      return ((CurrentProfile.ChannelStatus[1] & (1 << (channel-8))) != 0);     
   }
}


void ChannelToggle(uint8_t channel)
{
   SetChannelStatus( channel, !GetChannelStatus(channel) );
}

void SetChannelStatus(uint8_t channel, uint8_t status)
{
   if( channel < 8 )
   {
      CurrentProfile.ChannelStatus[0] &=  ~(1 << channel);
      CurrentProfile.ChannelStatus[0] |=   (status << channel);
   }
   else
   {
      CurrentProfile.ChannelStatus[1] &= ~(1 << (channel-8));
      CurrentProfile.ChannelStatus[1] |= (status << (channel-8));            
   }
}

uint8_t GetChannelKey(uint8_t channel)
{
   return CurrentProfile.ChannelKey[channel];
}

void SetChannelKey(uint8_t channel, uint8_t key)
{
   if( key > MIDI_MAX_KEY)
   {
      key = MIDI_MAX_KEY;  
   }
   else
   {
      CurrentProfile.ChannelKey[channel] = key;
   }
}

/* Channel Gain */
uint8_t GetChannelGain(uint8_t channel)
{
   return CurrentProfile.ChannelGain[channel];
}

void SetChannelGain(uint8_t channel, uint8_t Gain)
{
   if( Gain > MAX_GAIN)
   {
      Gain = MAX_GAIN;  
   }
   else
   {
      CurrentProfile.ChannelGain[channel] = Gain;
   }
}

/* Channel Threshold */
uint16_t GetChannelThresh(uint8_t channel)
{
   return CurrentProfile.ChannelThreshold[channel];
}

void SetChannelThresh(uint8_t channel, uint16_t thresh)
{
   if( thresh > MAX_THRESHOLD)
   {
      thresh = MAX_THRESHOLD;  
   }
   else
   {
      CurrentProfile.ChannelThreshold[channel] = thresh + MIN_THRESHOLD;
   }
}


uint8_t GetChannelReTrig(uint8_t channel)
{
   return CurrentProfile.ChannelRetrigger[channel];
}

void SetChannelReTrig(uint8_t channel, uint8_t retrig)
{
   if( retrig > MAX_RETRIGGER)
   {
      retrig = MAX_RETRIGGER;  
   }
   else
   {
      CurrentProfile.ChannelRetrigger[channel] = retrig + MIN_RETRIGGER;
   }
}


/* Initiate changes */
void UpdateChannelRetriggers(void)
{
   uint8_t i;
   /* Update the Retrigger periods */
   for( i = 0; i < NUMBER_OF_INPUTS; i++ )
   {
      RetriggerPeriod[i].timeCompare = GetChannelReTrig(i);
   }
}


void ResetValues(void)
{
   uint8_t i;
   for( i = 0; i < NUMBER_OF_INPUTS; i++)
   {
      SignalPeak[i] = 0;  
   }
}

/* Peaks must be larger than the threshold to trigger, else 0
 * The sample is compared against the previous samples of the channel */
/* Channel must be <= CH15 */ 
 
void ObtainPeak(uint8_t channel, uint16_t sample)
{
   if( sample < CurrentProfile.ChannelThreshold[channel] || 
		 sample < SignalPeak[channel] )
   {
      return;  
   }

   /* Update the largest sample */
   SignalPeak[channel] = sample;
}

/* For SMCLK clocked at MCLK = fosc = 8MHz */
void TimerInit(void)
{
   /* No Divider, 16bit operation @ SMCLK rate, activate Interrupts */
   TBCTL |= (TBSSEL_SMCLK | MC_CONT | TBIE);
     
   /* 100us timer */
   TBCCTL1 |= (CCIE);   
   TBCCR1 = SAMPLE_100US;

   /* 1ms Timer */
   TBCCTL2 |= (CCIE);
   TBCCR2 = SAMPLE_1MS;
   
}




