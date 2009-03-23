/* Project Specific Menus */
#include <stdint.h>
#include "MenuSettings.h"
#include "UART/uart.h"
#include "UserFunctions/userFunctions.h"
#include "LCDSettings.h"

#if MENU_DEBUG == 0
#include "UI_LCD/UI_LCD.h"
#include "UI_KP/UI_KP.h"
#include <string.h>
#endif


const MENU_TEXT  MT_CONTROLLER_MODE[] = "Controller Mode";


const MENU_TEXT  MT_PROFILES[] = "Profiles";
const MENU_TEXT  MT_LOAD_PROFILE[] = "Load Profile";
const MENU_TEXT  MT_SAVE_PROFILE[] = "Save Profile As...";

const MENU_TEXT  MT_PROFILE_1[] = "Profile 1";
const MENU_TEXT  MT_PROFILE_2[] = "Profile 2";
const MENU_TEXT  MT_PROFILE_3[] = "Profile 3";
const MENU_TEXT  MT_PROFILE_4[] = "Profile 4";
const MENU_TEXT  MT_PROFILE_DEF[] = "Default Profile";


const MENU_TEXT  MT_VUMETER[] = "VU Meter";
const MENU_TEXT  MT_DIGITAL_VUMETER[] = "Digital VU Meter";
const MENU_TEXT  MT_SYSTEM_SETUP[] = "System Setup";

const MENU_TEXT  MT_SYSTEM_RESET[] = "Reset eDrum";
const MENU_TEXT  MT_SYSTEM_ABOUT[] = "About";
const MENU_TEXT  MT_SYSTEM_SYSEX[] = "System Exclusive";
const MENU_TEXT  MT_SYSEX_DUMP[] = "Dump Current Setup";
const MENU_TEXT  MT_SYSEX_RECEIVE[] = "Get SysEx Settings";

const MENU_TEXT  MT_OPTIONS[] = "Options";
const MENU_TEXT  MT_MIDI_OUTPUT_RATE[] = "MIDI Output Rate";
const MENU_TEXT  MT_SET_RATE[] = "Edit Output Rate";
const MENU_TEXT  MT_CHANGE_CHANNEL_CODE[] = "Edit Channel Code";
const MENU_TEXT  MT_CHANNEL_SETUP[] = "Channel Settings";
const MENU_TEXT  MT_INPUT_SELECT[] = "Amp. Input Select";
const MENU_TEXT  MT_CROSSTALK[] = "Adjust Crosstalk";

const MENU_TEXT  MT_ANALOGUE_INPUTS[] = "Analogue Inputs";
const MENU_TEXT  MT_DIGITAL_INPUTS[] = "Digital Inputs";
const MENU_TEXT  MT_METRONOME_INPUTS[] = "Metronome Inputs";
const MENU_TEXT  MT_KEYPAD_INPUTS[] = "Keypad Buttons";

const MENU_TEXT  MT_CHANNEL_1[] = "Channel 1";
const MENU_TEXT  MT_CHANNEL_2[] = "Channel 2";
const MENU_TEXT  MT_CHANNEL_3[] = "Channel 3";
const MENU_TEXT  MT_CHANNEL_4[] = "Channel 4";
const MENU_TEXT  MT_CHANNEL_5[] = "Channel 5";
const MENU_TEXT  MT_CHANNEL_6[] = "Channel 6";
const MENU_TEXT  MT_CHANNEL_7[] = "Channel 7";
const MENU_TEXT  MT_CHANNEL_8[] = "Channel 8";
const MENU_TEXT  MT_CHANNEL_9[] = "Channel 9";
const MENU_TEXT  MT_CHANNEL_10[] = "Channel 10";
const MENU_TEXT  MT_CHANNEL_11[] = "Channel 11";
const MENU_TEXT  MT_CHANNEL_12[] = "Channel 12";
const MENU_TEXT  MT_CHANNEL_13[] = "Channel 13";
const MENU_TEXT  MT_CHANNEL_14[] = "Channel 14";
const MENU_TEXT  MT_CHANNEL_15[] = "Channel 15";
const MENU_TEXT  MT_CHANNEL_16[] = "Channel 16";

