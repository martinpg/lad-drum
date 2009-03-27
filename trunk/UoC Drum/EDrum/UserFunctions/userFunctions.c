
#include <io.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "main.h"
#include "Menu/Menu.h"
#include "UI_KP/UI_KP.h"
#include "UI_LCD/UI_LCD.h"
#include "UART/uart.h"
#include "UserFunctions/userFunctions.h"
#include "mmculib/uint16toa.h"
#include "mmculib/uint8toa.h"
#include "MIDI/midi.h"
#include "MIDI/SysEx/SysEx.h"
#include "Sample/sample.h"
#include "Sensor/sensor.h"
#include "ADC/adc12.h"
#include "Delay/delay.h"
#include "Profiles/profiles.h"
#include "VUMeter/vumeter.h"
#include "MenuSettings.h"
#include "LCDSettings.h"
#include "ControllerMode/ControllerMode.h"

static uint8_t SelectedProfile = DEFAULT_PROFILE;
static uint8_t SelectedChannel = 0;

void reset(void* data)
{
	_reset_vector__();
}

/* Menu wrapper functions */
void UF_MenuSetInput(uint8_t NewInput)
{
   MenuSetInput(&primaryMenu, NewInput);
}

void UF_stateMachine(uint8_t CurrentState)
{
   stateMachine(&primaryMenu, CurrentState);
}

void UF_executeState(uint8_t state)
{
   executeState(&primaryMenu, state);
}

void UF_MenuReset(void)
{
   primaryMenu.MenuReset();
}

void UF_MenuNewLine(void)
{
   primaryMenu.MenuNewLine();
}

void UF_MenuChar(uint8_t data)
{
   primaryMenu.MenuChar(data);
}

void UF_MenuPrint_P(const char* string)
{
   primaryMenu.MenuPrint_P(string);  
}   

void UF_MenuPrint(char* string)
{
   primaryMenu.MenuPrint(string);    
}

void UF_MenuUpOneLevel(Menu_t* menu)
{
   Menu_UpOneLevel(menu);
}

/* End of wrapper functions */
   

void about(void* data)
{
	uint8_t* input = data;

	if( primaryMenu.firstEnter != 1 )
	{
		switch( *input )
		{	
	      case KP_BACK:      
			UI_LCD_LoadDefaultChars();	
	   	UF_MenuSetInput(KP_BACK);
	      UF_stateMachine(primaryMenu.currentState);
	      UF_MenuSetInput(0);
			SoftTimerStop(SoftTimer2[SC_AboutUpdate]);
			ThanksIndex(MAIN_SCREEN);       
	      return;	
		}	
	}
	
	primaryMenu.firstEnter = 0;
	SoftTimerStart(SoftTimer2[SC_AboutUpdate]);
	aboutScroll(ThanksIndex(GET));	
}




/* Pass GET to obtain the index, otherwise the index is set to the passed
 * parameter */
uint8_t ThanksIndex(uint8_t mode)
{
	static uint8_t nameIndex = MAIN_SCREEN;
	if( mode == GET )
	{
		return 	nameIndex;
	}
	else
	{
		nameIndex = mode;
		return mode;
	}
}

