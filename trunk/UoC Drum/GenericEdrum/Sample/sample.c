/* Has the Channel sampling code in it */

#include <stdint.h>
#include "hardwareSpecific.h"
#include "sample.h"
#include "MIDI/midi.h"
#include "Profiles/profiles.h"
#include "SoftTimer/Softtimer.h"

#include "mmculib/log2.h"

/* Signal Peak also holds the value of the Digital State */
uint16_t SignalPeak[NUMBER_OF_INPUTS];

/* The Digital Triggers must be cycled for the singleshot triggers */
uint8_t DigitalCycle[DIGITAL_INPUTS];

/* For Retrigger Timers */
SoftTimer_8   RetriggerPeriod[NUMBER_OF_INPUTS];

/* This stores which analogue channels are active, this streamlines the polling process */
uint8_t ActiveChannels[NUMBER_OF_INPUTS+1];

/* The functions below all work on the 'current profile */

ChannelSettings_t* ChannelSettings;

DigitalSettings_t* DigitalSettings;

GainSettings_t* GainSettings;

static uint16_t LastSampleValue[ANALOGUE_INPUTS];

PROGRAM_CHAR PRESET_1[] = "Exponential X";
PROGRAM_CHAR PRESET_2[] = "Exponential";
PROGRAM_CHAR PRESET_3[] = "Logorithmic";
PROGRAM_CHAR PRESET_4[] = "Logorithmic X";
PROGRAM_CHAR PRESET_5[] = "Custom";

PROGRAM_PTR PresetGainStrings[] = {PRESET_1, PRESET_2, PRESET_3, PRESET_4, PRESET_5};


#define EXPONENTIALX_CROSSOVER      (((1 << ADC_RESOLUTION)*24)/30)
#define EXPONENTIAL_CROSSOVER       (((1 << ADC_RESOLUTION)*17)/30)
#define LOGARITHMIC_CROSSOVER       (((1 << ADC_RESOLUTION)*10)/30)
#define LOGARITHMIC_X_CROSSOVER     (((1 << ADC_RESOLUTION)*6)/30)

const int8_t PresetGain1[] = {5, 5 , 8, 9};
const int8_t PresetGain2[] = {9, 8 , 6, 5};

const uint16_t PresetGainCrossover[] = 
{
   EXPONENTIALX_CROSSOVER, 
   EXPONENTIAL_CROSSOVER,
   LOGARITHMIC_CROSSOVER, 
   LOGARITHMIC_X_CROSSOVER
};


void UpdateActiveChannels(void)
{
   uint8_t i;
   uint8_t ChannelIndex = 0;
   
   for( i = 0; i < ANALOGUE_INPUTS; i++)
   {
      if( ((ChannelSettings->ChannelStatus) & ((uint32_t)1 << i)) != 0 )
      {
         ActiveChannels[ChannelIndex++] = i;
      }
   }
   
   ActiveChannels[ChannelIndex] = LAST_CHANNEL_INDEX;
}


uint8_t GetChannelStatus(uint8_t channel)
{
   return ((ChannelSettings->ChannelStatus & ((uint32_t)1 << channel)) != 0);
}


void ChannelToggle(uint8_t channel)
{
   SetChannelStatus( channel, !GetChannelStatus(channel) );
}

/* This is the only place where channel settings can be adjusted */
void SetChannelStatus(uint8_t channel, uint8_t status)
{
   ChannelSettings->ChannelStatus &=  ~((uint32_t)1 << channel);
   ChannelSettings->ChannelStatus |=   ((uint32_t)status << channel);
}

/* Channel Codes  */
uint8_t GetChannelCommand(uint8_t channel)
{
   return ChannelSettings->ChannelCommand[channel];
}

void SetChannelCommand(uint8_t channel, uint8_t command)
{
   if( command >= MIDI_SYSEX_START)
   {
      command = MIDI_NOTE_OFF;  
   }
   if( command < MIDI_NOTE_OFF)
   {
      command = MIDI_NOTE_OFF;  
   }   

   ChannelSettings->ChannelCommand[channel] = command;
}


/* General Channel Key Notes, and also the Dual Input Open Notes */
uint8_t GetChannelKey(uint8_t channel)
{
   return ChannelSettings->ChannelKey[channel];
}

void SetChannelKey(uint8_t channel, int8_t key)
{
	/* Coming from 127 + 10 will be less than -50 */
   if( key < -50)
   {
      key = 0;  
   }
   /* zero - 15 will be less than 0, but not less than -50 */
   if( key < 0)
   {
      key = MIDI_MAX_KEY;  
   }   

   ChannelSettings->ChannelKey[channel] = key;
}