const MENU_TEXT  MT_DIGITAL_1[] = "Digital Channel 1";
const MENU_TEXT  MT_DIGITAL_2[] = "Digital Channel 2";
const MENU_TEXT  MT_DIGITAL_3[] = "Digital Channel 3";
const MENU_TEXT  MT_DIGITAL_4[] = "Digital Channel 4";
const MENU_TEXT  MT_DIGITAL_5[] = "Digital Channel 5";
const MENU_TEXT  MT_DIGITAL_6[] = "Digital Channel 6";
const MENU_TEXT  MT_DIGITAL_7[] = "Digital Channel 7";
const MENU_TEXT  MT_DIGITAL_8[] = "Digital Channel 8";

const MENU_TEXT  MT_METRONOME_1[] = "Metronome Input 1";
const MENU_TEXT  MT_METRONOME_2[] = "Metronome Input 2";
const MENU_TEXT  MT_METRONOME_3[] = "Metronome Input 3";
const MENU_TEXT  MT_METRONOME_4[] = "Metronome Input 4";
const MENU_TEXT  MT_METRONOME_5[] = "Metronome Input 5";
const MENU_TEXT  MT_METRONOME_6[] = "Metronome Input 6";
const MENU_TEXT  MT_METRONOME_7[] = "Metronome Input 7";
const MENU_TEXT  MT_METRONOME_8[] = "Metronome Input 8";

const MENU_TEXT  MT_KEYPAD_1[] = "Key 0";
const MENU_TEXT  MT_KEYPAD_2[] = "Key 1";
const MENU_TEXT  MT_KEYPAD_3[] = "Key 2";
const MENU_TEXT  MT_KEYPAD_4[] = "Key 3";
const MENU_TEXT  MT_KEYPAD_5[] = "Key 4";
const MENU_TEXT  MT_KEYPAD_6[] = "Key 5";
const MENU_TEXT  MT_KEYPAD_7[] = "Key 6";
const MENU_TEXT  MT_KEYPAD_8[] = "Key 7";
const MENU_TEXT  MT_KEYPAD_9[] = "Key 8";
const MENU_TEXT  MT_KEYPAD_10[] = "Key 9";
const MENU_TEXT  MT_KEYPAD_11[] = "Key A";
const MENU_TEXT  MT_KEYPAD_12[] = "Key B";
const MENU_TEXT  MT_KEYPAD_13[] = "Key C";
const MENU_TEXT  MT_KEYPAD_14[] = "Key D";
const MENU_TEXT  MT_KEYPAD_15[] = "Key *";
const MENU_TEXT  MT_KEYPAD_16[] = "Key #";

const MENU_TEXT  MT_THRESHOLD[] = "Set Threshold";
const MENU_TEXT  MT_TRIGGER_TYPE[] = "Switch Settings";
const MENU_TEXT  MT_RETRIGGER[] = "Set Retrigger";
const MENU_TEXT  MT_DUALINPUT[] = "Set Dual Triggering";
const MENU_TEXT  MT_SETGAIN[] = "Set Gain Curves";

const MENU_TEXT  MT_FIXED_GAIN[] = "Fixed Gain (1x)";
const MENU_TEXT  MT_VARIABLE_GAIN[] = "Variable Gain";

/* Once a menu item is selected, it will either end in
 * 1) Another Menu or
 * 2) A function
 * */
