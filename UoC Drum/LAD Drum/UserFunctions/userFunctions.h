#ifndef USERFUNCTIONS_H
#define USERFUNCTIONS_H

#include "Profiles/profiles.h"

/* Saves the passed profile into the profileIndex */
void SaveProfile(void* data);
void LoadProfile(void* data);
void ShowProfile(void* data);

Profile_t* Profile_Read(uint8_t profileIndex);

void SetMIDIRate(void* data);
void EditMIDIRate(void* data);
void PrintMIDIRate(void);


void ChannelSettings(void* data);

void SetThreshold(void* data);

void lcdProgressBar(uint16_t progress, uint16_t maxprogress, uint8_t length);

void ChangeChannelCode(void* data);

#endif
