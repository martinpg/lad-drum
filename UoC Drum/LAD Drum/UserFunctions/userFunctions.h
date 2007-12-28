#ifndef USERFUNCTIONS_H
#define USERFUNCTIONS_H

#include "Profiles/profiles.h"

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

#endif
