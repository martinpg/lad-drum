#include <string.h>
#include "hardwareSpecific.h"
#include "profiles.h"
#include "MIDICodes/MIDICodes.h"
#include "flashmem/flashmem.h"

Profile_t CurrentProfile; 


/* Sets up the clock with MCLK*/
void ProfileInit(void)
{
   /* Divide 8MHZ by 20 as clk range must be within 257 -> 457kHz*/
   //FCTL2 = FWKEY | FSSEL_MCLK | ((F_CPU / 250000) - 1);
   
   MIDISettings = &CurrentProfile.midiSettings;
   ChannelSettings = &CurrentProfile.channelSettings;
   DigitalSettings = &CurrentProfile.digitalSettings;
   GainSettings = &CurrentProfile.gainSettings;
   SensorSettings = &CurrentProfile.sensorSettings;

}



/* Writes profile data to the profile Index,
   Profile Index (0 -> 3) */
void Profile_Write(Profile_t* profile, uint8_t profileIndex)
{
   void* memPtr = (void*)PROFILE(profileIndex);

#if PROFILE_MEMORY == PROFILE_EEPROM
   eeprom_update_block((void*)profile, memPtr, sizeof(Profile_t));
#else
   flashmem_bufferedWrite( (uint32_t)memPtr, (void*)profile, sizeof(Profile_t), 0 );
#endif

}

/* Reads the passed profileIndex into the settings */
void Profile_Read(uint8_t profileIndex)
{
	PROFILE_COPY((void *)&CurrentProfile,
		          (void *)PROFILE(profileIndex), sizeof(Profile_t));
			 	  		 
}
