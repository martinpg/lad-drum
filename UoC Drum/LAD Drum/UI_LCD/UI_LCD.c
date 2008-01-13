#include "UI/UI.h"
#include "MAX7300/max7300.h"
#include "PWM/pwm.h"
#include "UI_LCD.h"
#include "MSB2LSB/MSB2LSB.h"
#include "Delay/delay.h"

#define LCD_BL_MAX   65000

static uint8_t UI_LCD_PowerStatus = LCD_OFF;
static uint8_t UI_LCD_RSStatus = UI_LCD_RS_INSTRUCTION;




const uint8_t LcdCustomChar[][8] =
{
	{0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00}, // 0. 0/5 full progress block
	{0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00}, // 1. 1/5 full progress block
	{0x00, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x00}, // 2. 2/5 full progress block
	{0x00, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, 0x00}, // 3. 3/5 full progress block
	{0x00, 0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x00}, // 4. 4/5 full progress block
	{0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00}, // 5. 5/5 full progress block
	{0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00, 0x00}, // 6. Heart
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F}, // 7. Vertical 1/8 progress  
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F}, // 8. 2/8
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F}, // 9. 3/8
	{0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F}, // 10. Vertical 4/8 progress  
	{0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, // 11. 5/8
	{0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, // 12. 6/8 Vertical progress
	{0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, // 13. 7/8 Vertical progress	
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x0A, 0x04}, // 14. Down Arrow
};






/* Internal Functions */


void UI_LCD_SetInstruction(void)
{
   UI_LCD_RSStatus = UI_LCD_RS_INSTRUCTION;
}
   

void UI_LCD_SetData(void)
{
   UI_LCD_RSStatus = UI_LCD_RS_DATA;  
}


/* Use a wrapper for the UI_MAX7300 interface to ensure LCD_Power is enabled
 * if any write commands are used */
void UI_LCD_SetRegister(uint8_t reg, uint8_t data)
{

#if VERSION_CODE == VERSION_WITH_PE
   if( reg == UI_LCD_PORT )
   {   
		data = MSB2LSB(data) >> 4;		
      data &= ~(1 << UI_LCD_PRS);
      data |= (1 << UI_LCD_PPOWER) | (UI_LCD_RSStatus << UI_LCD_PRS); 
   }
   UI_SetRegister(reg, data);   
#endif

#if VERSION_CODE == VERSION_WITHOUT_PE	
	data = MSB2LSB(data) >> 4;
	UI_LCD_CONTROL_PORT &= ~(UI_LCD_RS);
	UI_LCD_CONTROL_PORT |= (UI_LCD_RSStatus << UI_LCD_PRS);
	 
   UI_LCD_DATA_PORT &= ~(UI_LCD_DATA);
	UI_LCD_DATA_PORT |= data;  
#endif   
}



/* Setup all of the UI_MAX7300 LCD Pins as an Output */
void UI_LCD_HWInit(void)
{
#if VERSION_CODE == VERSION_WITH_PE
   uint8_t LCD_INIT[] = {MAX7300_DDRA1, 0x55, 0xA5};
   /* All ports are by default inputs with no pullups */      

	/* Set all UI_LCD pins to outputs */
	/* UI_LCD Pins P12 - P15 as Outputs */	
	/* UI_LCD pins P16-P17 to Outputs leave P18&19 as Input*/
   UI_SetBlock(LCD_INIT, 3);

   /* Set UI_LCD pins to to output LOW */
	UI_LCD_SetRegister(UI_LCD_PORT, 0x00);   
#endif

#if VERSION_CODE == VERSION_WITHOUT_PE
	/* Make all pins outputs */
   UI_LCD_DATA_DIR |= (UI_LCD_DATA);
   UI_LCD_CONTROL_DIR |= (UI_LCD_CONTROL);   

	/* Bring all lines low */
	UI_LCD_DATA_PORT &= ~(UI_LCD_DATA);
	UI_LCD_CONTROL_PORT &= ~(UI_LCD_CONTROL);

	/* LCD BL as an output */
	LCD_BL_DDR |= (1 << LCD_BL_PIN);
#endif
	
}

void UI_LCD_Strobe(void)
{  
#if VERSION_CODE == VERSION_WITH_PE	
	UI_LCD_SetRegister(UI_LCD_E, 0x01);   
	UI_LCD_SetRegister(UI_LCD_E, 0x00);    
#endif

#if VERSION_CODE == VERSION_WITHOUT_PE	
	UI_LCD_CONTROL_PORT |= UI_LCD_E;
	UI_LCD_CONTROL_PORT &= ~UI_LCD_E; 
#endif
}

#if VERSION_CODE == VERSION_WITH_PE	
/* Shutdown and Activate only used in special cases (ie. not edrum) */
void UI_LCD_Activate(void)
{
   UI_LCD_PowerStatus = LCD_ON;
   /* Set UI_LCD pins to to output LOW except for LCD Power which is high*/
	UI_SetRegister(UI_LCD_POWER, 0x01);    
   /* Need to wait 40ms after applying power */
   _delay_ms(40);
}

void UI_LCD_Shutdown(void)
{
   /* Set UI_LCD pins to to output LOW */
	UI_SetRegister(UI_LCD_PORT, 0x00);   
   UI_SetRegister(UI_LCD_POWER, 0x00); 
   UI_LCD_PowerStatus = LCD_OFF;
}
#endif

/* Assumes 4-bit Mode, 4 MSB are sent first */
void UI_LCD_Write(char code)
{
	UI_LCD_SetRegister(UI_LCD_PORT, (code >> 4) & (0x0F) );
   UI_LCD_Strobe();
	UI_LCD_SetRegister(UI_LCD_PORT, (code) & (0x0F) );   
   UI_LCD_Strobe();
   _delay_us(40);
}

void  UI_LCD_Char(char data)
{
   UI_LCD_SetData();
   UI_LCD_Write(data);
}




/* Clears the screen, sets up the LCD to accept commands */
void UI_LCD_Init(void)
{

   UI_LCD_SetInstruction();
   UI_LCD_Strobe();
	/* Set to 8 - bit mode */
	UI_LCD_SetRegister(UI_LCD_PORT, ((LCD_FUNCTION_DEFAULT) >> 4) & (0x0F) );
   UI_LCD_Strobe();
	_delay_ms(5);
	
   UI_LCD_Write( LCD_FUNCTION_DEFAULT );
   _delay_ms(2); 	   
   
   UI_LCD_Write( LCD_DISPLAY_DEFAULT );
   _delay_ms(2);   
   UI_LCD_Write( LCD_MODE_DEFAULT );   
   UI_LCD_Clear();
   _delay_ms(2);    
   UI_LCD_Home();
   _delay_ms(2); 
	
	UI_LCD_LoadDefaultChars();

}




/* Public Functions */
void UI_LCD_Home(void)
{
   UI_LCD_SetInstruction();   
   UI_LCD_Write( (1 << LCD_HOME ) );   
   _delay_ms(2);   
}


void UI_LCD_Clear(void)
{
   UI_LCD_SetInstruction();   
   UI_LCD_Write( (1 << LCD_CLR ) );
   _delay_ms(2);
}

/* Prints a string to the LCD at the current position 
 * If the line overflows, the data continues to the next address */
void UI_LCD_String(char* string)
{
   UI_LCD_SetData();
   
   while( *string )
   {
      UI_LCD_Write(*string++);      
   }
}

/* New Lines are done by using UI_LCD_Pos( row+1, 0); function */

/* Prints a string to the LCD at the current position 
 * If the line overflows, the data continues to the next address */
void UI_LCD_String_P(const char* string_P)
{
   uint8_t c;
   UI_LCD_SetData();
   
   while( (c = *string_P++) )
   {
      UI_LCD_Write(c);
   }   
}


/* Max Row is 3 and Max Col is 0 -> 19 (Dec) */
void UI_LCD_Pos(uint8_t row, uint8_t col)
{
   uint8_t DDRAMAddr;
   
   UI_LCD_SetInstruction();   
   
	// remap lines into proper order
	switch(row)
	{
	  case 0: DDRAMAddr = LCD_LINE0_DDRAMADDR + col; break;
	  case 1: DDRAMAddr = LCD_LINE1_DDRAMADDR + col; break;
	  case 2: DDRAMAddr = LCD_LINE2_DDRAMADDR + col; break;
	  case 3: DDRAMAddr = LCD_LINE3_DDRAMADDR + col; break;
	  default: DDRAMAddr = LCD_LINE0_DDRAMADDR + col; break;
	}

	// set data address
	UI_LCD_Write(1<<LCD_DDRAM | DDRAMAddr);

}





void UI_LCD_SetCursor(void)
{
	UI_LCD_SetInstruction();
	
	/** Activates the display, cursor and cursor blink */
	UI_LCD_Write( (1 << LCD_DISPLAY) 
					| (1 << LCD_ON_DISPLAY) 
					| (1 << LCD_ON_CURSOR) ); 
}

void UI_LCD_ClearCursor(void)
{
	UI_LCD_SetInstruction();
	
	/** Activates the display, cursor and cursor blink */
	UI_LCD_Write( (1 << LCD_DISPLAY) | (1 << LCD_ON_DISPLAY)); 
}


   
   
/* Back light control don't need to use PWM, use the 16-bit timer for
 * something more useful
 */
void UI_LCD_BL_On(void)
{
   LCD_BL_PORT |= (1 << LCD_BL_PIN);   
}

void UI_LCD_BL_Off(void)
{
   LCD_BL_PORT &= ~(1 << LCD_BL_PIN);  
}

void UI_LCD_BL_Toggle(void)
{
   LCD_BL_PORT ^= (1 << LCD_BL_PIN);   
}



/* Accepts a 8 Byte custom char array */
void UI_LCD_LoadCustomChar(uint8_t* lcdCustomCharArray, uint8_t lcdCharNum)
{
	uint8_t i;

	// multiply the character index by 8
	lcdCharNum = (lcdCharNum<<3);	// each character occupies 8 bytes

	// copy the 8 bytes into CG (character generator) RAM
   UI_LCD_SetInstruction();
	// set CG RAM address
	UI_LCD_Write((1<<LCD_CGRAM) | (lcdCharNum));	
	
	/* Assumes auto increment of cursor position */
	for(i=0; i<8; i++)
	{	      

		_delay_us(200);
		UI_LCD_SetData();
		// write character data
		UI_LCD_Write( *(lcdCustomCharArray++) );
	}

   /* Reset the DDRAM LCD Position */
   /* Basically a faster return home */
   UI_LCD_Pos(0,0);

}

void UI_LCD_LoadDefaultChars(void)
{
	/* Load VU Meter and down arrow for the menu*/
	UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[14], 0);	
	UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[7], 1);
	UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[8], 2);
   UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[9], 3);
	UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[10], 4);
	UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[11], 5);
	UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[12], 6);
   UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[13], 7);		
}
