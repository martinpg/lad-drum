#ifndef PROFILES_H
#define PROFILES_H

#include <stdint.h>

#define PROFILE(x)    (PROFILE_FLASH_ADDRESS + (sizeof(Profile_t)*x))
#define NUMBER_OF_PROFILES   (4)
#define PROFILE_FLASH_ADDRESS (0xFA00)
#define PROFILE_IMAGE_ADDRESS (0xFC00)
#define FLASH_BLOCK_SIZE      (512)
#define NUMBER_OF_INPUTS   16
#define DEFAULT_BAUD_RATE	BAUD_31250


enum {
	PROFILE_1 = 0,
	PROFILE_2,
	PROFILE_3,
	PROFILE_4,
	DEFAULT_PROFILE
} profileIds;

typedef struct {

   /* In 100us resolution */
   uint16_t  MIDI_OutputRate;
   uint16_t  MIDI_BaudRate;
   uint16_t	 MIDI_ChannelCode;

   uint8_t   ChannelStatus[NUMBER_OF_INPUTS/8];
   uint8_t   ChannelKey[NUMBER_OF_INPUTS];
   uint8_t   ChannelGain[NUMBER_OF_INPUTS];
   uint16_t  ChannelThreshold[NUMBER_OF_INPUTS];
   
   /* in 100us resolution */
   uint8_t	 ChannelRetrigger[NUMBER_OF_INPUTS];
   
} Profile_t;


extern const Profile_t Default_Profile;

void ProfileInit(void);

void Profile_Erase(uint8_t profileIndex);

/* Writes profile data to the profile Index */
void Profile_Write(Profile_t* profile, uint8_t profileIndex);

void Profile_Copy(void);


void Flash_Erase(void* dst);
void Flash_write(void *dst, const void *src, unsigned int size);
void write_Seg(void *dst, char value);

Profile_t* Profile_Read(uint8_t profileIndex);

#endif
