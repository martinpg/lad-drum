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

#include <io.h>
#include <version.h>



#define UI_LCD_RS_INSTRUCTION (0)
#define UI_LCD_RS_DATA        (1)

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
#define LCD_BL_SEL   P1SEL

#define LCD_BL_MAX_BRIGHTNESS   (100)
#define LCD_BL_MIN_BRIGHTNESS   (20)
#define LCD_BL_TIMED   0
#define LCD_BL_ON    1

#define LCD_RAM_END  (0x67)

//#define 

#define LCD_ON 1
#define LCD_OFF 0


typedef struct
{   
   /* User to specify the dimensions of the LCD */
   uint8_t MAX_ROW;
   uint8_t MAX_COL;
   
   uint8_t RSStatus;
   uint8_t RowPos;
   uint8_t ColPos;
   
   /* Set up data direction registers */
   void (*HWInit)(void);
   /* Sets the data on the 8/4 bit bus */
   void (*SetRegister)(uint8_t data);
   
   /* Strobes the clock 'E' pin */
   void (*Strobe)(void);
   
   /* Backlight functionality */
   void (*BL_On)(void);
   void (*BL_Off)(void);
   
        
} HD44780lcd_t;

extern const uint8_t LcdCustomChar[][8]; 


void UI_LCD_Init(HD44780lcd_t* lcd);
void UI_LCD_Write(HD44780lcd_t* lcd, char code);
void UI_LCD_SetInstruction(HD44780lcd_t* lcd);
void UI_LCD_SetData(HD44780lcd_t* lcd);

void UI_LCD_Char(HD44780lcd_t* lcd, char data);


void UI_LCD_String(HD44780lcd_t* lcd, char* string);
void UI_LCD_String_P(HD44780lcd_t* lcd, const char* string_P);

void UI_LCD_Char(HD44780lcd_t* lcd, char data);

void UI_LCD_Clear(HD44780lcd_t* lcd);
void UI_LCD_Home(HD44780lcd_t* lcd);

void UI_LCD_Pos(HD44780lcd_t* lcd, uint8_t row, uint8_t col);


void UI_LCD_SetCursor(HD44780lcd_t* lcd);
void UI_LCD_ClearCursor(HD44780lcd_t* lcd);

void UI_LCD_Activate(void);
void UI_LCD_Shutdown(void);



/* Accepts a 7 Byte custom char array */
void UI_LCD_LoadCustomChar(HD44780lcd_t* lcd, uint8_t* lcdCustomCharArray, uint8_t lcdCharNum);



#endif
