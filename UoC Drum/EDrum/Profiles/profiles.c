
#include <io.h>
#include <mspgcc/flash.h>
#include <string.h>
#include "profiles.h"


Profile_t CurrentProfile = { 
/* MIDISettings_t */
{	
	/* 15ms output rate */
	15,
	DEFAULT_BAUD_RATE,
	/* MIDI Channel Instrument # */
	(0x09 | MIDI_NOTE_ON)
},

/* Channel Settings */
{
	/* Channel Status */
	0x00031FFF,
	
	/* Analogue Channel MIDI output Codes */
	{MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON,
	 MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON,
	 MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON,
	 MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON,
	 
	 /* Digital Channel MIDI output Codes */
	 MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON,
	 MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON,
	 
	 /* Metronome Channel MIDI output Codes */
	 MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON,
	 MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON, MIDI_NOTE_ON,
	 
	 /* Keypad MIDI output codes */
	 MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE,
	 MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE,
	 MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE,
	 MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE},
	 
	
	/* Default Open Keys*/
//Analogue Inputs	
	{OPEN_HIGH_HAT, LONG_WHISTLE, ELECTRIC_SNARE, SIDE_STICK,
    HI_MID_TOM, LOW_TOM , LOW_FLOOR_TOM, CRASH_CYMBAL1,
	 CRASH_CYMBAL2, SPLASH_CYMBAL, RIDE_CYMBAL2, RIDE_CYMBAL1,
    RIDE_BELL, NO_ITEM, NO_ITEM, NO_ITEM,
//Digital inputs     
    PEDAL_HIGH_HAT, BASS_DRUM1, ACOUSTIC_BASS_DRUM, NO_ITEM, 
    NO_ITEM, NO_ITEM, NO_ITEM, NO_ITEM,

//Metronome inputs 
    BASS_DRUM1, CLOSED_HI_HAT, MUTE_TRIANGLE, ELECTRIC_SNARE,
    RIDE_CYMBAL1, ACOUSTIC_BASS_DRUM, OPEN_TRIANGLE, SIDE_STICK,
//Keypad Inputs
	 REVERB_LEVEL, TREMOLO_LEVEL, CHORUS_LEVEL, CELESTE_LEVEL,
    GENERAL_PURPOSE_BUTTON1, GENERAL_PURPOSE_BUTTON2, GENERAL_PURPOSE_BUTTON3, GENERAL_PURPOSE_BUTTON4,
    GENERAL_PURPOSE_BUTTON5, GENERAL_PURPOSE_BUTTON6, GENERAL_PURPOSE_BUTTON7, GENERAL_PURPOSE_BUTTON8,
    ALL_SOUND_OFF, HOLD_PEDAL_TOGGLE, PORTAMENTO_TOGGLE, LEGATO_PEDAL_TOGGLE},
   
            
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
	 DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER,
  	 DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER,
	 DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER, DEFAULT_RETRIGGER},
	 
	 /* Has Dual Input */
	 0x0003,
	 
	 /* The Digital Trigger */
	 {0x00000000,
	  0x00000000},
	  
	/* Default Closed Keys*/
	{CLOSED_HI_HAT, SHORT_WHISTLE, NO_ITEM, RIDE_CYMBAL2,
    HIGH_FLOOR_TOM, LOW_FLOOR_TOM, LOW_MID_TOM, LOW_TOM,
    NO_ITEM, NO_ITEM, CRASH_CYMBAL2, NO_ITEM,
    NO_ITEM, NO_ITEM, NO_ITEM, NO_ITEM}
},

/* Gain Settings */
{
	/* Default Gains */                                     
	{DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,
    DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,
    DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,
    DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN},
    
	/* Default 2nd Slope Gains */                                     
	{DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,
    DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,
    DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,
    DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN,DEFAULT_GAIN},
	 
	/* Crossover Level */                                     
	{300,300,300,300,
    300,300,300,300,
    300,300,300,300,
    300,300,300,300},
	 
	 /* Gain Type (Linear or Non Linear) */
	 0x0000
},

/* Digital Channel Settings */
{

	/* Hit Velocities */
	{DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY,
    DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY,
    DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY,
    DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY,
	 DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY,
    DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY,
	 DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY,
    DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY, DEFAULT_VELOCITY},
    
   /* All swtiches are Active low type */
   0x00,
   
   /* All switches use single shot triggering */
   0x00
},



/* Sensor Settings */
{ SENSOR_OUTPUT2, DEFAULT_CROSSTALK }

};


/* Sets up the clock with MCLK*/
void ProfileInit(void)
{
   /* Divide 8MHZ by 20 as clk range must be within 257 -> 457kHz*/
   FCTL2 = FWKEY | FSSEL_MCLK | 0x1F;
   
   MIDISettings = &CurrentProfile.midiSettings;
   ChannelSettings = &CurrentProfile.channelSettings;
   DigitalSettings = &CurrentProfile.digitalSettings;
   GainSettings = &CurrentProfile.gainSettings;
   SensorSettings = &CurrentProfile.sensorSettings;
   
   /* Setup profile pointer */
   /*CurrentProfile.channelSettings = &ChannelSettings;
   CurrentProfile.midiSettings = &MIDISettings;
   CurrentProfile.digitalSettings = &DigitalSettings;	
   CurrentProfile.sensorSettings = &SensorSettings;
   CurrentProfile.gainSettings = &GainSettings;*/
   
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
void Profile_Write(Profile_t* profile, uint8_t profileIndex)
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
			
         flash_write((uint16_t*)(PROFILE(i)), 
                     profile, 
                     sizeof(Profile_t));
				      
      }
      else
      {
         flash_write((uint16_t*)(PROFILE(i)), 
                     (uint16_t*)(IMAGE_PROFILE(i)), 
                     sizeof(Profile_t));
                   
      }
   }
}

/* Reads the passed profileIndex into the settings */
void Profile_Read(uint8_t profileIndex)
{
	memcpy((uint16_t*)&CurrentProfile,
		    (uint16_t*)PROFILE(profileIndex), sizeof(Profile_t));
			 	  		 
}