const menu_list MenuState[] = {

	{ST_MAIN,	ST_CONTROLLER_MODE,  0},
   {ST_MAIN,   ST_OPTIONS,   1},
   {ST_MAIN,	ST_VUMETER,	  2},
   {ST_MAIN,   ST_DIGITAL_VUMETER, 3},
   {ST_MAIN, 	ST_PROFILES,  4},
   {ST_MAIN,   ST_SYSTEM_SETUP, 5},

   {ST_OPTIONS, ST_MIDI_OUTPUT_RATE,  0},
   {ST_OPTIONS, ST_CHANNEL_SETUP, 1},
   {ST_OPTIONS, ST_CHANGE_CHANNEL_CODE, 2},
   {ST_OPTIONS, ST_INPUT_SELECT, 3},
   {ST_OPTIONS, ST_CROSSTALK, 4},
	
	{ST_INPUT_SELECT, ST_FIXED_GAIN, 2},
	{ST_INPUT_SELECT, ST_VARIABLE_GAIN, 3},		
		
   {ST_MIDI_OUTPUT_RATE, ST_SET_RATE, 1},

   {ST_CHANNEL_SETUP, ST_ANALOGUE_INPUTS,  0},
   {ST_CHANNEL_SETUP, ST_DIGITAL_INPUTS,  1}, 
   {ST_CHANNEL_SETUP, ST_METRONOME_INPUTS, 2},
	{ST_CHANNEL_SETUP, ST_KEYPAD_INPUTS,  3},  

   {ST_ANALOGUE_INPUTS, ST_CHANNEL_1,  0},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_2,  1},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_3,  2},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_4,  3},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_5,  4},            
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_6,  5},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_7,  6},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_8,  7},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_9,  8},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_10, 9},    
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_11, 10},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_12, 11},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_13, 12},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_14, 13},
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_15, 14}, 
   {ST_ANALOGUE_INPUTS, ST_CHANNEL_16, 15},

	/* Digital Config Menu's */
   {ST_DIGITAL_INPUTS, ST_DIGITAL_1,  0},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_2,  1},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_3,  2},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_4,  3},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_5,  4},            
   {ST_DIGITAL_INPUTS, ST_DIGITAL_6,  5},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_7,  6},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_8,  7},

   {ST_METRONOME_INPUTS, ST_METRONOME_1, 0},
   {ST_METRONOME_INPUTS, ST_METRONOME_2, 1},
   {ST_METRONOME_INPUTS, ST_METRONOME_3, 2},
   {ST_METRONOME_INPUTS, ST_METRONOME_4, 3},
   {ST_METRONOME_INPUTS, ST_METRONOME_5, 4},
   {ST_METRONOME_INPUTS, ST_METRONOME_6, 5},
   {ST_METRONOME_INPUTS, ST_METRONOME_7, 6},                  
   {ST_METRONOME_INPUTS, ST_METRONOME_8, 7},

   {ST_KEYPAD_INPUTS, ST_KEYPAD_1,  0},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_2,  1},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_3,  2},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_4,  3},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_5,  4},            
   {ST_KEYPAD_INPUTS, ST_KEYPAD_6,  5},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_7,  6},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_8,  7},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_9,  8},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_10, 9},    
   {ST_KEYPAD_INPUTS, ST_KEYPAD_11, 10},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_12, 11},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_13, 12},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_14, 13},
   {ST_KEYPAD_INPUTS, ST_KEYPAD_15, 14}, 
   {ST_KEYPAD_INPUTS, ST_KEYPAD_16, 15},   
   
   {ST_PROFILES, ST_LOAD_PROFILE, 2},
   {ST_PROFILES, ST_SAVE_PROFILE, 3},
		
	{ST_LOAD_PROFILE, ST_LOAD_PROFILE_1, 0},	   
	{ST_LOAD_PROFILE, ST_LOAD_PROFILE_2, 1},
	{ST_LOAD_PROFILE, ST_LOAD_PROFILE_3, 2},
	{ST_LOAD_PROFILE, ST_LOAD_PROFILE_4, 3},
	{ST_LOAD_PROFILE, ST_LOAD_PROFILE_DEF, 4},
	
	{ST_SAVE_PROFILE, ST_SAVE_PROFILE_1, 0},	   
	{ST_SAVE_PROFILE, ST_SAVE_PROFILE_2, 1},
	{ST_SAVE_PROFILE, ST_SAVE_PROFILE_3, 2},
	{ST_SAVE_PROFILE, ST_SAVE_PROFILE_4, 3},
		
	{ST_SYSTEM_SETUP, ST_SYSTEM_RESET, 0},
	{ST_SYSTEM_SETUP, ST_SYSTEM_ABOUT, 1},
	{ST_SYSTEM_SETUP, ST_SYSEX, 2},
	
	{ST_SYSEX, ST_SYSEX_DUMP, 1},
	{ST_SYSEX, ST_SYSEX_RECEIVE, 2},
			   
   {0, 0, 0}
};


