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
   ST_PLAYMODE,
   ST_PROFILES,
   ST_OPTIONS,
   ST_MIDI_OUTPUT_RATE,
   ST_SET_RATE,
   ST_INPUT_SELECT,
   ST_CHANNEL_SETUP,
   ST_ANALOGUE_INPUTS,
   ST_DIGITAL_INPUTS,
   ST_METRONOME_INPUTS,
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
/* Effectively Channel 17 -> 32 */   
	ST_DIGITAL_1,
	ST_DIGITAL_2,
	ST_DIGITAL_3,
	ST_DIGITAL_4,
	ST_DIGITAL_5,
	ST_DIGITAL_6,
	ST_DIGITAL_7,
	ST_DIGITAL_8,
/* Metronome inputs are basically Digital inputs CH9->16 */	
   ST_METRONOME_1,
   ST_METRONOME_2,
   ST_METRONOME_3,
   ST_METRONOME_4,
   ST_METRONOME_5,
   ST_METRONOME_6,
   ST_METRONOME_7,
   ST_METRONOME_8,	
/* Analogue Thresholds */	
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
/* Digital Trigger Setup */	
	ST_TRIGGER_TYPE_D1,
	ST_TRIGGER_TYPE_D2,
	ST_TRIGGER_TYPE_D3,
	ST_TRIGGER_TYPE_D4,
	ST_TRIGGER_TYPE_D5,
	ST_TRIGGER_TYPE_D6,
	ST_TRIGGER_TYPE_D7,
	ST_TRIGGER_TYPE_D8,
/* Retriggers*/	
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
/* Digital Retrigger enums must go here */	
	ST_RETRIGGER_D1,
	ST_RETRIGGER_D2,
	ST_RETRIGGER_D3,
	ST_RETRIGGER_D4,
	ST_RETRIGGER_D5,
	ST_RETRIGGER_D6,
	ST_RETRIGGER_D7,
	ST_RETRIGGER_D8,
/* End of Digital Retrigger enums */
/* Metronome Retrigger enums */
	ST_RETRIGGER_M1,
	ST_RETRIGGER_M2,
	ST_RETRIGGER_M3,
	ST_RETRIGGER_M4,
	ST_RETRIGGER_M5,
	ST_RETRIGGER_M6,
	ST_RETRIGGER_M7,
	ST_RETRIGGER_M8,
/* Dual Input Menu States */
	ST_DUALINPUT_1,
	ST_DUALINPUT_2,
	ST_DUALINPUT_3,
	ST_DUALINPUT_4,
	ST_DUALINPUT_5,
	ST_DUALINPUT_6,
	ST_DUALINPUT_7,
	ST_DUALINPUT_8,
	ST_DUALINPUT_9,
	ST_DUALINPUT_10,
	ST_DUALINPUT_11,
	ST_DUALINPUT_12,
	ST_DUALINPUT_13,
	ST_DUALINPUT_14,
	ST_DUALINPUT_15,
	ST_DUALINPUT_16,
/* End of Dual Input Screens */	

/* Gain Curves */
	ST_SETGAIN_1,
	ST_SETGAIN_2,
	ST_SETGAIN_3,
	ST_SETGAIN_4,
	ST_SETGAIN_5,
	ST_SETGAIN_6,
	ST_SETGAIN_7,
	ST_SETGAIN_8,
	ST_SETGAIN_9,
	ST_SETGAIN_10,
	ST_SETGAIN_11,
	ST_SETGAIN_12,
	ST_SETGAIN_13,
	ST_SETGAIN_14,
	ST_SETGAIN_15,
	ST_SETGAIN_16,
/* END Of Gain Curves */
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
   ST_CROSSTALK,
   ST_FIXED_GAIN,
   ST_VARIABLE_GAIN,
   
   ST_VUMETER,
   ST_DIGITAL_VUMETER,
   ST_SYSTEM_SETUP,
	ST_SYSTEM_RESET,     
   ST_SYSTEM_ABOUT,
   
} menuIds;


extern uint8_t currentState;
extern uint8_t RowPosition;
extern uint8_t upperLimit;
extern uint8_t lowerLimit;
extern uint8_t firstEnter;
extern uint8_t selectedItem;
/** General Menu Functions */
void MenuSetDisplay(uint8_t display);
void (*MenuPrint)(char* string);
/* For a string residing in FLASH */
void (*MenuPrint_P)(const char* string);
void (*MenuNewLine)(void);
void (*MenuReset)(void);
void (*MenuChar)(char data);



void LCD_NewLine(void);
void LCD_Reset(void);

/* Menu Uart Functions */
void MenuUart_NewLine(void);
void MenuUartTxString(char* string);
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








