/* Sensory and channel control */



#include <stdint.h>
#include "hardwareSpecific.h"
//#include "ADC/adc12.h"
#include "sensor.h"


SensorSettings_t* SensorSettings;


/* Init the ports as low outputs */
void SensorInit(void)
{
   CHSELDIR |=  (CHSELPINS);
//   CHSELSEL &= ~(CHSELPINS);
   CHSELOUT &= ~(CHSELPINS);    
}

/* Changes the sampling channel to channel, CH0 -> CH15 
 * If a valid sample is taken, set the input port to an output and
 * pull it down.
 */
void SensorChannel(uint8_t channel)
{
	
	uint8_t channelState;
	
   if( channel > CHANNEL_COUNT )
   {
      return;    
   }
   
   channel = ~channel;
   
   channelState = (((channel & (0x08)) ? 1 : 0) << CHSELD) |
                  (((channel & (0x04)) ? 1 : 0) << CHSELC) |
                  (((channel & (0x02)) ? 1 : 0) << CHSELB) |
                  (((channel & (0x01)) ? 1 : 0) << CHSELA);

   CHSELOUT = (CHSELOUT & ~(CHSELPINS)) | channelState;
   return;
}

/* New Port must be within INCH_A0 -> INCH_Ax */
void SensorInputSelect(uint8_t newPort)
{
   SensorSettings->SensorInputPort = newPort;
   ADC_SetupAddress(newPort);
}

uint8_t GetSensorInput(void)
{
	return SensorSettings->SensorInputPort;
}



/* Crosstalk controls the delay between changing the channel */
int16_t GetCrossTalkDelay(void)
{
	return SensorSettings->CrosstalkDelay;
}


void SetCrossTalkDelay(int16_t newCrosstalk)
{
	SensorSettings->CrosstalkDelay = newCrosstalk;
}


uint16_t SensorPotValue(void)
{
   ADC_SetupAddress(POT_INPUT);
   uint16_t PotValue = (ADC_Sample());
   ADC_SetupAddress(POT_INPUT); 

	return PotValue;
}
