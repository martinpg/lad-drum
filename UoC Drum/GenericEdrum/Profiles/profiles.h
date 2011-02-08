#ifndef PROFILES_H
#define PROFILES_H

#include <stdint.h>
#include "hardwareSpecific.h"
#include "MIDI/midi.h"
#include "Sample/sample.h"
#include "Sensor/sensor.h"


#define NUMBER_OF_PROFILES   (5)
#define FLASH_BLOCK_SIZE      (512)
#define SEGMENTS_TO_USE       ((NUMBER_OF_PROFILES * sizeof(Profile_t) / FLASH_BLOCK_SIZE) + 1)
/* Start after the Device Interrupts! */
#define FLASH_END             (0xFE00)

#define TEMPORARY_FLASH_BUFFER (FLASH_END - (FLASH_BLOCK_SIZE))


#define PROFILE_FLASH_START (uint16_t)(TEMPORARY_FLASH_BUFFER - (SEGMENTS_TO_USE*FLASH_BLOCK_SIZE))
//#define PROFILE_IMAGE_START (uint16_t)(TEMPORARY_FLASH_BUFFER - (SEGMENTS_TO_USE*FLASH_BLOCK_SIZE))


#define PROFILE_FLASH_ADDRESS(x)  (PROFILE_FLASH_START + (FLASH_BLOCK_SIZE*x))
#define PROFILE_IMAGE_ADDRESS(x)  (PROFILE_IMAGE_START + (FLASH_BLOCK_SIZE*x))

#define PROFILE(x)    		(PROFILE_FLASH_ADDRESS(0) + (sizeof(Profile_t)*x))
//#define IMAGE_PROFILE(x)	(PROFILE_IMAGE_ADDRESS(0) + (sizeof(Profile_t)*x))

#define MIDI_SETTINGS(profile)		PROFILE(profile)
#define CHANNEL_SETTINGS(profile)	(MIDI_SETTINGS(profile)+sizeof(MidiSettings_t))
#define GAIN_SETTINGS(profile)		(CHANNEL_SETTINGS(profile)+sizeof(ChannelSettings_t))
#define DIGITAL_SETTINGS(profile)	(GAIN_SETTINGS(profile)+sizeof(GainSettings_t))
#define SENSOR_SETTINGS(profile) 	(DIGITAL_SETTINGS(profile) + sizeof(DigitalSettings_t))

/*#define IMAGE_MIDI_SETTINGS(profile)		IMAGE_PROFILE(profile)
#define IMAGE_CHANNEL_SETTINGS(profile)	(IMAGE_MIDI_SETTINGS(profile)+sizeof(MidiSettings_t))
#define IMAGE_GAIN_SETTINGS(profile)		(IMAGE_CHANNEL_SETTINGS(profile)+sizeof(ChannelSettings_t))
#define IMAGE_DIGITAL_SETTINGS(profile)	(IMAGE_GAIN_SETTINGS(profile)+sizeof(GainSettings_t))
#define IMAGE_SENSOR_SETTINGS(profile) 	(IMAGE_DIGITAL_SETTINGS(profile) + sizeof(DigitalSettings_t))*/




enum {
	PROFILE_1 = 0,
	PROFILE_2 = 1,
	PROFILE_3 = 2,
	PROFILE_4 = 3,
	DEFAULT_PROFILE = 4
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

void Profile_Erase(uint8_t profileIndex);

/* Writes profile data to the profile Index */
void Profile_Write(Profile_t* profile, uint8_t profileIndex);

void Profile_Copy(void);




/* Reads the passed profileIndex into the profile */
void Profile_Read(uint8_t profileIndex);

/* Export Local settings to the current profile
void Profile_ExportLocal(void);
void Profile_LoadLocal(void);*/


#endif
