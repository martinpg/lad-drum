/* Has the Channel sampling code in it */

#include <io.h>
#include <stdint.h>
#include "sample.h"
#include "MIDI/midi.h"
#include "Profiles/profiles.h"
#include "SoftTimer/Softtimer.h"

uint16_t SignalPeak[NUMBER_OF_INPUTS];
/* For Retrigger Timers */
SoftTimer_8   RetriggerPeriod[NUMBER_OF_INPUTS];

/* The functions below all work on the 'current profile */

ChannelSettings_t ChannelSettings = {

	/* Channel Status */
	0x0000000F,
	
	/* Default Open Keys*/
	{0x42, 0x28, 0x53, 0x34,
    0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30},
   
            
	/* Default Thresholds */
	{DEFAULT_THRESHOLD, DEFAULT_THRESHOLD, DEFAULT_THRESHOLD, DEFAULT_THRESHOLD, 
	 DEFAULT_THRESHOLD, DEFAULT_THRESHOLD, DEFAULT_THRESHOLD, DEFAULT_THRESHOLD,
	 DEFAULT_THRESHOLD, DEFAULT_THRESHOLD, DEFAULT_THRESHOLD, DEFAULT_THRESHOLD,
	 DEFAULT_THRESHOLD, DEFAULT_THRESHOLD, DEFAULT_THRESHOLD, DEFAULT_THRESHOLD},
	 
	{DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER,
	 DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER,
	 DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER,
	 DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER,
 	 DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER,
	 DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER},
	 
	 /* Has Dual Input */
	 0x0000,
	 
	 /* The Digital Trigger */
	 {0x00000000,
	  0x00000000},
	  
	/* Default Closed Keys*/
	{0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30}	  
};


DigitalSettings_t DigitalSettings = {

	/* Hit Velocities */
	{0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64},
    
   /* All swtiches are Active low type */
   0x00,
   
   /* All switches use single shot triggering */
   0x00
	
	
};

GainSettings_t GainSettings = {
	/* Default Gains */                                     
	{3,3,3,3,
    3,3,3,3,
    3,3,3,3,
    3,3,3,3},
    
	/* Default 2nd Slope Gains */                                     
	{3,3,3,3,
    3,3,3,3,
    3,3,3,3,
    3,3,3,3},
	 
	/* Crossover Level */                                     
	{300,300,300,300,
    300,300,300,300,
    300,300,300,300,
    300,300,300,300},
	 
	 /* Gain Type (Linear or Non Linear) */
	 0x0000
};



uint8_t GetChannelStatus(uint8_t channel)
{
   return ((ChannelSettings.ChannelStatus & (1 << channel)) != 0);
}


void ChannelToggle(uint8_t channel)
{
   SetChannelStatus( channel, !GetChannelStatus(channel) );
}

void SetChannelStatus(uint8_t channel, uint8_t status)
{
   ChannelSettings.ChannelStatus &=  ~(1 << channel);
   ChannelSettings.ChannelStatus |=   (status << channel);
}

/* General Channel Key Notes, and also the Dual Input Open Notes */
uint8_t GetChannelKey(uint8_t channel)
{
   return ChannelSettings.ChannelKey[channel];
}

void SetChannelKey(uint8_t channel, int16_t key)
{
   if( key > MIDI_MAX_KEY)
   {
      key = 0;  
   }
   if( key < 0)
   {
      key = MIDI_MAX_KEY;  
   }   

   ChannelSettings.ChannelKey[channel] = key;
}

/* Close Channel Key Notes for Dual Input*/
uint8_t GetChannelKeyClosed(uint8_t channel)
{
   return ChannelSettings.ClosedChannelKey[channel];
}

void SetChannelKeyClosed(uint8_t channel, int16_t key)
{
   if( key > MIDI_MAX_KEY)
   {
      key = 0;  
   }
   if( key < 0)
   {
      key = MIDI_MAX_KEY;  
   }   

   ChannelSettings.ClosedChannelKey[channel] = key;
}



/* Channel Threshold */
uint16_t GetChannelThresh(uint8_t channel)
{
   return ChannelSettings.ChannelThreshold[channel];
}

void SetChannelThresh(uint8_t channel, uint16_t thresh)
{
   if( thresh > MAX_THRESHOLD)
   {
      thresh = MAX_THRESHOLD;  
   }
   ChannelSettings.ChannelThreshold[channel] = thresh + MIN_THRESHOLD;
}

/* Setup Channel Retrigger times */
uint8_t GetChannelReTrig(uint8_t channel)
{
   return ChannelSettings.ChannelRetrigger[channel];
}

void SetChannelReTrig(uint8_t channel, int16_t retrig)
{
   if( retrig > MAX_RETRIGGER)
   {
      retrig = 0;  
   }
   if( retrig < 0)
   {
		retrig = MAX_RETRIGGER;	
	}   
   ChannelSettings.ChannelRetrigger[channel] = retrig + MIN_RETRIGGER;
}

/* Setup Analogue Dual Inputs */
uint8_t GetDualMode(uint8_t channel)
{
   return ((ChannelSettings.HasDualInput & (1 << channel)) != 0);
}


void DualModeToggle(uint8_t channel)
{
   SetDualMode( channel, !GetDualMode(channel) );
}

void SetDualMode(uint8_t channel, uint8_t dualInputMode)
{
   ChannelSettings.HasDualInput &=  ~(1 << channel);
   ChannelSettings.HasDualInput |=   (dualInputMode << channel);
}

