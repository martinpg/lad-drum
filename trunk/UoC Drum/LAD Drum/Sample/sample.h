#ifndef  SAMPLE_H
#define  SAMPLE_H

#include "SoftTimer/Softtimer.h"



#define ANALOGUE_INPUTS	(16)
#define DIGITAL_INPUTS	(8)
#define NUMBER_OF_INPUTS   (ANALOGUE_INPUTS + DIGITAL_INPUTS)

/* The number of bits in the ADC Reading */
#define MAX_THRESHOLD      (4095)
#define DEFAULT_THRESHOLD	(15)
#define MIN_THRESHOLD      (0)
#define THRESHOLD_LEVELS	(6)

#define MAX_VELOCITY			(127)

/* Retrigger Defines */
#define MAX_RETRIGGER      (255)
#define DEFAULT_RETRIGGER	(1)
#define MIN_RETRIGGER         (0)

#define CHANNEL_ON   (1)
#define CHANNEL_OFF  (0)

#define ACTIVE_HIGH	(1)
#define ACTIVE_LOW	(0)

#define SINGLE_SHOT	(0)
#define CONTINUOUS	(1)

/* Dual Input Defines */
#define HAS_DUAL_INPUT	(1)
#define NO_DUAL_INPUT	(0)

/* Gain Curve Defines */
#define LINEAR_GAIN		(0)
#define NON_LINEAR_GAIN	(1)

/* Amplification */
#define MAX_GAIN           (7)
/* Attenuation */
#define MIN_GAIN				(-12)
#define MAX_CROSSOVER	(4095)



#define DIGITAL_PIN1	P5IN
#define DIGITAL_PIN2	P4IN

#define DIGITAL_PORT1	P5OUT
#define DIGITAL_PORT2	P4OUT

#define DIGITAL_DDR1		P5DIR
#define DIGITAL_DDR2		P4DIR

enum {
	D0 = 0,
	D1,
	D2,
	D3,
	D4,
	D5,
	D6,
	D7	
} digitalPort1;

/* In Digital Port 1 */
#define DIGITAL_0		(1<<0)
#define DIGITAL_1		(1<<1)
#define DIGITAL_2		(1<<2)
#define DIGITAL_3		(1<<3)
#define DIGITAL_4		(1<<4)

/* In Digital Port 2 */
#define DIGITAL_5		(1<<5)
#define DIGITAL_6		(1<<6)
#define DIGITAL_7		(1<<7)
#define INPUT_HAS_BEEN_CYCLED	(0)
#define INPUT_IS_DOWN (1)

typedef struct {

	/* For both Analogue and Digital Channels */
   uint32_t  ChannelStatus;
   
   /* For Open notes, Digital Inputs have only Open notes */
   uint8_t   ChannelKey[NUMBER_OF_INPUTS];

   uint16_t  ChannelThreshold[ANALOGUE_INPUTS];
   
   /* in 10ms resolution for both Analogue and Digital Settings */
   uint8_t	 ChannelRetrigger[NUMBER_OF_INPUTS];
   
   /* Associating an analogue input with a digital one */
   uint16_t	 HasDualInput;
   /* Each analogue channel occupies a Nibble and corresponds to the 
    * digital input which activates the closed note */
   uint32_t	 AnalogueTrigger[ANALOGUE_INPUTS/8];
   /* For the 'closed' notes */
   uint8_t	 ClosedChannelKey[ANALOGUE_INPUTS];   

	
} ChannelSettings_t;


typedef struct {

	/* Slope 1 Gradient */
   int8_t   ChannelGain[ANALOGUE_INPUTS];	
   /* Slope 2 Gradient */
   int8_t	ChannelGain2[ANALOGUE_INPUTS];
   
   /* Crossover Level from Gain 1 to Gain 2*/
   uint16_t  Crossover[ANALOGUE_INPUTS];
	
	uint16_t   GainType;
	
} GainSettings_t;

typedef struct {

   /* The velocity of the digital outputs is constant */
   uint8_t	 DigitalVelocity[DIGITAL_INPUTS];
   
   /* Sets the input to either active high/low */
   uint8_t 	 DigitalActiveState;
   /* Either single shot (needs to be reset) or continuous */
   uint8_t	 DigitalTriggerMode;
	
} DigitalSettings_t;



extern uint16_t SignalPeak[];
extern SoftTimer_8   RetriggerPeriod[];
extern uint8_t DigitalCycle[];

extern ChannelSettings_t ChannelSettings;
extern DigitalSettings_t DigitalSettings;
extern GainSettings_t	 GainSettings;

void ResetValues(void);


void TimerInit(void);
void ObtainPeak(uint8_t channel, uint16_t sample);

void ChannelToggle(uint8_t channel);
uint8_t GetChannelStatus(uint8_t channel);
void SetChannelStatus(uint8_t channel, uint8_t status);

/* Channel Key */
uint8_t GetChannelKey(uint8_t channel);
void SetChannelKey(uint8_t channel, int16_t key);

/* Closed Channel Keys */
uint8_t GetChannelKeyClosed(uint8_t channel);
void SetChannelKeyClosed(uint8_t channel, int16_t key);

/* Channel Thresholds */
uint16_t GetChannelThresh(uint8_t channel);
void SetChannelThresh(uint8_t channel, int16_t thresh);

/* Channel Retrigger */
uint8_t GetChannelReTrig(uint8_t channel);
void SetChannelReTrig(uint8_t channel, int16_t retrig);
void UpdateChannelRetriggers(void);

/* Dual Input */
uint8_t GetDualMode(uint8_t channel);
void DualModeToggle(uint8_t channel);
void SetDualMode(uint8_t channel, uint8_t dualInputMode);


/* The channel passed is the Analogue channel */
uint8_t GetDigitalTrigger(uint8_t AnalogueChannel);
void SetDigitalTrigger(uint8_t AnalogueChannel, int8_t DigitalChannel);


uint8_t GetDigitalVelocity(uint8_t DigitalChannel);
void SetDigitalVelocity(uint8_t DigitalChannel, int16_t velocity);


/* Switch Type */
uint8_t GetActiveState(uint8_t DigitalChannel);
void ActiveStateToggle(uint8_t DigitalChannel);
void SetActiveState(uint8_t DigitalChannel, uint8_t activeState);


/* Trigger mode is either Single shot (needs to reset before next retrigger)
 * or continuous is triggering while switch is in active state. */
uint8_t GetTriggerMode(uint8_t DigitalChannel);
void TriggerModeToggle(uint8_t DigitalChannel);
void SetTriggerMode(uint8_t DigitalChannel, uint8_t triggerMode);



/* Channel Gain */
int8_t GetChannelGain(uint8_t channel);
void SetChannelGain(uint8_t channel, int8_t Gain);

/* Channel Gain 2nd Slope*/
int8_t GetSlope2Gain(uint8_t channel);
void SetSlope2Gain(uint8_t channel, int8_t Gain);

/* Gain Type setup */
uint8_t GetGainType(uint8_t channel);
void GainTypeToggle(uint8_t channel);
void SetGainType(uint8_t channel, uint8_t status);

/* Gain Settings */
uint16_t GetCrossover(uint8_t channel);
void SetCrossover(uint8_t channel, uint16_t crossover);

void DigitalInputInit(void);
uint8_t GetDigitalState(uint8_t DigitalChannel);
void ScanDigitalInputs(void);


#endif