const menu_data MenuData[] = {
   {ST_MAIN, 0, 0},
   {ST_CONTROLLER_MODE, MT_CONTROLLER_MODE, ControllerMode},
   {ST_PROFILES, MT_PROFILES, ShowProfile},   
   {ST_OPTIONS, MT_OPTIONS, 0},
	{ST_VUMETER, MT_VUMETER, VUMeterSetup},
	{ST_DIGITAL_VUMETER, MT_DIGITAL_VUMETER, VUMeterSetup},
	{ST_SYSTEM_SETUP, MT_SYSTEM_SETUP, 0},
	{ST_SYSTEM_RESET, MT_SYSTEM_RESET, reset},
	{ST_SYSTEM_ABOUT, MT_SYSTEM_ABOUT, about},
	{ST_SYSEX, MT_SYSTEM_SYSEX, SysExDisplay},
	{ST_SYSEX_DUMP, MT_SYSEX_DUMP, DumpSysEx},
	{ST_SYSEX_RECEIVE, MT_SYSEX_RECEIVE, GetSysEx},
	
   {ST_MIDI_OUTPUT_RATE, MT_MIDI_OUTPUT_RATE, SetMIDIRate},
   {ST_SET_RATE, MT_SET_RATE, EditMIDIRate},
   {ST_INPUT_SELECT, MT_INPUT_SELECT, AmpInputSelect},
   {ST_CROSSTALK, MT_CROSSTALK, AdjustCrosstalk},
 	{ST_CHANNEL_SETUP, MT_CHANNEL_SETUP, 0},
 	
   {ST_ANALOGUE_INPUTS, MT_ANALOGUE_INPUTS,  0},
   {ST_DIGITAL_INPUTS, MT_DIGITAL_INPUTS,  0},    	
   {ST_METRONOME_INPUTS, MT_METRONOME_INPUTS,  0},
 	{ST_KEYPAD_INPUTS, MT_KEYPAD_INPUTS,  0},
 	
 	/* Analogue Menus */
   {ST_CHANNEL_1, MT_CHANNEL_1,   ChannelSetup},
   {ST_CHANNEL_2, MT_CHANNEL_2,   ChannelSetup},
   {ST_CHANNEL_3, MT_CHANNEL_3,   ChannelSetup},
   {ST_CHANNEL_4, MT_CHANNEL_4,   ChannelSetup},
   {ST_CHANNEL_5, MT_CHANNEL_5,   ChannelSetup},            
   {ST_CHANNEL_6, MT_CHANNEL_6,   ChannelSetup},
   {ST_CHANNEL_7, MT_CHANNEL_7,   ChannelSetup},
   {ST_CHANNEL_8, MT_CHANNEL_8,   ChannelSetup},
   {ST_CHANNEL_9, MT_CHANNEL_9,   ChannelSetup},
   {ST_CHANNEL_10, MT_CHANNEL_10, ChannelSetup},    
   {ST_CHANNEL_11, MT_CHANNEL_11, ChannelSetup},
   {ST_CHANNEL_12, MT_CHANNEL_12, ChannelSetup},
   {ST_CHANNEL_13, MT_CHANNEL_13, ChannelSetup},
   {ST_CHANNEL_14, MT_CHANNEL_14, ChannelSetup},
   {ST_CHANNEL_15, MT_CHANNEL_15, ChannelSetup}, 
   {ST_CHANNEL_16, MT_CHANNEL_16, ChannelSetup},  
   
	/* Digital Menus */
   {ST_DIGITAL_1, MT_DIGITAL_1,   DigitalChannelSettings},
   {ST_DIGITAL_2, MT_DIGITAL_2,   DigitalChannelSettings},
   {ST_DIGITAL_3, MT_DIGITAL_3,   DigitalChannelSettings},
   {ST_DIGITAL_4, MT_DIGITAL_4,   DigitalChannelSettings},
   {ST_DIGITAL_5, MT_DIGITAL_5,   DigitalChannelSettings},            
   {ST_DIGITAL_6, MT_DIGITAL_6,   DigitalChannelSettings},
   {ST_DIGITAL_7, MT_DIGITAL_7,   DigitalChannelSettings},
   {ST_DIGITAL_8, MT_DIGITAL_8,   DigitalChannelSettings},

	/* Digital Menus */
   {ST_METRONOME_1, MT_METRONOME_1,   DigitalChannelSettings},
   {ST_METRONOME_2, MT_METRONOME_2,   DigitalChannelSettings},
   {ST_METRONOME_3, MT_METRONOME_3,   DigitalChannelSettings},
   {ST_METRONOME_4, MT_METRONOME_4,   DigitalChannelSettings},
   {ST_METRONOME_5, MT_METRONOME_5,   DigitalChannelSettings},            
   {ST_METRONOME_6, MT_METRONOME_6,   DigitalChannelSettings},
   {ST_METRONOME_7, MT_METRONOME_7,   DigitalChannelSettings},
   {ST_METRONOME_8, MT_METRONOME_8,   DigitalChannelSettings},
 	/* Keypad Menus */
   {ST_KEYPAD_1, MT_KEYPAD_1,   KeypadButtonSettings},
   {ST_KEYPAD_2, MT_KEYPAD_2,   KeypadButtonSettings},
   {ST_KEYPAD_3, MT_KEYPAD_3,   KeypadButtonSettings},
   {ST_KEYPAD_4, MT_KEYPAD_4,   KeypadButtonSettings},
   {ST_KEYPAD_5, MT_KEYPAD_5,   KeypadButtonSettings},            
   {ST_KEYPAD_6, MT_KEYPAD_6,   KeypadButtonSettings},
   {ST_KEYPAD_7, MT_KEYPAD_7,   KeypadButtonSettings},
   {ST_KEYPAD_8, MT_KEYPAD_8,   KeypadButtonSettings},
   {ST_KEYPAD_9, MT_KEYPAD_9,   KeypadButtonSettings},
   {ST_KEYPAD_10, MT_KEYPAD_10, KeypadButtonSettings},    
   {ST_KEYPAD_11, MT_KEYPAD_11, KeypadButtonSettings},
   {ST_KEYPAD_12, MT_KEYPAD_12, KeypadButtonSettings},
   {ST_KEYPAD_13, MT_KEYPAD_13, KeypadButtonSettings},
   {ST_KEYPAD_14, MT_KEYPAD_14, KeypadButtonSettings},
   {ST_KEYPAD_15, MT_KEYPAD_15, KeypadButtonSettings}, 
   {ST_KEYPAD_16, MT_KEYPAD_16, KeypadButtonSettings},
	 
	{ST_FIXED_GAIN, MT_FIXED_GAIN, SensorInputChange},
	{ST_VARIABLE_GAIN, MT_VARIABLE_GAIN, SensorInputChange},
		
   {ST_SAVE_PROFILE, MT_SAVE_PROFILE, 0},
   {ST_LOAD_PROFILE, MT_LOAD_PROFILE, 0},
   
   {ST_LOAD_PROFILE_1, MT_PROFILE_1, LoadProfile},
   {ST_LOAD_PROFILE_2, MT_PROFILE_2, LoadProfile},
   {ST_LOAD_PROFILE_3, MT_PROFILE_3, LoadProfile},
   {ST_LOAD_PROFILE_4, MT_PROFILE_4, LoadProfile},
   {ST_LOAD_PROFILE_DEF, MT_PROFILE_DEF, LoadProfile},			   
   
   {ST_SAVE_PROFILE_1, MT_PROFILE_1, SaveProfile},
   {ST_SAVE_PROFILE_2, MT_PROFILE_2, SaveProfile},
   {ST_SAVE_PROFILE_3, MT_PROFILE_3, SaveProfile},
   {ST_SAVE_PROFILE_4, MT_PROFILE_4, SaveProfile},   
   
   {ST_CHANGE_CHANNEL_CODE, MT_CHANGE_CHANNEL_CODE, ChangeChannelCode},
   
   {0, 0, 0}
};
  