/* Dual Input digital triggering map */
/* The channel passed is the Analogue channel */
uint8_t GetDigitalTrigger(uint8_t AnalogueChannel)
{
	if( AnalogueChannel < 8 )
	{
   	return (ChannelSettings.AnalogueTrigger[0] & 
				 (0x0F << (AnalogueChannel << 2)  )) >> (AnalogueChannel << 2);		
	}
	else
	{
		return (ChannelSettings.AnalogueTrigger[1] & 
		 		 (0x0F << ((AnalogueChannel-8) << 2))) >> (AnalogueChannel << 2);
	}
}

void SetDigitalTrigger(uint8_t AnalogueChannel, int8_t DigitalChannel)
{
	if( DigitalChannel >= DIGITAL_INPUTS)
	{
		DigitalChannel = 0;		
	}
	
	if( DigitalChannel < 0 )
	{
		DigitalChannel = DIGITAL_INPUTS - 1;	
	}
	
	
	if( AnalogueChannel < ANALOGUE_INPUTS )
	{
		if( AnalogueChannel < 8 )
		{
	   	ChannelSettings.AnalogueTrigger[0] &=  ~(0x0F << (AnalogueChannel << 2));
			ChannelSettings.AnalogueTrigger[0] |=  (DigitalChannel << (AnalogueChannel << 2));			
		}
		else
		{
	   	ChannelSettings.AnalogueTrigger[1] &=  ~(0x0F << ((AnalogueChannel-8) << 2));
			ChannelSettings.AnalogueTrigger[1] |=  (DigitalChannel << ((AnalogueChannel-8) << 2));
		}
		
	}
}




/* Digital Channel Velocity Outputs */
uint8_t GetDigitalVelocity(uint8_t channel)
{
   return DigitalSettings.DigitalVelocity[channel];
}

void SetDigitalVelocity(uint8_t channel, int16_t velocity)
{
   if( velocity > MAX_VELOCITY)
   {
      velocity = 0;  
   }
   if( velocity < 0)
   {
		velocity = MAX_VELOCITY;	
	}
   DigitalSettings.DigitalVelocity[channel] = velocity;
}

/* To alter the switch type from Active Low/High */
uint8_t GetActiveState(uint8_t DigitalChannel)
{
   return ((DigitalSettings.DigitalActiveState & (1 << DigitalChannel)) != 0);
}

void ActiveStateToggle(uint8_t DigitalChannel)
{
   SetActiveState( DigitalChannel, !GetActiveState(DigitalChannel) );
}

void SetActiveState(uint8_t DigitalChannel, uint8_t activeState)
{
   DigitalSettings.DigitalActiveState &=  ~(1 << DigitalChannel);
   DigitalSettings.DigitalActiveState |=   (activeState << DigitalChannel);
}


/* Trigger mode is either Single shot (needs to reset before next retrigger)
 * or continuous is triggering while switch is in active state. */
uint8_t GetTriggerMode(uint8_t DigitalChannel)
{
   return ((DigitalSettings.DigitalTriggerMode & (1 << DigitalChannel)) != 0);
}

void TriggerModeToggle(uint8_t DigitalChannel)
{
   SetTriggerMode( DigitalChannel, !GetTriggerMode(DigitalChannel) );
}

void SetTriggerMode(uint8_t DigitalChannel, uint8_t triggerMode)
{
   DigitalSettings.DigitalTriggerMode &=  ~(1 << DigitalChannel);
   DigitalSettings.DigitalTriggerMode |=   (triggerMode << DigitalChannel);
}


/* Initiate changes */
void UpdateChannelRetriggers(void)
{
   uint8_t i;
   /* Update the Retrigger periods */
   for( i = 0; i < NUMBER_OF_INPUTS; i++ )
   {
      RetriggerPeriod[i].timeCompare = GetChannelReTrig(i);
      SoftTimerReset(RetriggerPeriod[i]);
   }
}





/****** GAIN SETTINGS ******/
/* Channel Gain 1st Slope*/
int8_t GetChannelGain(uint8_t channel)
{
   return GainSettings.ChannelGain[channel];
}

void SetChannelGain(uint8_t channel, int8_t Gain)
{
   if( Gain > MAX_GAIN)
   {
      Gain = MIN_GAIN;  
   }
   if( Gain < MIN_GAIN)
   {
      Gain = MAX_GAIN;  
   }   

   GainSettings.ChannelGain[channel] = Gain;
}


/* Channel Gain 2nd Slope*/
int8_t GetSlope2Gain(uint8_t channel)
{
   return GainSettings.ChannelGain2[channel];
}

void SetSlope2Gain(uint8_t channel, int8_t Gain)
{
   if( Gain > MAX_GAIN)
   {
      Gain = MIN_GAIN;  
   }
   if( Gain < MIN_GAIN)
   {
      Gain = MAX_GAIN;  
   }   

   GainSettings.ChannelGain2[channel] = Gain;
}



/* Gain Type setup */
uint8_t GetGainType(uint8_t channel)
{
   return ((GainSettings.GainType & (1 << channel)) != 0);
}


void GainTypeToggle(uint8_t channel)
{
   SetChannelStatus( channel, !GetChannelStatus(channel) );
}

void SetGainType(uint8_t channel, uint8_t status)
{
   GainSettings.GainType &=  ~(1 << channel);
   GainSettings.GainType |=   (status << channel);
}


/* Gain Crossover Levels */
uint16_t GetCrossover(uint8_t channel)
{
   return GainSettings.Crossover[channel];
}

void SetCrossover(uint8_t channel, uint16_t crossover)
{
   if( crossover > MAX_CROSSOVER)
   {
      crossover = MAX_CROSSOVER;  
   }
   GainSettings.Crossover[channel] = crossover;
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
   if( sample < ChannelSettings.ChannelThreshold[channel] || 
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
   
   
   /* Need to setup the timer specs */

   
}




