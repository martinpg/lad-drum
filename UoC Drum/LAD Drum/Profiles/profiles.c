
#include <io.h>
#include <mspgcc/flash.h>
#include <string.h>
#include "profiles.h"


ProfilePtr_t CurrentProfile;


/* Sets up the clock with MCLK*/
void ProfileInit(void)
{
   /* Divide 8MHZ by 20 as clk range must be within 257 -> 457kHz*/
   FCTL2 = FWKEY | FSSEL_MCLK | 0x1F;
   
   /* Setup profile pointer */
   CurrentProfile.channelSettings = &ChannelSettings;
   CurrentProfile.midiSettings = &MIDISettings;
   CurrentProfile.digitalSettings = &DigitalSettings;	
   CurrentProfile.sensorSettings = &SensorSettings;
   CurrentProfile.gainSettings = &GainSettings;
   
}


void Profile_Copy(void)
{
   uint16_t i;
   for( i = 0 ; i < SEGMENTS_TO_USE; i++ )
   {
      flash_erase_segment( (uint16_t*)PROFILE_IMAGE_ADDRESS(i)); 
      
      flash_write((uint16_t*)PROFILE_IMAGE_ADDRESS(i), 
                  (uint16_t*)PROFILE_FLASH_ADDRESS(i), 
                   FLASH_BLOCK_SIZE);        
   }				  					             
}


/* Writes profile data to the profile Index,
   Profile Index (0 -> 3) */
void Profile_Write(ProfilePtr_t* profile, uint8_t profileIndex)
{
   uint16_t i;
   
   Profile_Copy();   
   for( i = 0 ; i < SEGMENTS_TO_USE; i++ )
   {
      flash_erase_segment( (uint16_t*)PROFILE_FLASH_ADDRESS(i));
   }	   

   for( i = 0; i < NUMBER_OF_PROFILES; i++ )
   {
      if( i == profileIndex )
      {
			
         flash_write((uint16_t*)(MIDI_SETTINGS(i)), 
                     profile->midiSettings, 
                     sizeof(MidiSettings_t));

         flash_write((uint16_t*)(CHANNEL_SETTINGS(i)), 
                     profile->channelSettings, 
                     sizeof(ChannelSettings_t));   

         flash_write((uint16_t*)(GAIN_SETTINGS(i)), 
                     profile->gainSettings, 
                     sizeof(GainSettings_t)); 			
							
         flash_write((uint16_t*)(DIGITAL_SETTINGS(i)), 
                     profile->digitalSettings, 
                     sizeof(DigitalSettings_t)); 
							
         flash_write((uint16_t*)(SENSOR_SETTINGS(i)), 
                     profile->sensorSettings, 
                     sizeof(SensorSettings_t)); 							  														
							      
      }
      else
      {
         flash_write((uint16_t*)(MIDI_SETTINGS(i)), 
                     (uint16_t*)(IMAGE_MIDI_SETTINGS(i)), 
                     sizeof(MidiSettings_t));

         flash_write((uint16_t*)(CHANNEL_SETTINGS(i)), 
                     (uint16_t*)(IMAGE_CHANNEL_SETTINGS(i)), 
                     sizeof(ChannelSettings_t));   

         flash_write((uint16_t*)(GAIN_SETTINGS(i)), 
                     (uint16_t*)(IMAGE_GAIN_SETTINGS(i)), 
                     sizeof(GainSettings_t));   
							
         flash_write((uint16_t*)(DIGITAL_SETTINGS(i)), 
                     (uint16_t*)(IMAGE_DIGITAL_SETTINGS(i)), 
                     sizeof(DigitalSettings_t));               
                     
         flash_write((uint16_t*)(SENSOR_SETTINGS(i)), 
                     (uint16_t*)(IMAGE_SENSOR_SETTINGS(i)), 
                     sizeof(SensorSettings_t));                      
      }
   }
}

/* Reads the passed profileIndex into the settings */
void Profile_Read(uint8_t profileIndex)
{
	memcpy((uint16_t*)&MIDISettings,
		    (uint16_t*)MIDI_SETTINGS(profileIndex), sizeof(MidiSettings_t));
			 
	memcpy((uint16_t*)&ChannelSettings,
			 (uint16_t*)CHANNEL_SETTINGS(profileIndex), sizeof(ChannelSettings_t));

	memcpy((uint16_t*)&GainSettings,
			 (uint16_t*)GAIN_SETTINGS(profileIndex), sizeof(GainSettings_t));
	       
	memcpy((uint16_t*)&DigitalSettings,
	  		 (uint16_t*)DIGITAL_SETTINGS(profileIndex), sizeof(DigitalSettings_t));	
	  		 
	memcpy((uint16_t*)&SensorSettings,
	  		 (uint16_t*)SENSOR_SETTINGS(profileIndex), sizeof(SensorSettings_t));		  		 
}
