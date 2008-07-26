#ifndef VUMETER_H
#define VUMETER_H


#include "UI_LCD/UI_LCD.h"

#define VUPrint(char) UI_LCD_Char(&PrimaryDisplay, char)
#define VUPosition(row, col) UI_LCD_Pos(&PrimaryDisplay, row, col)


/* To print all columns */
#define ALL_METERS   (0xFF)
#define FULL_RANGE   (4)
#define PIXELS_PER_ROW  (8)
/* For a 20x4 LCD Module */
#define MAX_METERS  (20)

#define MAX_ROWS	(4)
#define FULL_BLOCK	(0xFF)
extern uint16_t	VUValues[];


void VUSetLevel(uint8_t meterIndex, uint8_t normalValue, uint8_t rows);

void UpdateVUValues(void* src);
void ResetVUValues(void);

uint16_t VUNormalise(uint16_t value, uint16_t maxVal, uint16_t rows);
uint16_t VUNormaliseMIDI(uint16_t value, uint16_t rows);


void VUMeterPrint(uint8_t column, uint8_t rows );
void VUSetPosition(uint8_t row, uint8_t column);
void VUNewLine(void);

void VULevelDecay(uint8_t meterIndex);


void VUSetRows(uint8_t rows);
uint8_t GetVURows(void);


#endif
