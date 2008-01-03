
#include <stdint.h>
#include "Menu.h"
#include "UART/uart.h"
#include "UserFunctions/userFunctions.h"


#if MENU_DEBUG == 0
#include "UI_LCD/UI_LCD.h"
#include "UI_KP/UI_KP.h"
#include <string.h>
#endif


uint8_t currentState = ST_MAIN;
uint8_t RowPosition = 0;
static uint8_t MenuInput;
uint8_t selectedItem = 0;
uint8_t upperLimit = WINDOW_SIZE;
uint8_t lowerLimit = 0;
uint8_t firstEnter = 1;


const MENU_TEXT  MT_PROFILES[] = "Profiles";
const MENU_TEXT  MT_LOAD_PROFILE[] = "Load Profile";
const MENU_TEXT  MT_SAVE_PROFILE[] = "Save Profile As...";

const MENU_TEXT  MT_PROFILE_1[] = "Profile 1";
const MENU_TEXT  MT_PROFILE_2[] = "Profile 2";
const MENU_TEXT  MT_PROFILE_3[] = "Profile 3";
const MENU_TEXT  MT_PROFILE_4[] = "Profile 4";
const MENU_TEXT  MT_PROFILE_DEF[] = "Default Profile";


const MENU_TEXT  MT_VUMETER[] = "VU Meter";


const MENU_TEXT  MT_OPTIONS[] = "Options";
const MENU_TEXT  MT_MIDI_OUTPUT_RATE[] = "MIDI Output Rate";
const MENU_TEXT  MT_SET_RATE[] = "Edit Output Rate";
const MENU_TEXT  MT_CHANGE_CHANNEL_CODE[] = "Edit Channel Code";
const MENU_TEXT  MT_CHANNEL_SETUP[] = "Channel Settings";
const MENU_TEXT  MT_INPUT_SELECT[] = "Amp. Input Select";

