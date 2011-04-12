#ifndef GAINADJUSTFUNCTIONS_H
#define GAINADJUSTFUNCTIONS_H

#include "Menu/menu.h"
#include "Profiles/profiles.h"
#include "hardwareSpecific.h"

void GainSlopeAdjustFunction(void* data);
uint8_t setPresetSetting(uint8_t newPreset);
void PrintGainInformation(uint8_t UpDownPosition);
void SetGainCurves(void* data);
void GainAdjustFunction(void* data);
void GainTypeAdjustFunction(void* data);
void GainCrossOverAdjustFunction(void* data);
void GainPresetAdjustFunction(void* data);

#endif