void aboutScroll(uint8_t nameIndex)
{
	if( nameIndex > SIZEOFTHANKS )
	{
		nameIndex = MAIN_SCREEN;	
	}

	const uint8_t lightning[][8] = {{158,143,135,140,156,142,130,129},
											  {143,158,156,134,135,142,136,144}};
	
	
	switch( nameIndex )
	{
		case MAIN_SCREEN:

			UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)lightning[0], 0);
			UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)lightning[1], 1);
			
			UF_MenuReset();
			UF_MenuPrint_P( PSTR("     FuzzyJohn"));
			UF_MenuNewLine();
			UF_MenuPrint_P(PSTR("   Inc. presents:") );
			UF_MenuNewLine();	
			UF_MenuChar(0x00);
			UF_MenuPrint_P( PSTR(" L-A-D eDrum"));
			UF_MenuChar(0x01);
			UF_MenuPrint_P( PSTR(" 2008"));	
		   UF_MenuNewLine();
			UF_MenuPrint_P(PSTR("Version:") );
			UF_MenuPrint_P(VersionId);		   
		break;
		
		case CREATORS_SCREEN:
			UF_MenuReset();		
			UF_MenuPrint_P( PSTR("Designer: Adrian Gin"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("Supervised by:"));
			UF_MenuNewLine();						
			UF_MenuPrint_P( PSTR("Dr Larry Brackney"));
			UF_MenuNewLine();				
			UF_MenuPrint_P( PSTR("Jullada Homtientong"));								
		break;
		
		case THANKS_SCREEN:
			UF_MenuReset();		
			UF_MenuPrint_P( PSTR("Special Thanks to:"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("Rowan 'Robot' Sinton"));
			UF_MenuNewLine();						
			UF_MenuPrint_P( PSTR("Shreejan Pandey"));
			UF_MenuNewLine();				
			UF_MenuPrint_P( PSTR("Tic-Sieu How"));									
		break;
		
		case THANKS2_SCREEN:
			UF_MenuReset();	
			UF_MenuPrint_P( PSTR("Ma & Ba"));
			UF_MenuNewLine();					
			UF_MenuPrint_P( PSTR("Bus,Ry,Tim,DJ Doboy"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("Bob Sinclair"));
			UF_MenuNewLine();						
			UF_MenuPrint_P( PSTR("Elec. Eng Department"));									
		break;			
		
		case THANKS3_SCREEN:
			UF_MenuReset();		
			UF_MenuPrint_P( PSTR("Phil Hof (DSL)"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("Malcolm Gordon"));
			UF_MenuNewLine();							
			UF_MenuPrint_P( PSTR("CAE2 Master"));
			UF_MenuNewLine();						
			UF_MenuPrint_P( PSTR("PCB Maker & Dudley B"));				
		break;	
		
		case THANKS4_SCREEN:
			UF_MenuReset();		
			UF_MenuPrint_P( PSTR("Slinkee Minx"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("Special D"));
			UF_MenuNewLine();							
			UF_MenuPrint_P( PSTR("Cosmic Gate & JJ"));
			UF_MenuNewLine();						
			UF_MenuPrint_P( PSTR("Siria"));				
		break;	
		
		case INSPIRATION_SCREEN:
			UF_MenuReset();		
			UF_MenuPrint_P( PSTR("Inspiration from:"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("eDrum.info"));
			UF_MenuNewLine();							
			UF_MenuPrint_P( PSTR("Megadrum, Toontrack"));
			UF_MenuNewLine();						
			UF_MenuPrint_P( PSTR("Smartie LCD"));			
		break;
		
		case INSPIRATION2_SCREEN:
			UF_MenuReset();		
			UF_MenuPrint_P( PSTR("Winamp Plugin"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("for the VU Meters"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("Dr Hamish Laird for"));
			UF_MenuNewLine();	
			UF_MenuPrint_P( PSTR("ENEL427 Supervision"));																
		break;
		
		case INFORMATION_SCREEN:
			UF_MenuReset();		
			UF_MenuPrint_P( PSTR("Built using:"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("Protel DXP"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("Dev-C++ 4.9.9.2"));
			UF_MenuNewLine();	
			UF_MenuPrint_P( PSTR("MSPGCC and MSPFET"));																
		break;	
		
		case INFORMATION2_SCREEN:
			UF_MenuReset();		
			UF_MenuPrint_P( PSTR("Recommended Tools:"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("MIDI-OX"));
			UF_MenuNewLine();
			UF_MenuPrint_P( PSTR("FL Studio 7"));
			UF_MenuNewLine();	
			UF_MenuPrint_P( PSTR("RealTerm for Debug"));																	
		break;												
					
	}
}


void SysExDisplay(void* data)
{
	uint8_t* input = (uint8_t*)data;
	uint8_t outputString[15];
		
	if( primaryMenu.firstEnter != 1 )
   {     
		UF_stateMachine( primaryMenu.currentState );
      switch( *input )
      {
			case KB_BACK:
			case KP_BACK:
				primaryMenu.firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
			   UF_executeState( primaryMenu.currentState);
			   
				return;	
			break;

			default:
			break;
		}	
	}
	
	UF_MenuReset();
	primaryMenu.firstEnter = 0;
	
	/* Since each byte is sent as 2x 7 bit SysEx 'bytes' */
	uint16toa( sizeof(Profile_t) * 2, outputString, 0);
	
   UF_MenuPrint_P( PSTR("SysEx Size:") );

   UF_MenuPrint(outputString);
   UF_MenuPrint_P( PSTR(" bytes") ); 
   
   UF_MenuNewLine(); 
}

void DumpSysEx(void* data)
{
   uint8_t* input = 0;
	uint16_t i;
	   
   input = data;

	UF_MenuPrint_P( PSTR("Sending SysEx Data") );	
   UF_MenuNewLine();		
	UF_MenuPrint_P( PSTR("Sending") );	

	for( i = 0; i < 4; i++ )
	{
		UF_MenuPrint_P( PSTR(".") );		
		_delay_ms(100);
	}
	
   UF_MenuNewLine();		

   SysexSend(&CurrentProfile);
   
	UF_MenuPrint_P( PSTR("Profile sucessfully"));			
   UF_MenuNewLine();		
	UF_MenuPrint_P( PSTR("uploaded!") );

	for( i = 0; i < 4; i++ )
	{	
		_delay_ms(200);
	}	
	
   UF_MenuUpOneLevel(&primaryMenu);  
}



void GetSysEx(void* data)
{
   uint8_t* input = 0;
	uint16_t i;
	   
   input = data;

	UF_MenuPrint_P( PSTR("Ready for SysEx Data") );	
   UF_MenuNewLine();			

	for( i = 0; i < 4; i++ )
	{
		UF_MenuPrint_P( PSTR(".") );		
		_delay_ms(100);
	}
   UF_MenuNewLine();	
	IsReceivingSysExData(RECEIVING_SYSEX_DATA);
	
   
	if( primaryMenu.firstEnter != 1 )
   {     
      switch( *input )
      {
			case KB_BACK:
			case KP_BACK:
				primaryMenu.firstEnter = 1;
				SysExFlush();
            IsReceivingSysExData(!RECEIVING_SYSEX_DATA);
            UF_MenuPrint_P( PSTR("User cancelled!"));	
         	for( i = 0; i < 4; i++ )
         	{	
         		_delay_ms(200);
         	}	
         	
		   	UF_MenuUpOneLevel(&primaryMenu);
         	
				return;
			break;

			default:
			break;
		}	
	}
   ActiveProcess = RECEIVE_SYSEX;
   
   /* Stop the Auxuliary Timer */
	TBCCTL2 &= ~(CCIE);
	TBCCTL0 &= ~(CCIE);
	primaryMenu.firstEnter = 0;
}


/* Play mode disables TimerB2 */
void ControllerMode(void* data)
{
	uint8_t* input = data;

	if( primaryMenu.firstEnter != 1 )
	{
		switch( *input )
		{	
         /* The LCD BL key does not exit however */
         case KP_5:
         
         break;
         
	      default:     
	   	UF_MenuSetInput(KP_BACK);
	      UF_stateMachine(primaryMenu.currentState);
	      UF_MenuSetInput(0);
	      /* Start the Aux Timer again */
			TBCCTL2 |= (CCIE);
	      return;	
		}	
	}
		
	primaryMenu.firstEnter = 0;	
	
	CM_SetMenuMode(CM_SETTINGS_MODE);
	
	CM_printEnteredData();		
	
	/* Only used when KP inputs are buttons 
	for( i = 0; i < ANALOGUE_INPUTS; i++)
	{
		SetLastMIDIValue(i, MIDI_MAX_DATA);	
		SetLastSampleValue(i, MAX_THRESHOLD);	
	}*/
	
	
	/* Don't stop the Auxuliary Timer */
	ActiveProcess = CONTROLLER_MODE;
}




void SetMIDIRate(void* data)
{
	uint8_t* input = (uint8_t*)data;
	//static uint8_t primaryMenu.firstEnter = 1;
	
		
	if( primaryMenu.firstEnter != 1 )
   {     
		UF_stateMachine( primaryMenu.currentState );
      switch( *input )
      {
			case KB_BACK:
			case KP_BACK:
				primaryMenu.firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
			   UF_executeState( primaryMenu.currentState);
				return;	
			break;

			default:
			break;
		}	
	}
	
	UF_MenuReset();
	primaryMenu.firstEnter = 0;
   PrintMIDIRate();
   UF_MenuNewLine(); 
}


void PrintMIDIRate(void)
{
	uint8_t outputString[15];
	uint8_t selectedBaud = 0;
	
	UF_MenuPrint_P( PSTR("MIDI Output Rate: ") );
   UF_MenuNewLine(); 	
	uint16toa( (MIDI_GetRate()), outputString, 0);
	UF_MenuPrint(outputString);
   UF_MenuPrint_P( PSTR(" ms @ "));
   
   switch(MIDI_GetBaud())
   {
		case BAUD_31250:
			selectedBaud = B31250;
		break;
		
		case BAUD_38400:
			selectedBaud = B38400;
		break;
		
		case BAUD_115200:
			selectedBaud = B115200;
		break;
		
		case BAUD_1M:
			selectedBaud = B1M;
		break;				
	
		default:
		break;
	}
      
  	UF_MenuPrint_P( MIDI_BAUD[selectedBaud] );	
}

void EditMIDIRate(void* data)
{
	uint8_t* input = (uint8_t*)data;
	static uint16_t	Delay;
	uint8_t selectedBaud = 0;
	uint8_t outputString[10];
	
	if( primaryMenu.firstEnter == 1)
	{
		Delay = MIDI_GetRate();
	}
	
   if( primaryMenu.firstEnter == 0 )
   {     
      switch( *input )
      {
         /* Increment msec delay */
         case 'q':
         case KP_A:
            if( ++Delay > MAX_MIDI_RATE)
            {
               Delay = 1;   
            }   
         break;
            
         /* Deccrement msec delay */  
         case 'a':                   
         case KP_B:
            if(!--Delay)
            {
               Delay = MAX_MIDI_RATE;   
            }
         break;
         
         case KP_C:
				   switch(MIDI_GetBaud())
				   {
						case BAUD_31250:
							MIDI_SetBaud(BAUD_38400); 
						break;
						
						case BAUD_38400:
							MIDI_SetBaud(BAUD_115200); 
						break;
						
						case BAUD_115200:
							MIDI_SetBaud(BAUD_1M); 
						break;		
						
						case BAUD_1M:
							MIDI_SetBaud(BAUD_31250);
						break;
					
						default:
						break;
					}            
         break;
  
         /* Time Component increment function */
         case KB_BACK:
         case KP_BACK:
				/* Go back up one menu */   
   			UF_MenuUpOneLevel(&primaryMenu);
  				return;
  				
         break;
                        
         default:
         break;     
      }
   }
   
   primaryMenu.firstEnter = 0;
   
   UF_MenuPrint_P( PSTR("Millisecond Delay:"));
	UF_MenuNewLine();
	
	uint16toa( Delay, outputString, 0);
	UF_MenuPrint(outputString);
	UF_MenuPrint_P( PSTR(" milliseconds") );      
	MIDI_SetRate( Delay );

	UF_MenuNewLine();
   UF_MenuPrint_P( PSTR("Baud Rate:"));
   switch(MIDI_GetBaud())
   {
		case BAUD_31250:
			selectedBaud = B31250;
		break;
		
		case BAUD_38400:
			selectedBaud = B38400;
		break;
		
		case BAUD_115200:
			selectedBaud = B115200;
		break;		
		
		case BAUD_1M:
			selectedBaud = B1M;
	
		default:
		break;
	}
      
  	UF_MenuPrint_P( MIDI_BAUD[selectedBaud] );
}






/** Function to setup each individual analogue channel */
void ChannelSetup(void* data)
{
   uint8_t* input = data;
   uint8_t outputString[21];
   
   SelectedChannel = GetState(&primaryMenu) - ST_CHANNEL_1;

 	if( primaryMenu.firstEnter != 1 )
 	{
      UF_stateMachine( primaryMenu.currentState );
	   switch( *input )
	   {
         case KP_UP:
         case KP_DOWN:
            MenuSetInput( &analogueMenu, *input );
         break;
         
	      /* Up and down a Key */
	      case KP_1:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)+1);
	      break;
	      
	      case KP_7:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)-1);  
	      break;
	      
	      /* Up an octave */
	      case KP_STAR:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)+NOTE_COUNT);
	      break;
	      
	      case KP_D:
            SetChannelCommand(SelectedChannel, GetChannelCommand(SelectedChannel) + 1 );
         break;
	      
	      /*
	      case KP_7:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)-NOTE_COUNT);  
	      break;*/
	      
	      /* Up and down a Gain */
	      case KP_B:
				SetChannelGain(SelectedChannel, (GetChannelGain(SelectedChannel)+1));
	      break;
	      
	      case KP_C:
				SetChannelGain(SelectedChannel, (GetChannelGain(SelectedChannel)-1));  
	      break;
	      
	      /* Setting Modifiers */
	      case KP_A:
				ChannelToggle(SelectedChannel);
	      	break;
	               
	      
			case KB_BACK:
			case KP_BACK:
            UpdateActiveChannels();
            MenuSetInput( &analogueMenu, 0 );
				primaryMenu.firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
				UF_MenuReset();
				ActiveMenu = &analogueMenu;
				MenuSetInput( &analogueMenu, *input );
				executeState(&analogueMenu, analogueMenu.currentState);
				primaryMenu.firstEnter = 1;

				
				return;	
			break;
	         
	         
	      default:
	         break;
	   }
	}
	   
   primaryMenu.firstEnter = 0;
	UF_MenuReset();
		
	/* Indicate the channel selected */
	UF_MenuPrint_P(PSTR("Channel "));
	uint8toa(SelectedChannel + 1, outputString);	
	UF_MenuPrint(outputString);          
   
 	UF_MenuPrint_P(PSTR(": "));
	if( GetChannelStatus(SelectedChannel) == CHANNEL_ON )
	{
		UF_MenuPrint_P( PSTR("On") );
	}
	else
	{
		UF_MenuPrint_P( PSTR("Off") );		
	}   
   UF_MenuNewLine();
   

	UF_MenuPrint_P(PSTR("Note: "));
	
	if( GetChannelCommand(SelectedChannel) == MIDI_NOTE_ON )
	{
   	MIDI_NoteString(GetChannelKey(SelectedChannel), outputString);	
      UF_MenuPrint(outputString);
      uint8toa( MIDI_Octave(GetChannelKey(SelectedChannel)), outputString);
    	UF_MenuPrint(outputString); 	
   }
   else
   {
      
      uint8toa( GetChannelCommand(SelectedChannel), outputString);
      UF_MenuPrint(outputString);
      UF_MenuPrint_P( PSTR(" | ") );
   	uint8toa( GetChannelKey(SelectedChannel), outputString);
 		UF_MenuPrint(outputString);         
   }
   
	UF_MenuNewLine();	   
	
	/* Display the channel 'gain' */
	if( GetGainType(SelectedChannel) == LINEAR_GAIN )
	{
		UF_MenuPrint_P( PSTR("Linear ") );
	}
	else
	{
		UF_MenuPrint_P( PSTR("Non-Linear ") );		
	}
	UF_MenuPrint_P(PSTR("Gain: "));
	itoa( (int8_t)(GetChannelGain(SelectedChannel) - GAIN_OFFSET), outputString, 10);
	UF_MenuPrint(outputString);
	
   UF_MenuNewLine();   
   
//	Don't hide sub children.
   analogueMenu.updateOptions = SHOW_CHILDREN;
   SelectedSubMenu = &analogueMenu;
   MenuUpdate(&analogueMenu, !RESET_MENU);
}


void HandleSubMenu(void* data)
{
   uint8_t* input = data;
   
   /* This is effectively the KP Back routine */
   if( *input == KP_UPDATE )
   {
      /* Stop looping from occuring */
      MenuSetInput(ActiveMenu, KP_INVALID);
      ActiveMenu->firstEnter = 1;
      
      /* Show the parent options */
      MenuSetInput(ActiveMenu->parentMenu, KP_UPDATE);   
      MenuUpdate(ActiveMenu->parentMenu, RESET_MENU);

      
      ActiveMenu->updateOptions = HIDE_CHILDREN;
      ActiveMenu->firstEnter = 0;
      return;
   }
   
   if( *input != KP_BACK && !SelectedSubMenu->firstEnter )
   {
      stateMachine(SelectedSubMenu, SelectedSubMenu->currentState);
      switch( *input )
      {
         case KP_ENTER:
            MenuUpdate(SelectedSubMenu, !RESET_MENU);
            SelectedSubMenu->firstEnter = 1;
            return;
            //executeState(&analogueMenu, analogueMenu.currentState);
         break;
      }  
   }
   
   SelectedSubMenu->firstEnter = 0;
}


void SetThreshold(void* data)
{
	uint8_t* input;
   uint8_t outputString[21];
   
   static uint16_t lastPotValue = 0;
	static uint8_t firstEnter = 1;
	
   input = data;
   
   //SoftTimerStop(SoftTimer1[SC_MIDIOutput]);
	uint16_t PotValue = SensorPotValue() >> THRESHOLD_LEVELS;
   //SoftTimerStart(SoftTimer1[SC_MIDIOutput]);
   
	SoftTimerStart(SoftTimer2[SC_AutoMenuUpdate]);
	

	if( firstEnter == 1)
	{
      LCD_Load_ProgressBar();
	}
	
	
	switch( *input )
	{
         /* Up and down a Threshold Level */
         case KP_A:
				SetChannelThresh(SelectedChannel, (((GetChannelThresh(SelectedChannel) >> THRESHOLD_LEVELS)+1) << THRESHOLD_LEVELS));
         break;
         
         case KP_B:
				SetChannelThresh(SelectedChannel, (((GetChannelThresh(SelectedChannel) >> THRESHOLD_LEVELS)-1) << THRESHOLD_LEVELS));  
         break;
	
	      case KP_C:
				SoftTimer2[SC_AutoMenuUpdate].timerEnable ^= 1;  
	      break;  	  
			       
         case KP_BACK:
				SoftTimerStop(SoftTimer2[SC_AutoMenuUpdate]);
         	UF_MenuUpOneLevel(&analogueMenu); 
            firstEnter = 1;
         return;
	}
		
	firstEnter = 0;

	SetChannelThresh(SelectedChannel, GetChannelThresh(SelectedChannel) - lastPotValue + PotValue - MIN_THRESHOLD );
	lastPotValue = PotValue;

	uint8toa((GetChannelThresh(SelectedChannel) >> THRESHOLD_LEVELS), outputString);
	UF_MenuPrint_P(PSTR("Threshold Level: "));
	UF_MenuPrint(outputString);    
	
	UF_MenuNewLine(); 
	UF_MenuPrint_P(PSTR("Fine Tune:")); 
	
	UI_LCD_Pos(&PrimaryDisplay, 1, 10);         
   lcdProgressBar(PotValue,(1<<THRESHOLD_LEVELS), 10);	  
}




void SetRetrigger(void* data)
{
	uint8_t* input;
   uint8_t outputString[5];
   
   static uint8_t adjustStyle = DIGITAL_ADJUST;
	static uint8_t firstEnter = 1;
	
   input = data;

	if( firstEnter == 1)
	{
      LCD_Load_ProgressBar();
	}	


	switch( *input )
	{
         /* Increase and decrease retrigger level */
	      case KP_A:
				SetChannelReTrig(SelectedChannel,  (int16_t)(GetChannelReTrig(SelectedChannel) + 10 ));  
	      break;  	           
         
         case KP_B:
				SetChannelReTrig(SelectedChannel, (int16_t)(GetChannelReTrig(SelectedChannel) + 1));
         break;
         
         case KP_C:
				SetChannelReTrig(SelectedChannel,  (int16_t)(GetChannelReTrig(SelectedChannel) -1 ));        			
         break;
	
	      case KP_D:
				SetChannelReTrig(SelectedChannel,  (int16_t)(GetChannelReTrig(SelectedChannel) - 10 ));  
	      break;  	      
			
			
			/* Toggle retrigger adjustment style */
			case KP_HASH:
				adjustStyle ^= 1;
				if( adjustStyle == ANALOGUE_ADJUST )
				{
					SoftTimerStart(SoftTimer2[SC_AutoMenuUpdate]);
				}
				else
				{
					SoftTimerStop(SoftTimer2[SC_AutoMenuUpdate]);
				}
			break;
			
         case KP_BACK:
				SoftTimerStop(SoftTimer2[SC_AutoMenuUpdate]);
				adjustStyle = DIGITAL_ADJUST;
				//SoftTimerStop(SC_AutoMenuUpdate);
				if( SelectedChannel >= ANALOGUE_INPUTS )
				{ 
         	   UF_MenuUpOneLevel(&digitalMenu); 
            }
            else
            {
               UF_MenuUpOneLevel(&analogueMenu); 
            }
            firstEnter = 1;
         return;
	}
		
	firstEnter = 0;

	if( adjustStyle == ANALOGUE_ADJUST )
	{
		uint16_t PotValue = SensorPotValue();
		SetChannelReTrig(SelectedChannel, PotValue >> 4);	
	}

	/*SetChannelThresh(SelectedChannel, GetChannelThresh(SelectedChannel) - lastPotValue + PotValue - MIN_THRESHOLD );
	lastPotValue = PotValue;*/   

	UF_MenuPrint_P(PSTR("Use # to change adj."));
	UF_MenuNewLine();	
	
	UF_MenuPrint_P(PSTR("Retrigger Level: "));
	UF_MenuNewLine();	
	uint8toa(GetChannelReTrig(SelectedChannel), outputString);
	
	UF_MenuPrint(outputString);
	UF_MenuPrint_P(PSTR("0 ms "));	
   
	UI_LCD_Pos(&PrimaryDisplay, 2, 9);
	 
   lcdProgressBar(GetChannelReTrig(SelectedChannel), MAX_RETRIGGER, 11);   	
	    
	UF_MenuNewLine(); 

	UF_MenuPrint_P(PSTR("Default Retrig: 10ms"));	
	
   UpdateChannelRetriggers();
   	
}

// progress bar defines
#define PROGRESSPIXELS_PER_CHAR	6

void lcdProgressBar(uint16_t progress, uint16_t maxprogress, uint8_t length)
{
	uint8_t i;
	uint32_t pixelprogress;
	uint8_t c;

	// draw a progress bar displaying (progress / maxprogress)
	// starting from the current cursor position
	// with a total length of "length" characters
	// ***note, LCD chars 0-5 must be programmed as the bar characters
	// char 0 = empty ... char 5 = full

	// total pixel length of bargraph equals length*PROGRESSPIXELS_PER_CHAR;
	// pixel length of bar itself is
	pixelprogress = ((progress*(length*PROGRESSPIXELS_PER_CHAR))/maxprogress);
	
	// print exactly "length" characters
	for(i=0; i<length; i++)
	{
		// check if this is a full block, or partial or empty
		// (uint16_t) cast is needed to avoid sign comparison warning
		if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)+5) > pixelprogress )
		{
			// this is a partial or empty block
			if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)) > pixelprogress )
			{
				// this is an empty block
				// use space character?
				c = 1;
			}
			else
			{
				// this is a partial block
				c = (pixelprogress % PROGRESSPIXELS_PER_CHAR)+1;
			}
		}
		else
		{
			// this is a full block
			c = 6;
		}
		
		// write character to display
		UI_LCD_Char(&PrimaryDisplay, c);
	}
}

