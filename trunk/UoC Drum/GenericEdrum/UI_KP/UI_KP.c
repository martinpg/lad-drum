
#include <stdint.h>
#include "UI_KP/UI_KP.h"
#include "hardwareSpecific.h"

static const uint8_t KP_ButtonIndex[] = {RAW_KP_0, RAW_KP_1, RAW_KP_2, RAW_KP_3, 
                                  RAW_KP_4, RAW_KP_5, RAW_KP_6, RAW_KP_7, 
                                  RAW_KP_8, RAW_KP_9, RAW_KP_A, RAW_KP_B,
                                  RAW_KP_C, RAW_KP_D, RAW_KP_STAR, RAW_KP_HASH};



void UI_KP_Init(void)
{  
   /* Columns as outputs */  
   UI_COL_DIR |= (UI_COLS);
   UI_COL_OUT |= (UI_COLS);

   UI_ROW_DIR &= ~(UI_ROWS);
   UI_ROW_OUT &= ~(UI_ROWS); 
}

uint8_t UI_KP_GetPress(void)
{
   uint8_t ColResult;
   uint8_t RowResult;
   uint8_t KPResult;

   if( UI_ROW_IN & UI_ROWS )
   {
      RowResult = (UI_ROW_IN & UI_ROWS);
      RowResult = (RowResult << 1);
   }

   /* Set Columns to inputs */
   UI_COL_DIR &= ~(UI_COLS);
   UI_COL_OUT &= ~(UI_COLS);
   UI_ROW_DIR |= (UI_ROWS);
   UI_ROW_OUT |= (UI_ROWS);  
   
   _delay_us(32);

   if( UI_COL_IN & UI_COLS )
   {
      ColResult = (UI_COL_IN & UI_COLS);
   }
   else
   {
      /* We assume one column */
      ColResult = (1 << 3);
   }

   KPResult = (ColResult) | RowResult;
   /* Reset the buttons to original state */
   UI_KP_Init();
   
   return UI_KP_ReturnID(KPResult);
   
}



uint8_t UI_KP_ReturnID(uint8_t KP_Button)
{
	uint8_t i;
	
	for( i = 0; i < KP_ButtonCount; i++)
	{
		if( KP_Button == KP_ButtonIndex[i] )
		{
			return i;	
		}
	}
	
	return KP_INVALID;
	
}