const menu_list AnalogueChannelSetupMenu[] = {
   
   {ST_ANALOGUE_SETUP, ST_THRESHOLD, 3},
   {ST_ANALOGUE_SETUP, ST_RETRIGGER, 4},
   {ST_ANALOGUE_SETUP, ST_SETGAIN, 5},
   {ST_ANALOGUE_SETUP, ST_DUALINPUT, 6},
   {0, 0, 0} 
};

const menu_list DigitalChannelSetupMenu[] = {
   
   {ST_DIGITAL_SETUP, ST_RETRIGGER, 3},
   {ST_DIGITAL_SETUP, ST_TRIGGER_TYPE, 4},  
   {0, 0, 0} 
};

const menu_data ChannelSetupMenu[] = {
   
   {ST_ANALOGUE_SETUP, 0, HandleSubMenu},
   {ST_DIGITAL_SETUP, 0, HandleSubMenu},
	{ST_THRESHOLD, MT_THRESHOLD, SetThreshold},
	{ST_RETRIGGER, MT_RETRIGGER, SetRetrigger},   
	{ST_SETGAIN, MT_SETGAIN, SetGainCurves},	
	{ST_DUALINPUT, MT_DUALINPUT, SetDualInput},
   {ST_TRIGGER_TYPE, MT_TRIGGER_TYPE, SetSwitchType},
   {0, 0, 0}
};  
  