void LCD_Load_ProgressBar(void)
{
	// load the progress bar Chars
	UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[0], 1);
	UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[1], 2);
   UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[2], 3);
	UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[3], 4);
	UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[4], 5);
	UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[5], 6);
   UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[6], 7);  
}


void SetGainCurves(void* data)
{

   uint8_t* input = 0;
   uint8_t outputString[10];
   
   static int8_t presetSetting = CUSTOM;
	input = data;


   switch( *input )
   {
      /* Up and down a Key for Open Key*/
      case KP_1:
			SetChannelGain(SelectedChannel, GetChannelGain(SelectedChannel)+1);
      break;
      
      case KP_7:
			SetChannelGain(SelectedChannel, GetChannelGain(SelectedChannel)-1);  
      break;
      
      /* Up an octave 
      case KP_STAR:
			SetChannelGain(SelectedChannel, GetChannelGain(SelectedChannel)+NOTE_COUNT);
      break;*/
      
      /* Up and down a Key for Closed */
      case KP_3:
			SetSlope2Gain(SelectedChannel, GetSlope2Gain(SelectedChannel)+1);
      break;
      
      case KP_9:
			SetSlope2Gain(SelectedChannel, GetSlope2Gain(SelectedChannel)-1);  
      break;
      
      /* Up an octave
      case KP_HASH:
			SetSlope2Gain(SelectedChannel, GetSlope2Gain(SelectedChannel)+NOTE_COUNT);
      break;*/
      
      /* Setting Modifiers */
      case KP_A:
			GainTypeToggle(SelectedChannel);
      break;	     
		
		case KP_B:
			if( ++presetSetting >= NUMBER_OF_GAIN_PRESETS )
			{
				presetSetting = EXPONENTIAL_1;
			}
		break;
		
		case KP_C:
			if( --presetSetting < EXPONENTIAL_1 )
			{
				presetSetting = CUSTOM;
			}
		break; 
      
      /* Digital Trigger Select */
      case KP_2:
			SetCrossover(SelectedChannel, (int16_t)GetCrossover(SelectedChannel)+50);
      break;
      
      case KP_8:
			SetCrossover(SelectedChannel, (int16_t)GetCrossover(SelectedChannel)-50);  
      break;
      
      case KP_0:
			SetCrossover(SelectedChannel, (int16_t)GetCrossover(SelectedChannel)+100);			
		break;
      
      case KB_BACK:
      case KP_BACK:      
      	UF_MenuUpOneLevel(&analogueMenu);     
         return;
         
      default:
         break;
   }
	

	if( GetGainType(SelectedChannel) == LINEAR_GAIN )
	{
		presetSetting = CUSTOM;
	}   
   
	if( presetSetting != CUSTOM )
	{
		SetChannelGain(SelectedChannel, PresetGain1[presetSetting] );
		SetSlope2Gain(SelectedChannel, PresetGain2[presetSetting] );
		SetCrossover(SelectedChannel, PresetGainCrossover[presetSetting]);
	}
	
	
	UF_MenuReset();
		
	/* Indicate the channel selected */
 	UF_MenuPrint_P(PSTR("Gain Type:"));
	if( GetGainType(SelectedChannel) == LINEAR_GAIN )
	{
		UF_MenuPrint_P( PSTR("Linear") );
		UF_MenuNewLine(); 		
		/* Display the first slope channel 'gain' */
		itoa( (int8_t)(GetChannelGain(SelectedChannel) - GAIN_OFFSET), outputString, 10);
		UF_MenuPrint_P(PSTR("Gain1:"));
		UF_MenuPrint(outputString);
	   UF_MenuNewLine();
		UF_MenuNewLine();		
	}
	else
	{
		UF_MenuPrint_P( PSTR("Non-Linear") );
		UF_MenuNewLine(); 
		/* Display the first slope channel 'gain' */
		itoa( (int8_t)(GetChannelGain(SelectedChannel) - GAIN_OFFSET) , outputString, 10);
		UF_MenuPrint_P(PSTR("Gain1:"));
		UF_MenuPrint(outputString);
		
		/* Display the channel slope 2 'gain' */
		itoa( (int8_t)(GetSlope2Gain(SelectedChannel) - GAIN_OFFSET), outputString, 10);
		UF_MenuPrint_P(PSTR(" Gain2:"));
		UF_MenuPrint(outputString);
	   UF_MenuNewLine();   
		
		/* Display the gain crossover point */
		itoa(GetCrossover(SelectedChannel), outputString, 10);
		UF_MenuPrint_P(PSTR("Gain Crossover:"));
		UF_MenuPrint(outputString);
	   UF_MenuNewLine();			
		
		UF_MenuPrint_P(PSTR("Preset:"));
		UF_MenuPrint_P(PresetGainStrings[presetSetting]);			
	}   
	
	   

   UF_MenuNewLine();   		
			
	
}	
		
	


