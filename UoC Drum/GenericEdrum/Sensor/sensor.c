/* Sensory and channel control */



#include <stdint.h>
#include "hardwareSpecific.h"
#include "ADC/adc12.h"
#include "sensor.h"
#include "MSB2LSB/MSB2LSB.h"





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
	
   if( channel > CH15 )
   {
      return;    
   }
   
   
   
   channel = ~channel;
   
   channelState = ((channel & (0x08)) >> 3) | ((channel & (0x04)) >> 1) | ((channel & (0x02)) << 1) | ((channel & (0x01)) << 3);
   CHSELOUT = (CHSELOUT & ~(CHSELPINS)) | channelState;
   return;
}

/* New Port must be within INCH_A0 -> INCH_Ax */
void SensorInputSelect(uint8_t newPort)
{
   SensorSettings->SensorInputPort = newPort;
   ADC12_SetupAddress(0, newPort);
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
   ADC12_SetupAddress(0, POT_INPUT);
   uint16_t PotValue = (ADC12_Sample());
   ADC12_SetupAddress(0, POT_INPUT); 

	return PotValue;
		
}


