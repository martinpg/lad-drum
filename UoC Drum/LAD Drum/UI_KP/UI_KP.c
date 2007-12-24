
#include <stdint.h>
#include "UI/UI.h"
#include "SoftI2C/softi2c.h"
#include "UI_KP.h"



/* Set all UI_ROWS outputs */
/* UI_COLS to Inputs and ROW0 to output*/
/* Enable Pullups on RTC0/RTC1 */
/* Set P31 to an output and COL0 to an input*/
static const uint8_t KP_INIT[] = {MAX7300_DDRB1, 0x56, 0xA9, 0x6A};

static const uint8_t KP_ROWS2INPUTS[] = {MAX7300_DDRB1, 0xAA, 0xAA};
static const uint8_t KP_COLS2OUTPUTS[] = {MAX7300_DDRB2, 0x56, 0x7D};

void UI_KP_Init(void)
{
   /* All ports are by default inputs with no pullups */

	/* Set all UI_ROWS outputs */
	/* UI_COLS to Inputs and ROW0 to output*/
   /* Set P31 to an output and COL0 to an input*/
   UI_SetBlock(KP_INIT, 4);
   /* Set UI_Rows to to output LOW */
   UI_SetRegister(UI_ROW_PORT, 0x00);
}

uint8_t UI_KP_GetPress(void)
{
   uint8_t ColResult;
   uint8_t RowResult;
   uint8_t KPResult;
   
	ColResult = (UI_ReadRegister(MAX7300_PORTINT) >> 1) & (0x0F);
   
   /* Set rows to inputs with no PULLUP and columns to outputs*/
   UI_SetBlock(KP_ROWS2INPUTS, 3);

   /* Set Columns to outputs */
   UI_SetBlock(KP_COLS2OUTPUTS, 3);
	
	/* Set all columns to Output High and Bring INT0 low*/
	UI_SetRegister(UI_COL_PORT, 0x0F);
   
   /* Get the row result */
	RowResult = ~(UI_ReadRegister(UI_ROW_PORT)) & (0x0F);  

   KPResult = (ColResult << 4) | RowResult;
   
   /* Reset the buttons to original state */
   UI_KP_Init();
   
   return KPResult;
   
}