void SetDualInput(void* data)
{
   uint8_t* input = 0;
   uint8_t outputString[10];
	input = data;


   switch( *input )
   {
      /* Up and down a Key for Open Key*/
      case KP_1:
			SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)+1);
      break;
      
      case KP_7:
			SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)-1);  
      break;
      
      /* Up an octave */
      case KP_STAR:
			SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)+NOTE_COUNT);
      break;
      
      /* Up and down a Key for Closed */
      case KP_3:
			SetChannelKeyClosed(SelectedChannel, GetChannelKeyClosed(SelectedChannel)+1);
      break;
      
      case KP_9:
			SetChannelKeyClosed(SelectedChannel, GetChannelKeyClosed(SelectedChannel)-1);  
      break;
      
      /* Up an octave */
      case KP_HASH:
			SetChannelKeyClosed(SelectedChannel, GetChannelKeyClosed(SelectedChannel)+NOTE_COUNT);
      break;
      
      /* Setting Modifiers */
      case KP_A:
			DualModeToggle(SelectedChannel);
      break;	      
      
      /* Digital Trigger Select */
      case KP_B:
			SetDigitalTrigger(SelectedChannel, (int16_t)GetDigitalTrigger(SelectedChannel)+1);
      break;
      
      case KP_C:
			SetDigitalTrigger(SelectedChannel, (int16_t)GetDigitalTrigger(SelectedChannel)-1);  
      break;
      
      case KB_BACK:
      case KP_BACK:      
      	UF_MenuUpOneLevel(&analogueMenu);      
         return;
         
      default:
         break;
   }
	
	UF_MenuReset();
		
	/* Indicate the channel selected */
	UF_MenuPrint_P(PSTR("CH"));
	uint8toa(SelectedChannel + 1, outputString);	
	UF_MenuPrint(outputString);          
   
 	UF_MenuPrint_P(PSTR(" Dual Input: "));
	if( GetDualMode(SelectedChannel) == HAS_DUAL_INPUT )
	{
		UF_MenuPrint_P( PSTR("On") );
	}
	else
	{
		UF_MenuPrint_P( PSTR("Off") );		
	}   
   UF_MenuNewLine();
   

	UF_MenuPrint_P(PSTR("Open Note:"));
	MIDI_NoteString(GetChannelKey(SelectedChannel), outputString);	
   UF_MenuPrint(outputString);
	if( MIDI_Octave(GetChannelKey(SelectedChannel)) == 0 )
	{
      UF_MenuPrint_P( PSTR("0"));  
   }
   else
   {  
   	uint8toa( MIDI_Octave(GetChannelKey(SelectedChannel)), outputString);
 		UF_MenuPrint(outputString); 	
   }   

	UF_MenuNewLine();
	UF_MenuPrint_P(PSTR("Closed Note:"));
	MIDI_NoteString(GetChannelKeyClosed(SelectedChannel), outputString);	
   UF_MenuPrint(outputString);
	if( MIDI_Octave(GetChannelKeyClosed(SelectedChannel)) == 0 )
	{
      UF_MenuPrint_P( PSTR("0"));  
   }
   else
   {  
   	uint8toa( MIDI_Octave(GetChannelKeyClosed(SelectedChannel)), outputString);
 		UF_MenuPrint(outputString); 	
   }   

   
	UF_MenuNewLine();	   
	
	/* Display the channel velocity */
	uint8toa(GetDigitalTrigger(SelectedChannel)+1, outputString);
	UF_MenuPrint_P(PSTR("Activated by: D"));
	UF_MenuPrint(outputString);
   UF_MenuNewLine();   
}




