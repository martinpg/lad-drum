#ifndef USERFUNCTIONS_H
#define USERFUNCTIONS_H

#define GET (0xFF)
enum {
	MAIN_SCREEN = 0,
	CREATORS_SCREEN,
	THANKS_SCREEN,
	THANKS2_SCREEN,
	THANKS3_SCREEN,
	THANKS4_SCREEN,
	INSPIRATION_SCREEN,
	INSPIRATION2_SCREEN,
	INFORMATION_SCREEN,
	INFORMATION2_SCREEN,
	SIZEOFTHANKS
};

#include "Profiles/profiles.h"


void reset(void* data);
void about(void* data);
void aboutScroll(uint8_t nameIndex);
/* Pass GET to obtain the index, otherwise the index is set to the passed
 * parameter */
uint8_t ThanksIndex(uint8_t mode);

/* Play mode disables TimerB2 */
void PlayMode(void* data);

/* Saves the passed profile into the profileIndex */
void SaveProfile(void* data);
void LoadProfile(void* data);
void ShowProfile(void* data);



void SetMIDIRate(void* data);
void EditMIDIRate(void* data);
void PrintMIDIRate(void);


void ChannelSetup(void* data);

void SetThreshold(void* data);
void SetRetrigger(void* data);
void SetGainCurves(void* data);


void ThresholdBar(void);
void lcdProgressBar(uint16_t progress, uint16_t maxprogress, uint8_t length);
void SetDualInput(void* data);


void ChangeChannelCode(void* data);


/** Function to setup each individual digial channel */
void DigitalChannelSettings(void* data);
void SetSwitchType(void* data);

/* Amplifer Input Select */
void AmpInputSelect(void* data);
void SensorInputChange(void* data);

/* VU Meter */
void VUMeterSetup(void* data);

#endif