const MENU_TEXT  MT_ANALOGUE_INPUTS[] = "Analogue Inputs";
const MENU_TEXT  MT_DIGITAL_INPUTS[] = "Digital Inputs";

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

   {ST_MAIN,   ST_OPTIONS,   0},
   {ST_MAIN, 	ST_PROFILES,  1},
   {ST_MAIN,	ST_VUMETER,	  2},
   {ST_OPTIONS, ST_MIDI_OUTPUT_RATE,  0},
   {ST_OPTIONS, ST_CHANNEL_SETUP, 1},
   {ST_OPTIONS, ST_CHANGE_CHANNEL_CODE, 2},
   {ST_OPTIONS, ST_INPUT_SELECT, 3},
	
	{ST_INPUT_SELECT, ST_FIXED_GAIN, 2},
	{ST_INPUT_SELECT, ST_VARIABLE_GAIN, 3},		
		
   {ST_MIDI_OUTPUT_RATE, ST_SET_RATE, 1},

   {ST_CHANNEL_SETUP, ST_ANALOGUE_INPUTS,  0},
   {ST_CHANNEL_SETUP, ST_DIGITAL_INPUTS,  1},   

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
		   
	{ST_CHANNEL_1, ST_THRESHOLD_1, 3},
	{ST_CHANNEL_2, ST_THRESHOLD_2, 3},
	{ST_CHANNEL_3, ST_THRESHOLD_3, 3},
	{ST_CHANNEL_4, ST_THRESHOLD_4, 3},
	{ST_CHANNEL_5, ST_THRESHOLD_5, 3},
	{ST_CHANNEL_6, ST_THRESHOLD_6, 3},
	{ST_CHANNEL_7, ST_THRESHOLD_7, 3},
	{ST_CHANNEL_8, ST_THRESHOLD_8, 3},
	{ST_CHANNEL_9, ST_THRESHOLD_9, 3},
	{ST_CHANNEL_10, ST_THRESHOLD_10, 3},
	{ST_CHANNEL_11, ST_THRESHOLD_11, 3},
	{ST_CHANNEL_12, ST_THRESHOLD_12, 3},
	{ST_CHANNEL_13, ST_THRESHOLD_13, 3},
	{ST_CHANNEL_14, ST_THRESHOLD_14, 3},
	{ST_CHANNEL_15, ST_THRESHOLD_15, 3},
	{ST_CHANNEL_16, ST_THRESHOLD_16, 3},

	/* Analogue Retrigger Menu's */	
	{ST_CHANNEL_1, ST_RETRIGGER_1, 4},
	{ST_CHANNEL_2, ST_RETRIGGER_2, 4},
	{ST_CHANNEL_3, ST_RETRIGGER_3, 4},
	{ST_CHANNEL_4, ST_RETRIGGER_4, 4},
	{ST_CHANNEL_5, ST_RETRIGGER_5, 4},
	{ST_CHANNEL_6, ST_RETRIGGER_6, 4},
	{ST_CHANNEL_7, ST_RETRIGGER_7, 4},
	{ST_CHANNEL_8, ST_RETRIGGER_8, 4},
	{ST_CHANNEL_9, ST_RETRIGGER_9, 4},
	{ST_CHANNEL_10, ST_RETRIGGER_10, 4},
	{ST_CHANNEL_11, ST_RETRIGGER_11, 4},
	{ST_CHANNEL_12, ST_RETRIGGER_12, 4},
	{ST_CHANNEL_13, ST_RETRIGGER_13, 4},
	{ST_CHANNEL_14, ST_RETRIGGER_14, 4},
	{ST_CHANNEL_15, ST_RETRIGGER_15, 4},
	{ST_CHANNEL_16, ST_RETRIGGER_16, 4},
	
	/* Gain Curve Screens */
	{ST_CHANNEL_1, ST_SETGAIN_1, 5},
	{ST_CHANNEL_2, ST_SETGAIN_2, 5},
	{ST_CHANNEL_3, ST_SETGAIN_3, 5},
	{ST_CHANNEL_4, ST_SETGAIN_4, 5},
	{ST_CHANNEL_5, ST_SETGAIN_5, 5},
	{ST_CHANNEL_6, ST_SETGAIN_6, 5},
	{ST_CHANNEL_7, ST_SETGAIN_7, 5},
	{ST_CHANNEL_8, ST_SETGAIN_8, 5},
	{ST_CHANNEL_9, ST_SETGAIN_9, 5},
	{ST_CHANNEL_10, ST_SETGAIN_10, 5},
	{ST_CHANNEL_11, ST_SETGAIN_11, 5},
	{ST_CHANNEL_12, ST_SETGAIN_12, 5},
	{ST_CHANNEL_13, ST_SETGAIN_13, 5},
	{ST_CHANNEL_14, ST_SETGAIN_14, 5},
	{ST_CHANNEL_15, ST_SETGAIN_15, 5},
	{ST_CHANNEL_16, ST_SETGAIN_16, 5},	
	
	/* Analgoue Dual Input Menu's */
	{ST_CHANNEL_1, ST_DUALINPUT_1, 6},
	{ST_CHANNEL_2, ST_DUALINPUT_2, 6},
	{ST_CHANNEL_3, ST_DUALINPUT_3, 6},
	{ST_CHANNEL_4, ST_DUALINPUT_4, 6},
	{ST_CHANNEL_5, ST_DUALINPUT_5, 6},
	{ST_CHANNEL_6, ST_DUALINPUT_6, 6},
	{ST_CHANNEL_7, ST_DUALINPUT_7, 6},
	{ST_CHANNEL_8, ST_DUALINPUT_8, 6},
	{ST_CHANNEL_9, ST_DUALINPUT_9, 6},
	{ST_CHANNEL_10, ST_DUALINPUT_10, 6},
	{ST_CHANNEL_11, ST_DUALINPUT_11, 6},
	{ST_CHANNEL_12, ST_DUALINPUT_12, 6},
	{ST_CHANNEL_13, ST_DUALINPUT_13, 6},
	{ST_CHANNEL_14, ST_DUALINPUT_14, 6},
	{ST_CHANNEL_15, ST_DUALINPUT_15, 6},
	{ST_CHANNEL_16, ST_DUALINPUT_16, 6},


	/* Digital Config Menu's */
   {ST_DIGITAL_INPUTS, ST_DIGITAL_1,  0},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_2,  1},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_3,  2},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_4,  3},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_5,  4},            
   {ST_DIGITAL_INPUTS, ST_DIGITAL_6,  5},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_7,  6},
   {ST_DIGITAL_INPUTS, ST_DIGITAL_8,  7},

	{ST_DIGITAL_1,ST_TRIGGER_TYPE_D1, 3},
	{ST_DIGITAL_2,ST_TRIGGER_TYPE_D2, 3},
	{ST_DIGITAL_3,ST_TRIGGER_TYPE_D3, 3},
	{ST_DIGITAL_4,ST_TRIGGER_TYPE_D4, 3},
	{ST_DIGITAL_5,ST_TRIGGER_TYPE_D5, 3},
	{ST_DIGITAL_6,ST_TRIGGER_TYPE_D6, 3},
	{ST_DIGITAL_7,ST_TRIGGER_TYPE_D7, 3},
	{ST_DIGITAL_8,ST_TRIGGER_TYPE_D8, 3},

	/* Digital Retrigger Menu's */
	{ST_DIGITAL_1,ST_RETRIGGER_D1, 4},
	{ST_DIGITAL_2,ST_RETRIGGER_D2, 4},
	{ST_DIGITAL_3,ST_RETRIGGER_D3, 4},
	{ST_DIGITAL_4,ST_RETRIGGER_D4, 4},
	{ST_DIGITAL_5,ST_RETRIGGER_D5, 4},
	{ST_DIGITAL_6,ST_RETRIGGER_D6, 4},
	{ST_DIGITAL_7,ST_RETRIGGER_D7, 4},
	{ST_DIGITAL_8,ST_RETRIGGER_D8, 4},


   {ST_PROFILES, ST_LOAD_PROFILE, 2},
   {ST_PROFILES, ST_SAVE_PROFILE, 3},
   {ST_PROFILES, ST_LOAD_PROFILE, 4},
		
	{ST_LOAD_PROFILE, ST_LOAD_PROFILE_1, 0},	   
	{ST_LOAD_PROFILE, ST_LOAD_PROFILE_2, 1},
	{ST_LOAD_PROFILE, ST_LOAD_PROFILE_3, 2},
	{ST_LOAD_PROFILE, ST_LOAD_PROFILE_4, 3},
	{ST_LOAD_PROFILE, ST_LOAD_PROFILE_DEF, 4},
	
	{ST_SAVE_PROFILE, ST_SAVE_PROFILE_1, 0},	   
	{ST_SAVE_PROFILE, ST_SAVE_PROFILE_2, 1},
	{ST_SAVE_PROFILE, ST_SAVE_PROFILE_3, 2},
	{ST_SAVE_PROFILE, ST_SAVE_PROFILE_4, 3},
		
			   
   {0, 0, 0}
};