/** Function to setup each individual digial channel */
void DigitalChannelSettings(void* data)
{
   uint8_t* input = 0;
   uint8_t outputString[21];
   
   SelectedChannel = GetState(&primaryMenu) - ST_CHANNEL_1;
	uint8_t SelectedDigitalChannel = SelectedChannel - ANALOGUE_INPUTS;

	input = data;

 	if( primaryMenu.firstEnter != 1 )
 	{
		UF_stateMachine( primaryMenu.currentState ); 
	   switch( *input )
	   {
         case KP_UP:
         case KP_DOWN:
            MenuSetInput( &digitalMenu, *input );
         break;
         
	      /* Up and down a Key */
	      case KP_1:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)+1);
	      break;
	      
	      case KP_7:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)-1);  
	      break;
	      
	      /* Up  an octave */
	      case KP_STAR:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)+NOTE_COUNT);
	      break;
	      
	      case KP_D:
            SetChannelCommand(SelectedChannel, GetChannelCommand(SelectedChannel) + 1 );
         break;
	      
	      
	      /* Up and down output levels */
	      case KP_3:
				SetDigitalVelocity(SelectedDigitalChannel, 
										 GetDigitalVelocity(SelectedDigitalChannel)+1);
	      break;
	      
	      case KP_9:
				SetDigitalVelocity(SelectedDigitalChannel, 
										 GetDigitalVelocity(SelectedDigitalChannel)-1);  
	      break;
	      
	      case KP_HASH:
				SetDigitalVelocity(SelectedDigitalChannel, 
										 GetDigitalVelocity(SelectedDigitalChannel)+10);  
	      break;	
	      /* Setting Modifiers */
	      case KP_A:
				ChannelToggle(SelectedChannel);
	      break;
	               
	      
			case KB_BACK:
			case KP_BACK:
				primaryMenu.firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
            
            UF_MenuReset();
				ActiveMenu = &digitalMenu;
				MenuSetInput( &digitalMenu, *input );
				executeState(&digitalMenu, digitalMenu.currentState);
				primaryMenu.firstEnter = 1;
            
				return;	
			break;
	         
	         
	      default:
	         break;
	   }
	}
   
   primaryMenu.firstEnter = 0;
	UF_MenuReset();
		
	/* Indicate the channel selected */
	
	if( SelectedDigitalChannel >= DIGITAL_INPUTS )
	{
   	UF_MenuPrint_P(PSTR("Metronome Ch "));
   	uint8toa(SelectedDigitalChannel - 7, outputString);
   }
   else
   {
   	UF_MenuPrint_P(PSTR("Digital Ch "));
   	uint8toa(SelectedDigitalChannel + 1, outputString);      
   }
	UF_MenuPrint(outputString);          
   
 	UF_MenuPrint_P(PSTR(": "));
	if( GetChannelStatus(SelectedChannel) == CHANNEL_ON )
	{
		UF_MenuPrint_P( PSTR("On") );
	}
	else
	{
		UF_MenuPrint_P( PSTR("Off") );		
	}   
   UF_MenuNewLine();
   

	UF_MenuPrint_P(PSTR("Note: "));
	
	if( GetChannelCommand(SelectedChannel) == MIDI_NOTE_ON )
	{
   	MIDI_NoteString(GetChannelKey(SelectedChannel), outputString);	
      UF_MenuPrint(outputString);
      uint8toa( MIDI_Octave(GetChannelKey(SelectedChannel)), outputString);
    	UF_MenuPrint(outputString); 	
   }  
   else
   {
      uint8toa( GetChannelCommand(SelectedChannel), outputString);
      UF_MenuPrint(outputString);
      UF_MenuPrint_P( PSTR(" | ") );
   	uint8toa( GetChannelKey(SelectedChannel), outputString);
 		UF_MenuPrint(outputString);         
   }
   
   
   
	UF_MenuNewLine();	   
	
	/* Display the channel velocity */
	uint8toa(GetDigitalVelocity(SelectedDigitalChannel), outputString);
	UF_MenuPrint_P(PSTR("Velocity: "));
	UF_MenuPrint(outputString);
   UF_MenuNewLine();  
   
   digitalMenu.updateOptions = SHOW_CHILDREN;
   SelectedSubMenu = &digitalMenu;
   MenuUpdate(&digitalMenu, !RESET_MENU);    
}





