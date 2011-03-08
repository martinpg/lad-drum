#ifndef PROFILES_H
#define PROFILES_H

#include <stdint.h>

#include "hardwareSpecific.h"
#include "MIDI/midi.h"
#include "Sample/sample.h"
#include "Sensor/sensor.h"


#define SEGMENTS_TO_USE       (((NUMBER_OF_PROFILES) * sizeof(Profile_t) / FLASH_BLOCK_SIZE) + 1)
/* Start after the Device Interrupts! */
// For devices which use FLASH memory, it's to be placed at the end of flash 
#if PROFILE_MEMORY == PROFILE_EEPROM
   #define PROFILE_START ((NUMBER_OF_PROFILES) * sizeof(Profile_t))
   #define PROFILE_LOCATION(x) EEMEM
   #define PROFILE(x)    		(sizeof(Profile_t)*(x))
#else
   #define PROFILE_START (FLASH_TEMP_BUFFER - (SEGMENTS_TO_USE*FLASH_BLOCK_SIZE)) 
   #define PROFILE_LOCATION(x) SET_SECTION(".PROFILE_"#x)
   #define PROFILE(x)    		(FLASH_TEMP_BUFFER - (sizeof(Profile_t)*(x+1)))
#endif



#define MIDI_SETTINGS(profile)		PROFILE(profile)
#define CHANNEL_SETTINGS(profile)	(MIDI_SETTINGS(profile)+sizeof(MidiSettings_t))
#define GAIN_SETTINGS(profile)		(CHANNEL_SETTINGS(profile)+sizeof(ChannelSettings_t))
#define DIGITAL_SETTINGS(profile)	(GAIN_SETTINGS(profile)+sizeof(GainSettings_t))
#define SENSOR_SETTINGS(profile) 	(DIGITAL_SETTINGS(profile) + sizeof(DigitalSettings_t))


/* define either EEMEM or PROGRAM_SPACE */
/*
#define PROFILE_MEMORY  PROFILE_EEPROM
#if PROFILE_MEMORY == PROFILE_EEPROM
   #define PROFILE_SPACE   EEMEM
   #define PROFILE_COPY(dest, src, len) eeprom_read_block(dest, src, len)
#else
   #define PROFILE_SPACE   PROGRAM_SPACE
   #define PROFILE_COPY(dest, src, len) memcpy_P(dest, src, len)
#endif
*/

enum {
   DEFAULT_PROFILE = 0,
	PROFILE_1 = 1,
	PROFILE_2 = 2,
	PROFILE_3 = 3,
	PROFILE_4 = 4
} profileIds;

typedef struct {

	MidiSettings_t midiSettings;
	ChannelSettings_t channelSettings;
   GainSettings_t gainSettings;	
	DigitalSettings_t digitalSettings;
   SensorSettings_t sensorSettings;
   
} Profile_t;


typedef struct {

	MidiSettings_t* midiSettings;
	ChannelSettings_t* channelSettings;
   GainSettings_t* gainSettings;	
	DigitalSettings_t* digitalSettings;
   SensorSettings_t* sensorSettings;  
	
} ProfilePtr_t;

extern Profile_t CurrentProfile;

void ProfileInit(void);

#define page_write(address, buffer, len) _page_write(address, buffer, len, 0)
#define page_write_P(address, buffer, len) _page_write(address, buffer, len, 1)

void Profile_Erase(uint8_t profileIndex);

/* Writes profile data to the profile Index */
void Profile_Write(Profile_t* profile, uint8_t profileIndex);

void Profile_Copy(void);




/* Reads the passed profileIndex into the profile */
void Profile_Read(uint8_t profileIndex);

/* Export Local setting s to the current profile
void Profile_ExportLocal(void);
void Profile_LoadLocal(void);*/


#endif