/* Close Channel Key Notes for Dual Input*/
uint8_t GetChannelKeyClosed(uint8_t channel)
{
   return ChannelSettings->ClosedChannelKey[channel];
}

void SetChannelKeyClosed(uint8_t channel, int8_t key)
{
   if( key < -50)
   {
      key = 0;  
   }
   if( key < 0)
   {
      key = MIDI_MAX_KEY;  
   }   

   ChannelSettings->ClosedChannelKey[channel] = key;
}



/* Channel Threshold */
uint16_t GetChannelThresh(uint8_t channel)
{
   return ChannelSettings->ChannelThreshold[channel];
}

void SetChannelThresh(uint8_t channel, int16_t thresh)
{
   if( thresh > MAX_THRESHOLD)
   {
      thresh = MIN_THRESHOLD;  
   }
   if( thresh < MIN_THRESHOLD )
   {
		thresh = MAX_THRESHOLD;	
	}
   ChannelSettings->ChannelThreshold[channel] = thresh;
}

/* Setup Channel Retrigger times */
uint8_t GetChannelReTrig(uint8_t channel)
{
   return ChannelSettings->ChannelRetrigger[channel];
}

void SetChannelReTrig(uint8_t channel, int16_t retrig)
{
   if( retrig > MAX_RETRIGGER)
   {
      retrig = MIN_RETRIGGER;  
   }
   if( retrig < MIN_RETRIGGER)
   {
		retrig = MAX_RETRIGGER;	
	}   
   ChannelSettings->ChannelRetrigger[channel] = retrig + MIN_RETRIGGER;
}

/* Setup Analogue Dual Inputs */
uint8_t GetDualMode(uint8_t channel)
{
   return ((ChannelSettings->HasDualInput & (1 << channel)) != 0);
}


void DualModeToggle(uint8_t channel)
{
   SetDualMode( channel, !GetDualMode(channel) );
}

void SetDualMode(uint8_t channel, uint8_t dualInputMode)
{
   ChannelSettings->HasDualInput &=  ~(1 << channel);
   ChannelSettings->HasDualInput |=   (dualInputMode << channel);
}

/* Dual Input digital triggering map */
/* The channel passed is the Analogue channel */
uint8_t GetDigitalTrigger(uint8_t AnalogueChannel)
{
	if( AnalogueChannel < 8 )
	{
   	return (ChannelSettings->AnalogueTrigger[0] & 
				 (0x0F << (AnalogueChannel << 2)  )) >> (AnalogueChannel << 2);		
	}
	else
	{
		return (ChannelSettings->AnalogueTrigger[1] & 
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
	   	ChannelSettings->AnalogueTrigger[0] &=  ~(0x0F << (AnalogueChannel << 2));
			ChannelSettings->AnalogueTrigger[0] |=  (DigitalChannel << (AnalogueChannel << 2));			
		}
		else
		{
	   	ChannelSettings->AnalogueTrigger[1] &=  ~(0x0F << ((AnalogueChannel-8) << 2));
			ChannelSettings->AnalogueTrigger[1] |=  (DigitalChannel << ((AnalogueChannel-8) << 2));
		}
		
	}
}




/* Digital Channel Velocity Outputs */
uint8_t GetDigitalVelocity(uint8_t DigitalChannel)
{
   return DigitalSettings->DigitalVelocity[DigitalChannel];
}

void SetDigitalVelocity(uint8_t DigitalChannel, int8_t velocity)
{
	
	if( velocity < -50)
	{
		velocity = 0;	
	}
   if( velocity < 0)
   {
		velocity = MAX_VELOCITY;	
	}

   DigitalSettings->DigitalVelocity[DigitalChannel] = velocity;
}

/* To alter the switch type from Active Low/High */
uint8_t GetActiveState(uint8_t DigitalChannel)
{
   return ((DigitalSettings->DigitalActiveState & (1 << DigitalChannel)) != 0);
}

void ActiveStateToggle(uint8_t DigitalChannel)
{
   SetActiveState( DigitalChannel, !GetActiveState(DigitalChannel) );
}

void SetActiveState(uint8_t DigitalChannel, uint8_t activeState)
{
   DigitalSettings->DigitalActiveState &=  ~(1 << DigitalChannel);
   DigitalSettings->DigitalActiveState |=   (activeState << DigitalChannel);
}


/* Trigger mode is either Single shot (needs to reset before next retrigger)
 * or continuous is triggering while switch is in active state. */
uint8_t GetTriggerMode(uint8_t DigitalChannel)
{
   return ((DigitalSettings->DigitalTriggerMode & (1 << DigitalChannel)) != 0);
}

void TriggerModeToggle(uint8_t DigitalChannel)
{
   SetTriggerMode( DigitalChannel, !GetTriggerMode(DigitalChannel) );
}