void SetSwitchType(void* data)
{
	uint8_t* input;
   uint8_t i;
   uint8_t SelectedDigitalChannel = SelectedChannel - ANALOGUE_INPUTS;
   
   input = data;

   

   if( SelectedDigitalChannel >= DIGITAL_INPUTS )
   {
      UF_MenuPrint_P(PSTR("Metronome inputs are"));
      UF_MenuNewLine();
      UF_MenuPrint_P(PSTR("fixed at:"));
      UF_MenuNewLine();
      UF_MenuPrint_P(PSTR("Active Low & "));
      UF_MenuNewLine();
      UF_MenuPrint_P(PSTR("Continuous"));

      for( i = 0; i < 15; i++ )
      {
         _delay_ms(200);   
      }

      UF_MenuUpOneLevel(&digitalMenu);
      return;
   }

	switch( *input )
	{
			/* Active High/Low toggle */
         case KP_A:
				ActiveStateToggle(SelectedDigitalChannel);
         break;
         
         case KP_B:
				TriggerModeToggle(SelectedDigitalChannel);         
			break;
	
         case KP_BACK:
         	UF_MenuUpOneLevel(&digitalMenu);
         return;
	}
	/* Switch Type */
	UF_MenuPrint_P(PSTR("Type: Active "));

	if( GetActiveState(SelectedDigitalChannel) == ACTIVE_HIGH )
	{
		UF_MenuPrint_P(PSTR("High"));
	}
	else
	{
		UF_MenuPrint_P(PSTR("Low"));
	}
   UF_MenuNewLine();


	/* Trigger Mode */
	UF_MenuPrint_P(PSTR("Mode: "));

	if( GetTriggerMode(SelectedDigitalChannel) == SINGLE_SHOT )
	{
		UF_MenuPrint_P(PSTR("Single Shot"));
	}
	else
	{
		UF_MenuPrint_P(PSTR("Continuous"));
	}
}





