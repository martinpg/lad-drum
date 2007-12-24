/** \file Menu.h
	 \brief Menu Machine for AVR
*/

/**	 

	\ingroup avr_datalogger
 	\defgroup menu Menu State Machine
 	\code #include "menu.h" \endcode

 	 
	 \par Description
	Implements a menu function.
 	 
*/
//@{


#ifndef	_MENU_ROUTINES
#define	_MENU_ROUTINES

#define MENU_DEBUG 0

#define PRINT_FUNC   UI_LCD_String_P
#define MENU_TEXT    char

#define NO_STATE     0
#define INVALID_STATE 255
#define INVALID_SEQUENCE 255

#define MAIN_MENU    ST_MAIN
#define MENU_LCD  0
#define MENU_UART 1



#define WINDOW_SIZE 3

#if MENU_DEBUG == 1

#define KP_1   '1'
#define KP_2   '2'
#define KP_3   '3'
#define KP_4   '4'
#define KP_5   '5'
#define KP_6   '6'
#define KP_7   '7'
#define KP_8   '8'
#define KP_9   '9'



#endif

#define KB_UP '8'
#define KB_DOWN '2'
#define KB_ENTER '6'
#define KB_BACK  '4'

#define KP_UP  KP_2
#define KP_DOWN KP_8
#define KP_ENTER KP_6
#define KP_BACK  KP_4

#define PSTR(x)   (x)

typedef struct
{
   uint8_t  parent;
   uint8_t  child;
   uint8_t  sequence; /// Effectively, which line of the LCD the item is listed on.
   
} menu_list;


typedef struct
{
   uint8_t  menu_item; 
   const char* descriptor;
   void (*function)(void* data);

} menu_data;

/* Menu Texts */
enum {  
   ST_MAIN = 10,
   ST_PROFILES,
   ST_OPTIONS,
   ST_MIDI_OUTPUT_RATE,
   ST_SET_RATE,
   ST_CHANNEL_SETUP,
   ST_CHANNEL_1,
   ST_CHANNEL_2,
   ST_CHANNEL_3,
   ST_CHANNEL_4,
   ST_CHANNEL_5,
   ST_CHANNEL_6,
   ST_CHANNEL_7,
   ST_CHANNEL_8,
   ST_CHANNEL_9,
   ST_CHANNEL_10,
   ST_CHANNEL_11,
   ST_CHANNEL_12,
   ST_CHANNEL_13,
   ST_CHANNEL_14,
   ST_CHANNEL_15,   
   ST_CHANNEL_16,
	ST_THRESHOLD_1,
	ST_THRESHOLD_2,
	ST_THRESHOLD_3,
	ST_THRESHOLD_4,
	ST_THRESHOLD_5,
	ST_THRESHOLD_6,
	ST_THRESHOLD_7,
	ST_THRESHOLD_8,
	ST_THRESHOLD_9,
	ST_THRESHOLD_10,
	ST_THRESHOLD_11,
	ST_THRESHOLD_12,
	ST_THRESHOLD_13,
	ST_THRESHOLD_14,
	ST_THRESHOLD_15,
	ST_THRESHOLD_16,
	ST_RETRIGGER_1,
	ST_RETRIGGER_2,
	ST_RETRIGGER_3,
	ST_RETRIGGER_4,
	ST_RETRIGGER_5,
	ST_RETRIGGER_6,
	ST_RETRIGGER_7,
	ST_RETRIGGER_8,
	ST_RETRIGGER_9,
	ST_RETRIGGER_10,
	ST_RETRIGGER_11,
	ST_RETRIGGER_12,
	ST_RETRIGGER_13,
	ST_RETRIGGER_14,
	ST_RETRIGGER_15,
	ST_RETRIGGER_16,
   ST_LOAD_PROFILE,
   ST_SAVE_PROFILE,
   ST_LOAD_PROFILE_1,
   ST_LOAD_PROFILE_2,
   ST_LOAD_PROFILE_3,
   ST_LOAD_PROFILE_4,
   ST_LOAD_PROFILE_DEF,
   ST_SAVE_PROFILE_1,
   ST_SAVE_PROFILE_2,
   ST_SAVE_PROFILE_3,
   ST_SAVE_PROFILE_4,
   ST_CHANGE_CHANNEL_CODE,
   
     
} menuIds;


extern uint8_t currentState;
extern uint8_t RowPosition;
extern uint8_t upperLimit;
extern uint8_t lowerLimit;
extern uint8_t firstEnter;
extern uint8_t selectedItem;
/** General Menu Functions */
void MenuSetDisplay(uint8_t display);
void (*MenuPrint)(uint8_t* string);
/* For a string residing in FLASH */
void (*MenuPrint_P)(const char* string);
void (*MenuNewLine)(void);
void (*MenuReset)(void);




void LCD_NewLine(void);
void LCD_Reset(void);

/* Menu Uart Functions */
void MenuUart_NewLine(void);
void MenuUartTxString(uint8_t* string);
void MenuUartTxString_P(const char* string);
void MenuUart_Reset(void);


void MenuSetDisplay(uint8_t display);

void MenuSetInput(uint8_t NewInput);

void MenuUpdate(void);
char* MenuDescriptor(uint8_t menuItem);
void stateMachine(uint8_t CurrentState);
uint8_t GetMenuState(uint8_t state, uint8_t Sequence);
uint8_t GetParent(uint8_t state);
uint8_t SubItems(uint8_t state);
uint8_t GetIndex(uint8_t parent);
void executeState(uint8_t state);
uint8_t LargestSequence(uint8_t state);
uint8_t SmallestSequence(uint8_t state);

uint8_t GetState(void);

#endif