void SetTriggerMode(uint8_t DigitalChannel, uint8_t triggerMode)
{
   DigitalSettings->DigitalTriggerMode &=  ~(1 << DigitalChannel);
   DigitalSettings->DigitalTriggerMode |=   (triggerMode << DigitalChannel);
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
   return GainSettings->ChannelGain[channel];
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

   GainSettings->ChannelGain[channel] = Gain;
}


/* Channel Gain 2nd Slope*/
int8_t GetSlope2Gain(uint8_t channel)
{
   return GainSettings->ChannelGain2[channel];
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

   GainSettings->ChannelGain2[channel] = Gain;
}



/* Gain Type setup */
uint8_t GetGainType(uint8_t channel)
{
   return ((GainSettings->GainType & ((uint16_t)(1) << channel)) != 0);
}


void GainTypeToggle(uint8_t channel)
{
   SetGainType( channel, !GetGainType(channel) );
}

void SetGainType(uint8_t channel, uint8_t status)
{
   GainSettings->GainType &=  ~((uint16_t)1 << channel);
   GainSettings->GainType |=   ((uint16_t)status << channel);
}


/* Gain Crossover Levels */
int16_t GetCrossover(uint8_t channel)
{
   return GainSettings->Crossover[channel];
}

void SetCrossover(uint8_t channel, int16_t crossover)
{
   if( crossover > MAX_CROSSOVER)
   {
      crossover = MIN_CROSSOVER;  
   }
   if( crossover < MIN_CROSSOVER)
   {
		crossover = MAX_CROSSOVER;	
	}
   
   GainSettings->Crossover[channel] = crossover;
}

/* Returns the conditioned signal after being passed through the
 * configured gain settings */
uint16_t GainFunction(uint8_t channel, uint16_t signalValue)
{
	if( GetGainType(channel) == NON_LINEAR_GAIN )
	{
		/* Signal > Crossover */
		uint16_t crossover = GetCrossover(channel);
		
      int16_t signalOffset = signalValue - crossover;
		if( (signalOffset > 0) )
		{
			return ApplyGain(signalOffset , GetSlope2Gain(channel)) +
					 ApplyGain(crossover, GetChannelGain(channel));	
		}	
	}

	return ApplyGain(signalValue, GetChannelGain(channel));		
}

/* The signal is multiplied by 2^ (gain - ADCBitResolution) */
uint16_t ApplyGain(uint16_t signalValue, int8_t gain)
{
   int8_t gainToApply = gain - GAIN_OFFSET;
   
	if( (gainToApply) > 0 )
	{
		signalValue = signalValue << gainToApply;
	}
	else
	{
		signalValue = signalValue >> (-gainToApply);	
	}
	
	return signalValue;
	
}


/* Updates the SignalPeak Variable regardless of whether the Digital Input is
 * Active */
void ScanDigitalInputs(void)
{
	uint8_t i;
	
	for( i = 0; i < DIGITAL_INPUTS; i++)
	{
		if( GetDigitalState(i) == GetActiveState(i) )
		{
			if( GetTriggerMode(i) == SINGLE_SHOT )
			{
				/* Schmitt Trigger Type Operation */
				if( DigitalCycle[i] == INPUT_HAS_BEEN_CYCLED )
				{
					SignalPeak[i + ANALOGUE_INPUTS] = 1;
					DigitalCycle[i] = INPUT_IS_DOWN;	
				}
				else
				{
					/* Reset Value */
					SignalPeak[i + ANALOGUE_INPUTS] = 0;						
				}
			}
			else
			{
				SignalPeak[i + ANALOGUE_INPUTS] = 1;
			}
		}
		else
		{
			/* Reset Value */
			SignalPeak[i + ANALOGUE_INPUTS] = 0;
			DigitalCycle[i] = INPUT_HAS_BEEN_CYCLED;
		}

	}
	
}

void SetLastSampleValue(uint8_t channel, uint16_t value)
{
		/* Update the last sample */
   LastSampleValue[channel] = value;
}

uint16_t GetLastSampleValue(uint8_t channel)
{
	return LastSampleValue[channel];	
}

void ResetValues(void)
{
   uint8_t i;
   for( i = 0; i < ANALOGUE_INPUTS; i++)
   {
      SignalPeak[i] = 0;  
   }
}

/* Peaks must be larger than the threshold to trigger, else 0
 * The sample is compared against the previous samples of the channel */
/* Channel must be <= CH15 */ 
 
void ObtainPeak(uint8_t channel, uint16_t sample)
{
   if( sample < ChannelSettings->ChannelThreshold[channel] || 
		 sample < SignalPeak[channel] )
   {
      return;  
   }

   /* Update the largest sample */
   SignalPeak[channel] = sample;
}