const menu_data MenuData[] = {
   {ST_MAIN, 0, 0},
   {ST_PROFILES, MT_PROFILES, ShowProfile},   
   {ST_OPTIONS, MT_OPTIONS, 0},
	{ST_VUMETER, MT_VUMETER, VUMeterSetup},   
   {ST_MIDI_OUTPUT_RATE, MT_MIDI_OUTPUT_RATE, SetMIDIRate},
   {ST_SET_RATE, MT_SET_RATE, EditMIDIRate},
   {ST_INPUT_SELECT, MT_INPUT_SELECT, AmpInputSelect},
 	{ST_CHANNEL_SETUP, MT_CHANNEL_SETUP, 0},
 	
   {ST_ANALOGUE_INPUTS, MT_ANALOGUE_INPUTS,  0},
   {ST_DIGITAL_INPUTS, MT_DIGITAL_INPUTS,  0},    	
 	
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
   
   /* Analogue Threshold Menu */
	{ST_THRESHOLD_1, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_2, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_3, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_4, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_5, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_6, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_7, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_8, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_9, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_10, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_11, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_12, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_13, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_14, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_15, MT_THRESHOLD, SetThreshold},
	{ST_THRESHOLD_16, MT_THRESHOLD, SetThreshold},
	
	/* Analogue Retrigger Menus */
	{ST_RETRIGGER_1, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_2, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_3, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_4, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_5, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_6, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_7, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_8, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_9, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_10, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_11, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_12, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_13, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_14, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_15, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_16, MT_RETRIGGER, SetRetrigger},

	/* Gain Set Screens */
	{ST_SETGAIN_1, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_2, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_3, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_4, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_5, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_6, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_7, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_8, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_9, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_10, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_11, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_12, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_13, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_14, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_15, MT_SETGAIN, SetGainCurves},
	{ST_SETGAIN_16, MT_SETGAIN, SetGainCurves},
	
	/* Analogue Dual Input Menu */
	{ST_DUALINPUT_1, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_2, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_3, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_4, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_5, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_6, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_7, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_8, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_9, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_10, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_11, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_12, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_13, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_14, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_15, MT_DUALINPUT, SetDualInput},
	{ST_DUALINPUT_16, MT_DUALINPUT, SetDualInput},

	
	/* Digital Menus */
   {ST_DIGITAL_1, MT_DIGITAL_1,   DigitalChannelSettings},
   {ST_DIGITAL_2, MT_DIGITAL_2,   DigitalChannelSettings},
   {ST_DIGITAL_3, MT_DIGITAL_3,   DigitalChannelSettings},
   {ST_DIGITAL_4, MT_DIGITAL_4,   DigitalChannelSettings},
   {ST_DIGITAL_5, MT_DIGITAL_5,   DigitalChannelSettings},            
   {ST_DIGITAL_6, MT_DIGITAL_6,   DigitalChannelSettings},
   {ST_DIGITAL_7, MT_DIGITAL_7,   DigitalChannelSettings},
   {ST_DIGITAL_8, MT_DIGITAL_8,   DigitalChannelSettings},

	/* Digital Switch type setup */
	{ST_TRIGGER_TYPE_D1, MT_TRIGGER_TYPE, SetSwitchType},
	{ST_TRIGGER_TYPE_D2, MT_TRIGGER_TYPE, SetSwitchType},
	{ST_TRIGGER_TYPE_D3, MT_TRIGGER_TYPE, SetSwitchType},
	{ST_TRIGGER_TYPE_D4, MT_TRIGGER_TYPE, SetSwitchType},
	{ST_TRIGGER_TYPE_D5, MT_TRIGGER_TYPE, SetSwitchType},
	{ST_TRIGGER_TYPE_D6, MT_TRIGGER_TYPE, SetSwitchType},
	{ST_TRIGGER_TYPE_D7, MT_TRIGGER_TYPE, SetSwitchType},
	{ST_TRIGGER_TYPE_D8, MT_TRIGGER_TYPE, SetSwitchType},
		
	/* Digital Retriggers */
	{ST_RETRIGGER_D1, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_D2, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_D3, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_D4, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_D5, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_D6, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_D7, MT_RETRIGGER, SetRetrigger},
	{ST_RETRIGGER_D8, MT_RETRIGGER, SetRetrigger},

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

void MenuSetDisplay(uint8_t display)
{
   if(display == MENU_LCD)
   {
      MenuPrint = UI_LCD_String;
      MenuPrint_P = UI_LCD_String_P;
      MenuNewLine = LCD_NewLine;
      MenuReset = LCD_Reset;
   }
   /* Route output to UART */
   else
   {
      MenuPrint = MenuUartTxString;
      MenuPrint_P = MenuUartTxString_P;
      MenuNewLine = MenuUart_NewLine;
      MenuReset = MenuUart_Reset;      
   }
}





void LCD_NewLine(void)
{
   UI_LCD_Pos(++RowPosition, 0);
}

void LCD_Reset(void)
{
   UI_LCD_Clear();
   UI_LCD_Pos(0, 0);
   RowPosition = 0;
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
   UI_LCD_String(string);
}

void MenuUartTxString_P(const char* string)
{
   UART_TxString_P(string);
   UI_LCD_String_P(string);
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

/** Updates the state of the menu */
void MenuUpdate(void)
{
   uint8_t i;   
   uint8_t sequenceIndex = 0;
   uint8_t MenuOffset;
   uint8_t MenuMax;
   
   char* outputString;
   
   MenuReset();

   /** Only switch Menu input IF we are in a menu item which has NO associated
    * function */
   if( MenuData[ GetIndex(currentState) ].function == 0 )
   {
      stateMachine( currentState ); 
   }
        
   /** Run the associated function if it has one */   
   executeState(currentState);    

   MenuOffset = SmallestSequence(currentState);
   MenuMax = LargestSequence(currentState);

   
   /** Print out the menu's sub-menu's */   
   /* Ensures that the screen limits are not exceeded */
   for( i = 0, sequenceIndex = 0;  MenuState[i].parent != 0 ; i++)
   {  
      /* Find the current state's sub children. */        
      if( MenuState[i].parent == currentState )
      {  
         outputString = MenuDescriptor( MenuState[i].child );
         sequenceIndex = MenuState[i].sequence;
         
         
         if( selectedItem > upperLimit )
         {
            upperLimit = selectedItem;
            lowerLimit = upperLimit - WINDOW_SIZE + MenuOffset;   
         }
            
         if( selectedItem < lowerLimit )
         {
            lowerLimit = selectedItem;
            upperLimit = lowerLimit + WINDOW_SIZE - MenuOffset;               
         }
         
         /* If this is the selected item then prefix an asterix */
         if( (sequenceIndex <= upperLimit) && (sequenceIndex >= lowerLimit) )
         {
            if(sequenceIndex > WINDOW_SIZE)
            {
               UI_LCD_Pos(0, 19);
               MenuPrint_P( PSTR("^") );               
            }
            
            if( MenuMax - sequenceIndex + MenuOffset  > WINDOW_SIZE)
            {
               UI_LCD_Pos(3, 19);
               MenuPrint_P( PSTR("v") );
            }
            
            UI_LCD_Pos(RowPosition, 0); 
            
            if( MenuState[i].sequence == selectedItem)
            {
#if MENU_DEBUG == 1  
               printf("*");    
            }
            else
            {
               printf(" ");   
            }
#else
               MenuPrint_P( PSTR("*") );    
            }
            else
            {
               MenuPrint_P( PSTR(" ") );   
            }
#endif
                       

#if MENU_DEBUG == 1                  
            if( outputString )
            {
               PRINT_FUNC("%s\n", (uint8_t*)outputString );
            }
#else
            MenuPrint((char*)outputString);
            MenuNewLine();            
#endif            
         }
      }  
   }
}


/** Determines the new state depending on the user input and passed state */
void stateMachine(uint8_t state)
{
   uint8_t maxStateItems;
   uint8_t parentIndex = 0;
   
   
   
   maxStateItems = SubItems(state);
   
   

   switch( MenuInput )
   {
      case KB_UP:
      case KP_UP:
         if(!(selectedItem <= SmallestSequence(currentState)) )
         {
            selectedItem--;
         }
      break;
         
      case KB_DOWN:   
      case KP_DOWN:
         selectedItem++;          
         if( selectedItem - SmallestSequence(currentState) > maxStateItems )
         {
            selectedItem = LargestSequence(currentState);
         }
      break;
      
      case KB_ENTER:
      case KP_ENTER:
         /* Go into child sub menu */
         currentState = GetMenuState(currentState, selectedItem);
         selectedItem = SmallestSequence(currentState);
         upperLimit = WINDOW_SIZE;
         lowerLimit = 0;
         firstEnter = 1; 
         
      break;
      
      case KB_BACK:
      case KP_BACK: 
         /** Need to reset the 'first' enter flag */
         /** This is so that the commands of a function are not
          * executed on entering the associated sub-menu */
         firstEnter = 1;                
         parentIndex = GetParent(currentState);
         lowerLimit = 0;
         upperLimit = WINDOW_SIZE;
         if(parentIndex != INVALID_STATE)
         {
            upperLimit = WINDOW_SIZE;
            lowerLimit = 0;
            
            currentState = MenuState[ parentIndex ].parent;
            selectedItem = MenuState[ parentIndex ].sequence; 
         }
      break;     
      
      default:
      break;   
   }

   /* Revert back to initial state */
   if(currentState == NO_STATE || parentIndex == INVALID_STATE)
   {
      currentState = state;       
   }
}

/* Returns the Menu descriptor given the menu Item / Index */
char* MenuDescriptor(uint8_t menuItem)
{

   uint8_t index;
   static char buffer[21];
      
   index = GetIndex(menuItem);
   
   if( index  != INVALID_STATE )
   {
      strcpy(buffer, MenuData[index].descriptor);
      return buffer;
   }
   return 0;
}


/* Returns the corresponding MenuState 
 * given the Parent State and its sequnce. */
uint8_t GetMenuState(uint8_t state, uint8_t Sequence)
{
   int i;
   for( i = 0; MenuState[i].parent; i++)
   {
      if( MenuState[i].parent == state
       && MenuState[i].sequence == Sequence ){      
         return MenuState[i].child;
      }
   }
   return NO_STATE;
}

/* Get out the first instance of where the given state
 * origninates from */
uint8_t GetParent(uint8_t state)
{
 
   int i;  
   for( i = 0; MenuState[i].parent; i++)
   {
      if( MenuState[i].child == state) {      
         return i;
      }
   }
   return INVALID_STATE;
}

/* Updates the input last received by the menu */
void MenuSetInput(uint8_t NewInput)
{
   MenuInput = NewInput;    
}

/** Calls the associated menu's function */
void executeState(uint8_t state)
{
   uint8_t index;
   void (*funcPtr)(void* data);
   
   index = GetIndex(state);
   
   if(index != INVALID_STATE)
   {
      if( MenuData[index].function != 0)
      {
         funcPtr = (void*)MenuData[index].function;
         funcPtr(&MenuInput);
      }
   }
   
   
}

/*& Get the element in MenuData which has 'parent' */
uint8_t GetIndex(uint8_t parent)
{
   uint8_t i;
   for( i = 0; MenuData[i].menu_item; i++)
   {
      if( MenuData[i].menu_item == parent)
      {
         return i;   
      } 
   }
   
   return INVALID_STATE;
   
}

/** Get the number of sub items in the passed state */
uint8_t SubItems(uint8_t state)
{
   int i;
   uint8_t StateItems = 0;    
   for( i = 0; MenuState[i].parent; i++)
   {
      if( MenuState[i].parent == state)
      {
         StateItems++;
      }
   }
   return (StateItems - 1);
}

/** Get the maximum sequence of the the passed state */
uint8_t LargestSequence(uint8_t state)
{
   int i;
   uint8_t StateItem = 0;    
   for( i = 0; MenuState[i].parent; i++)
   {
      if( MenuState[i].parent == state)
      {
         /* Obtain the number of Menu Items in the given state */
         if( MenuState[i].sequence >= StateItem )
         {
            StateItem = MenuState[i].sequence;
         } 
      }
   }
   return (StateItem);
}

uint8_t SmallestSequence(uint8_t state)
{
   int i;
   uint8_t StateItem = 0xFF;    
   for( i = 0; MenuState[i].parent; i++)
   {
      if( MenuState[i].parent == state)
      {
         /* Obtain the number of Menu Items in the given state */
         if( MenuState[i].sequence <= StateItem )
         {
            StateItem = MenuState[i].sequence;
         } 
      }
   }
   return (StateItem);
}


/* Returns the sequence given the menu state */
uint8_t GetSequence(uint8_t parent, uint8_t child)
{
   uint8_t parentIndex;
   uint8_t i;
   
   parentIndex = GetIndex(parent);
   

   for( i = 0; MenuState[i].parent; i++)
   {
      if( MenuState[i].parent == parent &&
          MenuState[i].child  == child )
      {
         return MenuState[i].sequence;     
      }
   }      
   return INVALID_SEQUENCE;
}


uint8_t GetState(void)
{
   return currentState;  
}