/** Function to setup each individual keypad button */
void KeypadButtonSettings(void* data)
{
   uint8_t* input = 0;
   uint8_t outputString[21];
   
   SelectedChannel = GetState(&primaryMenu) - ST_CHANNEL_1;
	uint8_t SelectedDigitalChannel = SelectedChannel - ANALOGUE_INPUTS;
	uint8_t SelectedKeypadChannel = SelectedDigitalChannel - (DIGITAL_INPUTS + METRONOME_INPUTS);
	input = data;

 	if( primaryMenu.firstEnter != 1 )
 	{
	   switch( *input )
	   {
	      /* Up and down a Key */
	      case KP_1:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)+1);
	      break;
	      
	      case KP_7:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)-1);  
	      break;
	      
	      /* Up  an octave */
	      case KP_STAR:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)+NOTE_COUNT);
	      break;
	      
	      case KP_D:
            SetChannelCommand(SelectedChannel, GetChannelCommand(SelectedChannel) + 1 );
         break;
	      
	      /* Up and down output levels */
	      case KP_3:
				SetDigitalVelocity(SelectedDigitalChannel, 
										 GetDigitalVelocity(SelectedDigitalChannel)+1);
	      break;
	      
	      case KP_9:
				SetDigitalVelocity(SelectedDigitalChannel, 
										 GetDigitalVelocity(SelectedDigitalChannel)-1);  
	      break;
	      
	      case KP_HASH:
				SetDigitalVelocity(SelectedDigitalChannel, 
										 GetDigitalVelocity(SelectedDigitalChannel)+10);  
	      break;	      
	      
         case KP_BACK:
				/* Go back up one menu */   
   			UF_MenuSetInput(KB_BACK);
  				UF_stateMachine(primaryMenu.currentState);
  				UF_MenuSetInput(0);
  			return;
				

	      default:
	         break;
	   }
	}
	
   primaryMenu.firstEnter = 0;
   /* Indicate the Keypad selected */
	
	UF_MenuPrint_P(PSTR("Keypad Button "));
	
	if( SelectedKeypadChannel > 0x0A )
	{
		outputString[0] = (SelectedKeypadChannel - 0x09) + 'A';		
		
		if( SelectedKeypadChannel == 14 )
		{
		outputString[0] = '*';	
		}
	
   	if( SelectedKeypadChannel == 15 )
		{
			outputString[0] = '#';	
		}	
		outputString[1] = '\0';	
	}
	else
	{
		uint8toa( SelectedKeypadChannel, outputString );
	}
	
	UF_MenuPrint(outputString);          
	UF_MenuNewLine();	 
	
	UF_MenuPrint_P(PSTR("Note: "));
	
	if( GetChannelCommand(SelectedChannel) == MIDI_NOTE_ON )
	{
   	MIDI_NoteString(GetChannelKey(SelectedChannel), outputString);	
      UF_MenuPrint(outputString);
   	if( MIDI_Octave(GetChannelKey(SelectedChannel)) == 0 )
   	{
         UF_MenuPrint_P( PSTR("0"));  
      }
      else
      {  
      	uint8toa( MIDI_Octave(GetChannelKey(SelectedChannel)), outputString);
    		UF_MenuPrint(outputString); 	
      } 
   }  
   else
   {
      uint8toa( GetChannelCommand(SelectedChannel), outputString);
      UF_MenuPrint(outputString);
      UF_MenuPrint_P( PSTR(" | ") );
   	uint8toa( GetChannelKey(SelectedChannel), outputString);
 		UF_MenuPrint(outputString);         
   }
   
	UF_MenuNewLine();	   
	
	/* Display the channel velocity */
	uint8toa(GetDigitalVelocity(SelectedDigitalChannel), outputString);
	UF_MenuPrint_P(PSTR("Velocity: "));
	UF_MenuPrint(outputString);   
}



void VUMeterSetup(void* data)
{

	uint8_t* input = data;
	static uint8_t firstEnter = 1;
   uint8_t VUMeterIndex = GetState(&primaryMenu) - ST_VUMETER;

	if( firstEnter == 1)
	{
		/* Load VU Meter */
		UI_LCD_LoadDefaultChars();
	}

	switch( *input )
	{         
			case KP_A:
				VUSetRows(GetVURows() + 1);	
			   SoftTimer2[SC_VUDecay].timeCompare = (MAX_ROWS + 1 - GetVURows()) << 2;
						
			break;
		
		
         case KP_BACK:
				SoftTimerStop(SoftTimer2[SC_VUDecay]);	
            
            if( VUMeterIndex == 0 )
            {
				  SoftTimerStop(SoftTimer2[SC_VUMeterUpdate]);
            }
            else
            {
              SoftTimerStop(SoftTimer2[SC_DigitalVUUpdate]);   
            }
							
				UF_MenuUpOneLevel(&primaryMenu);
            firstEnter = 1;
            /* Reset the VU Height */
            VUSetRows(1);
         return;
	}
	firstEnter = 0;
		
	/* Start the VU Meter */
	UF_MenuReset();
	UF_MenuPrint_P(PSTR("123456789ABCDEFG MAX"));
	
	
	VUSetPosition(1,18);
	UF_MenuPrint_P(PSTR("96"));
	VUSetPosition(2,18);
	UF_MenuPrint_P(PSTR("64"));	
	VUSetPosition(3,18);
	UF_MenuPrint_P(PSTR("32"));	
	
	if( GetVURows() == MAX_ROWS )
	{
		VUSetPosition(0,0);
	}
	else
	{
		VUSetPosition(1,0);
	}

   /* Start the correct timer */
   if( VUMeterIndex == 0 )
   {
	  SoftTimerStart(SoftTimer2[SC_VUMeterUpdate]);
   }
   else
   {
     SoftTimerStart(SoftTimer2[SC_DigitalVUUpdate]);   
   }

	SoftTimerStart(SoftTimer2[SC_VUDecay]);
}

/*
void DigitalVUMeterSetup(void* data)
{

	uint8_t* input = data;
	static uint8_t primaryMenu.firstEnter = 1;

	if( primaryMenu.firstEnter == 1)
	{
		UI_LCD_LoadDefaultChars();
	}

	switch( *input )
	{         
			case KP_A:
				VUSetRows(GetVURows() + 1);	
			   SoftTimer2[SC_VUDecay].timeCompare = (MAX_ROWS + 1 - GetVURows()) << 2;
						
			break;
		
		
         case KP_BACK:
				SoftTimerStop(SoftTimer2[SC_VUDecay]);				
				SoftTimerStop(SoftTimer2[SC_DigitalVUUpdate]);
			
				UF_MenuSetInput(KP_BACK);
            UF_stateMachine(primaryMenu.currentState);
            UF_MenuSetInput(0);
            primaryMenu.firstEnter = 1;
            UF_executeState( primaryMenu.currentState);

            VUSetRows(1);            
         return;
	}
	primaryMenu.firstEnter = 0;
		
	
	UF_MenuReset();
	UF_MenuPrint_P(PSTR("123456789ABCDEFG"));

	if( GetVURows() == MAX_ROWS )
	{
		VUSetPosition(0,0);
	}
	else
	{
		VUSetPosition(1,0);
	}
	SoftTimerStart(SoftTimer2[SC_DigitalVUUpdate]);
	SoftTimerStart(SoftTimer2[SC_VUDecay]);	
	
}*/


/* Amplifer Input Select */
void AmpInputSelect(void* data)
{
	uint8_t* input = (uint8_t*)data;
	
	if( primaryMenu.firstEnter != 1 )
   {     
		UF_stateMachine( primaryMenu.currentState );
      switch( *input )
      {
			case KB_BACK:
			case KP_BACK:
				primaryMenu.firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
			   UF_executeState( primaryMenu.currentState);
			break;

			default:
			break;
		}	
	}
	
	UF_MenuReset();
	primaryMenu.firstEnter = 0;
	UF_MenuPrint_P( PSTR("Trigger Using:") );
	
	UF_MenuNewLine();
	
	if( GetSensorInput() == SENSOR_OUTPUT )
	{
		UF_MenuPrint_P( PSTR("Fixed Gain (1x)") );		
	}
	else
	{
		UF_MenuPrint_P( PSTR("Variable Gain (POTx)") );	
	}
	
   UF_MenuNewLine(); 		
}




void SensorInputChange(void* data)
{
	/* Corresponding to either SENSOR_OUT or SENSOR_OUT2 */
	uint8_t SelectedState = ST_VARIABLE_GAIN - GetState(&primaryMenu);
	
   SoftTimerStop(SoftTimer1[SC_MIDIOutput]);
	SensorInputSelect( (SelectedState == SENSOR_OUTPUT2) ? SENSOR_OUTPUT2 : SENSOR_OUTPUT );
	SoftTimerStart(SoftTimer1[SC_MIDIOutput]);

   UF_MenuSetInput(KP_BACK);
   UF_stateMachine(primaryMenu.currentState);
   UF_MenuSetInput(0);	
	
}




