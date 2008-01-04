/** \file UI_LCD.h
	 \brief LCD Routines
*/

/**	 

	\ingroup avr_external
 	\defgroup UI_LCD HD44780 LCD Interface
 	\code #include "uartInput.h" \endcode

 	 
	 \par Description
    This module utilises the MAX7300 Port Expander to interface with
    a 20x4 Alphanumeric LCD.
*/
//@{
   

#ifndef	_UI_LCD_ROUTINES
#define	_UI_LCD_ROUTINES

#include "MAX7300/max7300.h"

/* LCD Inputs */
#define UI_LCD_POWER    (MAX7300_P18)
#define UI_LCD_RS       (MAX7300_P17)
#define UI_LCD_E        (MAX7300_P16)
#define UI_LCD_D4       (MAX7300_P15)
#define UI_LCD_D5       (MAX7300_P14)
#define UI_LCD_D6       (MAX7300_P13)
#define UI_LCD_D7       (MAX7300_P12)    

#define UI_LCD_PORT     (0x4C)
#define UI_LCD_PPOWER   (6)
#define UI_LCD_PRS      (5)
#define UI_LCD_PE       (4)
#define UI_LCD_PD7      (3)
#define UI_LCD_PD6      (2)
#define UI_LCD_PD5      (1)
#define UI_LCD_PD4      (0)

#define UI_LCD_INSTRUCTION (0)
#define UI_LCD_DATA        (1)

// HD44780 LCD controller command set (do not modify these)
// writing:
#define LCD_CLR             0      // DB0: clear display
#define LCD_HOME            1      // DB1: return to home position
#define LCD_ENTRY_MODE      2      // DB2: set entry mode
#define LCD_ENTRY_INC       1      //   DB1: increment
#define LCD_ENTRY_SHIFT     0      //   DB2: shift
#define LCD_DISPLAY         3      //   DB3: Set Display Mode
#define LCD_ON_DISPLAY      2      //   DB2: turn display on
#define LCD_ON_CURSOR       1      //   DB1: turn cursor on
#define LCD_ON_BLINK        0      //   DB0: blinking cursor
#define LCD_MOVE            4      // DB4: move cursor/display
#define LCD_MOVE_DISP       3      //   DB3: move display (0-> move cursor)
#define LCD_MOVE_RIGHT      2      //   DB2: move right (0-> left)
#define LCD_FUNCTION        5      // DB5: function set
#define LCD_FUNCTION_8BIT   4      //   DB4: set 8BIT mode (0->4BIT mode)
#define LCD_FUNCTION_2LINES 3      //   DB3: two lines (0->one line)
#define LCD_FUNCTION_10DOTS 2      //   DB2: 5x10 font (0->5x7 font)
#define LCD_CGRAM           6      // DB6: set CG RAM address
#define LCD_DDRAM           7      // DB7: set DD RAM address
// reading:
#define LCD_BUSY            7      // DB7: LCD is busy


#define LCD_FUNCTION_DEFAULT	((1<<LCD_FUNCTION) | (1<<LCD_FUNCTION_2LINES))
#define LCD_MODE_DEFAULT		((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC))
#define LCD_DISPLAY_DEFAULT   ((1<<LCD_DISPLAY) | (1<<LCD_ON_DISPLAY))


#define LCD_LINE0_DDRAMADDR   (0x00)
#define LCD_LINE1_DDRAMADDR   (0x40)
#define LCD_LINE2_DDRAMADDR   (0x14)
#define LCD_LINE3_DDRAMADDR   (0x54)


#define LCD_BL_PORT  P1OUT
#define LCD_BL_PIN   2
#define LCD_BL_DDR   P1DIR

#define LCD_RAM_END  (0x67)

//#define 

#define LCD_ON 1
#define LCD_OFF 0

extern const uint8_t LcdCustomChar[][8]; 

void UI_LCD_HWInit(void);
void UI_LCD_Init(void);
void UI_LCD_Write(char code);
void UI_LCD_SetInstruction(void);
void UI_LCD_SetData(void);

void UI_LCD_Char(char data);


void UI_LCD_String(char* string);
void UI_LCD_String_P(const char* string_P);

void UI_LCD_Char(char data);

void UI_LCD_Clear(void);
void UI_LCD_Home(void);

void UI_LCD_Pos(uint8_t row, uint8_t col);
void UI_LCD_Strobe(void);

void UI_LCD_SetCursor(void);
void UI_LCD_ClearCursor(void);

void UI_LCD_Activate(void);
void UI_LCD_Shutdown(void);

void UI_LCD_BL_Off(void);
void UI_LCD_BL_On(void);

/* Accepts a 7 Byte custom char array */
void UI_LCD_LoadCustomChar(uint8_t* lcdCustomCharArray, uint8_t lcdCharNum);
void UI_LCD_LoadDefaultChars(void);


#endif
