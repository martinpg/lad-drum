#ifndef _MENU_SETTINGS_H
#define _MENU_SETTINGS_H

#include "Menu/Menu.h"

/* Menu Texts */
enum {  
   ST_MAIN = 10,
   ST_CONTROLLER_MODE,
   ST_PROFILES,
   ST_OPTIONS,
   ST_MIDI_OUTPUT_RATE,
   ST_SET_RATE,
   ST_INPUT_SELECT,
   ST_CHANNEL_SETUP,
   ST_ANALOGUE_INPUTS,
   ST_DIGITAL_INPUTS,
   ST_METRONOME_INPUTS,
   ST_KEYPAD_INPUTS,
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
/* Keypad Setting Channels */   
   ST_KEYPAD_1,
   ST_KEYPAD_2,
   ST_KEYPAD_3,
   ST_KEYPAD_4,
   ST_KEYPAD_5,
   ST_KEYPAD_6,
   ST_KEYPAD_7,
   ST_KEYPAD_8,
   ST_KEYPAD_9,
   ST_KEYPAD_10,
   ST_KEYPAD_11,
   ST_KEYPAD_12,
   ST_KEYPAD_13,
   ST_KEYPAD_14,
   ST_KEYPAD_15,   
   ST_KEYPAD_16,		
/* End of Metronome inputs */   
   ST_LOAD_PROFILE,
   ST_SAVE_PROFILE,
   ST_LOAD_PROFILE_DEF,
   ST_LOAD_PROFILE_1,
   ST_LOAD_PROFILE_2,
   ST_LOAD_PROFILE_3,
   ST_LOAD_PROFILE_4,
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
   ST_SYSTEM_UPGRADE,
   ST_SYSEX,
   
   ST_SYSEX_DUMP,
   ST_SYSEX_RECEIVE,
 
   
} menuIds;

/* Channel Setup Menus */
enum {
   /* The 'main' sub menu */
   ST_ANALOGUE_SETUP = 10,
   ST_DIGITAL_SETUP,
   ST_THRESHOLD,
   ST_RETRIGGER,
   ST_SETGAIN,
   ST_DUALINPUT,
   ST_MONITORCHANNEL,
   /* For digital sub menu */
   ST_TRIGGER_TYPE,
   
} channelSetupIds;


extern Menu_t primaryMenu;
extern Menu_t analogueMenu;
extern Menu_t digitalMenu;

extern Menu_t* ActiveMenu;
extern Menu_t* SelectedSubMenu;

void LCD_NewLine(void);
void LCD_Reset(void);
void MenuLCD_SetPos(uint8_t row, uint8_t col);
void MenuLCD_String(char* string);
void MenuLCD_String_P(const char* string);
void MenuLCD_Char(uint8_t data);

/* Menu Uart Functions */
void MenuUart_NewLine(void);
void MenuUartTxString(char* string);
void MenuUartTxString_P(const char* string);
void MenuUart_Reset(void);


void MenuSetDisplay(Menu_t* menu, uint8_t display);
void Menu_UpOneLevel(Menu_t* menu);


#endif