/* Profiles */

void ShowProfile(void* data)
{
	uint8_t outputString[3];
	uint8_t* input = (uint8_t*)data;
	static uint8_t firstEnter = 1;
	
		
	if( firstEnter != 1 )
   {     
		UF_stateMachine( primaryMenu.currentState );
      switch( *input )
      {
			case KB_BACK:
			case KP_BACK:
				firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
				UF_MenuReset();
		   	UF_executeState( primaryMenu.currentState);
				firstEnter = 1;
				return;	
			break;

			default:
			break;
		}	
	}

	firstEnter = 0;

	UF_MenuPrint_P( PSTR("Current Profile: ") );
   UF_MenuNewLine();
   
   if( SelectedProfile == DEFAULT_PROFILE )
   {
		UF_MenuPrint_P( PSTR("Default Profile") );	
	}
	else
	{
		UF_MenuPrint_P( PSTR("Profile ") );
		uint8toa(SelectedProfile + 1, outputString );
		UF_MenuPrint(outputString);
	}
   UF_MenuNewLine();	
}

/* Save the CurrentProfile into the passed profileIndex */
void SaveProfile(void* data)
{
   uint8_t* input = 0;
   uint8_t  outputString[3];
   uint8_t   ProfileSlot = GetState(&primaryMenu) - ST_SAVE_PROFILE_1;
 	uint8_t i;
	  
   input = data;

	UF_MenuPrint_P( PSTR("Saving Data") );	
   UF_MenuNewLine();		
	UF_MenuPrint_P( PSTR("Please Wait...") );	

	for( i = 0; i < 4; i++ )
	{
		UF_MenuPrint_P( PSTR(".") );		
		_delay_ms(200);
	}
	
   UF_MenuNewLine();
	Profile_Write(&CurrentProfile, ProfileSlot);
	UF_MenuPrint_P( PSTR("Profile successfully") );
   UF_MenuNewLine();		
	UF_MenuPrint_P( PSTR("saved to: ") );				
	UF_MenuPrint_P( PSTR("Profile ") );	
	uint8toa( ProfileSlot + 1, outputString );
	UF_MenuPrint(outputString);

	for( i = 0; i < 4; i++ )
	{	
		_delay_ms(200);
	}	
	
	UF_MenuUpOneLevel(&primaryMenu);
	UF_MenuReset();
}

void LoadProfile(void* data)
{
   uint8_t* input = 0;
   uint8_t  outputString[3];
   uint8_t   ProfileSlot = GetState(&primaryMenu) - ST_LOAD_PROFILE_1;
	uint8_t i;
	
	   
   input = data;

	UF_MenuPrint_P( PSTR("Retrieving Data") );	
   UF_MenuNewLine();		
	UF_MenuPrint_P( PSTR("Please Wait") );	

	

	for( i = 0; i < 4; i++ )
	{
		UF_MenuPrint_P( PSTR(".") );		
		_delay_ms(100);
	}
	
   UF_MenuNewLine();		

   /* Loading the default profile resets the device */
	if( ProfileSlot == DEFAULT_PROFILE)
	{
      reset(0);
   }


	Profile_Read(ProfileSlot);
   /* Implement the changes */
	MIDI_SetRate(MIDI_GetRate());
	MIDI_SetBaud(MIDI_GetBaud());
	MIDI_SetChannelCode( MIDI_GetChannelCode() );

	/* Update the sensor select */
	SensorInputSelect(GetSensorInput());
	

   /* Update the Retrigger periods */
   UpdateChannelRetriggers();


	UF_MenuPrint_P( PSTR("Profile "));
	uint8toa( ProfileSlot + 1, outputString );
	UF_MenuPrint(outputString);				
   UF_MenuNewLine();		
	UF_MenuPrint_P( PSTR("successfully loaded!") );

	SelectedProfile = ProfileSlot;
	for( i = 0; i < 4; i++ )
	{	
		_delay_ms(200);
	}	
	
	UF_MenuUpOneLevel(&primaryMenu);
	UF_MenuReset();
}




void AdjustCrosstalk(void* data)
{
	uint8_t* input = (uint8_t*)data;
	int16_t crosstalk;
	uint8_t outputString[5];
	
	crosstalk = GetCrossTalkDelay();
	
	
   if( primaryMenu.firstEnter == 0 )
   {     
      switch( *input )
      {
         /* Increment crosstalk delay by 10 */
         case 'q':
         case KP_A:
				crosstalk += 10;
            if( crosstalk > MAX_CROSSTALK)
            {
               crosstalk = MIN_CROSSTALK;   
            }   
         break;
            
         /* Increment crosstalk delay by 1*/  
         case 'a':                   
         case KP_B:
				crosstalk += 1;
            if( crosstalk > MAX_CROSSTALK)
            {
               crosstalk = MIN_CROSSTALK;   
            }   
         break;

         /* decrement crosstalk delay by 1*/  
        case KP_C:
				crosstalk -= 1;
            if(crosstalk < MIN_CROSSTALK)
            {
               crosstalk = MAX_CROSSTALK;   
            }
         break;     
         
         /* Decrement crosstalk delay by 10 */  
        case KP_D:
				crosstalk -= 10;
            if(crosstalk < MIN_CROSSTALK)
            {
               crosstalk = MAX_CROSSTALK;   
            }
         break;       
           
         /* Time Component increment function */
         case KB_BACK:
         case KP_BACK:
				/* Go back up one menu */   
   			UF_MenuSetInput(KB_BACK);
  				UF_stateMachine(primaryMenu.currentState);
  				UF_MenuSetInput(0);
  				return;
  				
         break;
                        
         default:
         break;     
      }
   }
   
   primaryMenu.firstEnter = 0;
   
   SetCrossTalkDelay(crosstalk);
   
   UF_MenuPrint_P( PSTR("Crosstalk Delay:"));
   UF_MenuNewLine();
	utoa(crosstalk, outputString, 10);
	UF_MenuPrint(outputString);		
   UF_MenuPrint_P( PSTR(" us"));
	
}






void ChangeChannelCode(void* data)
{
	uint8_t* input = (uint8_t*)data;
	static int8_t	code;
	uint8_t outputString[4];
	
	if( primaryMenu.firstEnter == 1)
	{
		code = MIDI_GetChannelCode();
	}
	
	
   if( primaryMenu.firstEnter == 0 )
   {     
      switch( *input )
      {
         /* Increment channel # */
         case 'q':
         case KP_A:
            if( ++code > MAX_MIDI_CHANNEL)
            {
               code = 0;   
            }   
         break;
            
         /* Deccrement channel #*/  
         case 'a':                   
         case KP_B:
            if(--code < 0)
            {
               code = MAX_MIDI_CHANNEL;   
            }
         break;
           
         /* Time Component increment function */
         case KB_BACK:
         case KP_BACK:
				/* Go back up one menu */   
   			UF_MenuSetInput(KB_BACK);
  				UF_stateMachine(primaryMenu.currentState);
  				UF_MenuSetInput(0);
  				return;
  				
         break;
                        
         default:
         break;     
      }
   }
   
   primaryMenu.firstEnter = 0;
   
   MIDI_SetChannelCode(code);
   
   UF_MenuPrint_P( PSTR("eDrum MIDI Code: "));
	uint8toa( code + 1, outputString);
	UF_MenuPrint(outputString);		
}

