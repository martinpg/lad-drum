/* Sensory and channel control */


#include <io.h>
#include <stdint.h>
#include "ADC/adc12.h"
#include "MSB2LSB/MSB2LSB.h"
#include "sensor.h"

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

#define CHSELOUT  P4OUT
#define CHSELDIR  P4DIR
#define CHSELSEL  P4SEL
#define CHSELIN   P4IN

#define CHSELA     (1<<3)
#define CHSELB     (1<<2)
#define CHSELC     (1<<1)
#define CHSELD     (1<<0)

#define CHSELPINS  ( CHSELA | CHSELB | CHSELC | CHSELD )







SensorSettings_t SensorSettings = {SENSOR_OUTPUT2};


/* Init the ports as low outputs */
void SensorInit(void)
{
   CHSELDIR |=  (CHSELPINS);
   CHSELSEL &= ~(CHSELPINS);
   CHSELOUT &= ~(CHSELPINS);    
}

/* Changes the sampling channel to channel, CH0 -> CH15 */
void SensorChannel(uint8_t channel)
{
   if( channel > CH15 )
   {
      return;    
   }
   
   channel = MSB2LSB(~channel) >> 4;
   CHSELOUT = (CHSELOUT & ~(CHSELPINS)) | channel;
   return;
}

/* New Port must be within INCH_A0 -> INCH_Ax */
void SensorInputSelect(uint8_t newPort)
{
	SensorSettings.SensorInputPort = newPort;
   ADC12_SetupAddress(0, newPort);
}


uint8_t GetSensorInput(void)
{
	return SensorSettings.SensorInputPort;
}


uint16_t SensorPotValue(void)
{
   ADC12_SetupAddress(0, POT_INPUT);
   uint16_t PotValue = (ADC12_Sample());
   ADC12_SetupAddress(0, INCH_A0); 

	return PotValue;
		
}