Menu_t primaryMenu = {ST_MAIN, 0, 0, WINDOW_SIZE, WINDOW_SIZE, 0 , 1 , &primaryMenu, 0, (const menu_list*)&MenuState, (const menu_data*)&MenuData};
Menu_t analogueMenu = {ST_ANALOGUE_SETUP, 0, 3, WINDOW_SIZE, WINDOW_SIZE, 0 , 1 , &primaryMenu, 0, 
                       (const menu_list*)&AnalogueChannelSetupMenu, (const menu_data*)&ChannelSetupMenu};
                       
Menu_t digitalMenu = {ST_DIGITAL_SETUP, 0, 3, WINDOW_SIZE, WINDOW_SIZE, 0 , 1 , &primaryMenu, 0,
                       (const menu_list*)&DigitalChannelSetupMenu, (const menu_data*)&ChannelSetupMenu};                       

Menu_t* ActiveMenu = &primaryMenu;
Menu_t* SelectedSubMenu = &primaryMenu;

void MenuSetDisplay(Menu_t* menu, uint8_t display)
{
   if(display == MENU_LCD)
   {
      menu->MenuPrint = MenuLCD_String;
      menu->MenuPrint_P = MenuLCD_String_P;
      menu->MenuNewLine = LCD_NewLine;
      menu->MenuReset = LCD_Reset;
      menu->MenuChar = MenuLCD_Char;
      menu->MenuSetPos = MenuLCD_SetPos;
   }
   /* Route output to UART */
   else
   {
      menu->MenuPrint = MenuUartTxString;
      menu->MenuPrint_P = MenuUartTxString_P;
      menu->MenuNewLine = MenuUart_NewLine;
      menu->MenuReset = MenuUart_Reset;
		menu->MenuChar = UART_Tx;      
   }
}

void MenuLCD_SetPos(uint8_t row, uint8_t col)
{
   UI_LCD_Pos(&PrimaryDisplay, row, col);
}

void  MenuLCD_String(char* string)
{
   UI_LCD_String(&PrimaryDisplay, string);
}


void  MenuLCD_String_P(const char* string)
{
   UI_LCD_String_P(&PrimaryDisplay, string);
}

void MenuLCD_Char(char data)
{
   UI_LCD_Char(&PrimaryDisplay , data);
}

void LCD_NewLine(void)
{
   UI_LCD_Pos(&PrimaryDisplay, PrimaryDisplay.RowPos + 1, 0);
}

void LCD_Reset(void)
{
   UI_LCD_Clear(&PrimaryDisplay);
   UI_LCD_Pos(&PrimaryDisplay, 0, 0);
}

/* Uart Functions also route to the LCD */
void MenuUart_NewLine(void)
{
   UART_TxString_P( PSTR("\n\r") );
   LCD_NewLine();
}


void MenuUartTxString(char* string)
{
   UART_TxString(string);
   UI_LCD_String(&PrimaryDisplay, string);
}

void MenuUartTxString_P(const char* string)
{
   UART_TxString_P(string);
   UI_LCD_String_P(&PrimaryDisplay, string);
}

void MenuUart_Reset(void)
{
   uint8_t i;
 
   for( i = 0; i < 30; i++)
   {
      MenuUart_NewLine();
   }
   LCD_Reset();
}

/* Goes up one level and displays the fixed SubMenu Text */
void Menu_UpOneLevel(Menu_t* menu)
{	
   MenuSetInput(menu, KB_BACK);
   stateMachine(menu, menu->currentState);
   MenuSetInput(menu, KP_UPDATE);   
   executeState(menu, menu->currentState); 	
   //menu->firstEnter = 1;
   ActiveMenu = menu->parentMenu;
}

