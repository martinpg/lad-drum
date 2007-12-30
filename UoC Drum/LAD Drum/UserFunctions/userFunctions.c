
#include <io.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "Menu/Menu.h"
#include "UI_KP/UI_KP.h"
#include "UI_LCD/UI_LCD.h"
#include "UserFunctions/userFunctions.h"
#include "mmculib/uint16toa.h"
#include "mmculib/uint8toa.h"
#include "MIDI/midi.h"
#include "Sample/sample.h"
#include "Sensor/sensor.h"
#include "ADC/adc12.h"
#include "Delay/delay.h"
#include "Profiles/profiles.h"


static uint8_t SelectedProfile = DEFAULT_PROFILE;

void SetMIDIRate(void* data)
{
	uint8_t* input = (uint8_t*)data;
	//static uint8_t firstEnter = 1;
	
		
	if( firstEnter != 1 )
   {     
		stateMachine( currentState );
      switch( *input )
      {
			case KB_BACK:
			case KP_BACK:
				firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
			   executeState(currentState);
				return;	
			break;

			default:
			break;
		}	
	}
	
	MenuReset();
	firstEnter = 0;
   PrintMIDIRate();
   MenuNewLine(); 
}


void PrintMIDIRate(void)
{
	uint8_t outputString[8];
	uint8_t selectedBaud = 0;
	
	MenuPrint_P( PSTR("MIDI Output Rate: ") );
   MenuNewLine(); 	
	uint16toa( (MIDI_GetRate()), outputString, 0);
	MenuPrint(outputString);
   MenuPrint_P( PSTR(" ms @ "));
   
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
	
		default:
		break;
	}
      
  	MenuPrint_P( MIDI_BAUD[selectedBaud] );	
}

void EditMIDIRate(void* data)
{
	uint8_t* input = (uint8_t*)data;
	static uint16_t	Delay;
	uint8_t selectedBaud = 0;
	uint8_t outputString[4];
	
	if( firstEnter == 1)
	{
		Delay = MIDI_GetRate();
	}
	
   if( firstEnter == 0 )
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
   			MenuSetInput(KB_BACK);
  				stateMachine(currentState);
  				MenuSetInput(0);
  				firstEnter = 1;
  				executeState(currentState);
  				return;
  				
         break;
                        
         default:
         break;     
      }
   }
   
   firstEnter = 0;
   
   MenuPrint_P( PSTR("Millisecond Delay:"));
	MenuNewLine();
	
	uint16toa( Delay, outputString, 0);
	MenuPrint(outputString);
	MenuPrint_P( PSTR(" milliseconds") );      
	MIDI_SetRate( Delay );

	MenuNewLine();
   MenuPrint_P( PSTR("Baud Rate: "));
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
	
		default:
		break;
	}
      
  	MenuPrint_P( MIDI_BAUD[selectedBaud] );
}














/** Function to setup each individual analogue channel */
void ChannelSetup(void* data)
{
   uint8_t* input = 0;
   uint8_t outputString[21];
   uint8_t SelectedChannel = GetState() - ST_CHANNEL_1;

	input = data;

 	if( firstEnter != 1 )
 	{
		stateMachine( currentState ); 
	   switch( *input )
	   {
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
				firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
				MenuReset();						
		   	executeState(currentState);
				firstEnter = 1;

				
				return;	
			break;
	         
	         
	      default:
	         break;
	   }
	}
   
   firstEnter = 0;
	MenuReset();
		
	/* Indicate the channel selected */
	MenuPrint_P(PSTR("Channel "));
	uint8toa(SelectedChannel + 1, outputString);	
	MenuPrint(outputString);          
   
 	MenuPrint_P(PSTR(": "));
	if( GetChannelStatus(SelectedChannel) == CHANNEL_ON )
	{
		MenuPrint_P( PSTR("On") );
	}
	else
	{
		MenuPrint_P( PSTR("Off") );		
	}   
   MenuNewLine();
   

	MenuPrint_P(PSTR("Note: "));
	MIDI_NoteString(GetChannelKey(SelectedChannel), outputString);	
   MenuPrint(outputString);
	if( MIDI_Octave(GetChannelKey(SelectedChannel)) == 0 )
	{
      MenuPrint_P( PSTR("0"));  
   }
   else
   {  
   	uint8toa( MIDI_Octave(GetChannelKey(SelectedChannel)), outputString);
 		MenuPrint(outputString); 	
   }   
   
	MenuNewLine();	   
	
	/* Display the channel 'gain' */
	itoa(GetChannelGain(SelectedChannel), outputString, 10);
	MenuPrint_P(PSTR("Gain: "));
	MenuPrint(outputString);
	
   MenuNewLine();   
}


void SetThreshold(void* data)
{
	uint8_t* input;
   uint8_t outputString[21];
   uint8_t SelectedChannel = GetState() - ST_THRESHOLD_1;
   
   static uint16_t lastPotValue = 0;
	static uint8_t firstEnter = 1;
	
   input = data;
   
   SoftTimerStop(SoftTimer1[SC_MIDIOutput]);
	uint16_t PotValue = SensorPotValue() >> THRESHOLD_LEVELS;
   SoftTimerStart(SoftTimer1[SC_MIDIOutput]);
   
	SoftTimerStart(SoftTimer2[SC_AutoMenuUpdate]);
	

	if( firstEnter == 1)
	{
				// load the first 8 custom characters
		UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[0], 0);
		UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[1], 1);
	   UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[2], 2);
		UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[3], 3);
		UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[4], 4);
		UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[5], 5);
	   UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[6], 6);	
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
         	MenuSetInput(KP_BACK);
            stateMachine(currentState);
            MenuSetInput(0);
            firstEnter = 1;
            
				/* Load VU Meter */
				// load the first 8 custom characters
				UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[6], 0);				
				UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[7], 1);
				UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[8], 2);
			   UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[9], 3);
				UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[10], 4);
				UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[11], 5);
				UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[12], 6);
			   UI_LCD_LoadCustomChar((uint8_t*)LcdCustomChar[13], 7);
            
            executeState(currentState);

         return;
	}
		
	firstEnter = 0;

	SetChannelThresh(SelectedChannel, GetChannelThresh(SelectedChannel) - lastPotValue + PotValue - MIN_THRESHOLD );
	lastPotValue = PotValue;

	uint8toa((GetChannelThresh(SelectedChannel) >> THRESHOLD_LEVELS), outputString);
	MenuPrint_P(PSTR("Threshold Level: "));
	MenuPrint(outputString);    
	
	MenuNewLine(); 
	MenuPrint_P(PSTR("Fine Tune:")); 
	
	UI_LCD_Pos(1, 10);         
   lcdProgressBar(PotValue,(1<<THRESHOLD_LEVELS), 10);	  
}


void ThresholdBar(void)
{
   SoftTimerStop(SoftTimer1[SC_MIDIOutput]);
   ADC12_SetupAddress(0, INCH_A3);
   uint16_t PotValue = (ADC12_Sample() >> THRESHOLD_LEVELS);
   ADC12_SetupAddress(0, INCH_A0); 
   SoftTimerStart(SoftTimer1[SC_MIDIOutput]);	
	
	UI_LCD_Pos(1, 10);         
   lcdProgressBar(PotValue,(1<<THRESHOLD_LEVELS), 10);	
}


void SetRetrigger(void* data)
{
	uint8_t* input;
   uint8_t outputString[5];
   uint8_t SelectedChannel = GetState() - ST_RETRIGGER_1;
   
   static uint16_t lastPotValue = 0;
	static uint8_t firstEnter = 1;
	
   input = data;


	switch( *input )
	{
         /* Up and down a Trigger Level */
         case KP_A:
				SetChannelReTrig(SelectedChannel, (int16_t)(GetChannelReTrig(SelectedChannel) + 1));
         break;
         
         case KP_B:
				SetChannelReTrig(SelectedChannel,  (int16_t)(GetChannelReTrig(SelectedChannel) -1 ));        
			break;
	
	      case KP_C:
				//SC_AutoMenuUpdate.timerEnable ^= 1;  
	      break;  	  
			       
         case KP_BACK:
				//SoftTimerStop(SC_AutoMenuUpdate);
         	MenuSetInput(KP_BACK);
            stateMachine(currentState);
            MenuSetInput(0);
            firstEnter = 1;
            executeState(currentState);
         return;
	}
		
	firstEnter = 0;

	/*SetChannelThresh(SelectedChannel, GetChannelThresh(SelectedChannel) - lastPotValue + PotValue - MIN_THRESHOLD );
	lastPotValue = PotValue;*/   
	uint8toa(GetChannelReTrig(SelectedChannel), outputString);
	MenuPrint_P(PSTR("Retrigger Level: "));
	MenuNewLine();
	MenuPrint(outputString);
	MenuPrint_P(PSTR(" ms"));	    
	MenuNewLine(); 
	
   UpdateChannelRetriggers();
   	
/*	MenuPrint_P(PSTR("Fine Tune:"));   */
/*	UI_LCD_Pos(1, 10);         
   lcdProgressBar(PotValue,(1<<THRESHOLD_LEVELS), 10);*/
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
				c = 0;
			}
			else
			{
				// this is a partial block
				c = pixelprogress % PROGRESSPIXELS_PER_CHAR;
			}
		}
		else
		{
			// this is a full block
			c = 5;
		}
		
		// write character to display
		UI_LCD_Char(c);
	}
}




void SetGainCurves(void* data)
{
	
	
		
	
	
	
	
	
	
}



void SetDualInput(void* data)
{
   uint8_t* input = 0;
   uint8_t outputString[10];
   uint8_t SelectedChannel = GetState() - ST_DUALINPUT_1;
   static uint8_t firstEnter = 1;
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
      	MenuSetInput(KP_BACK);
         stateMachine(currentState);
         MenuSetInput(0);
         firstEnter = 1;
         executeState(currentState);         
         return;
         
      default:
         break;
   }
	
   
   firstEnter = 0;
	MenuReset();
		
	/* Indicate the channel selected */
	MenuPrint_P(PSTR("CH"));
	uint8toa(SelectedChannel + 1, outputString);	
	MenuPrint(outputString);          
   
 	MenuPrint_P(PSTR(" Dual Input: "));
	if( GetDualMode(SelectedChannel) == HAS_DUAL_INPUT )
	{
		MenuPrint_P( PSTR("On") );
	}
	else
	{
		MenuPrint_P( PSTR("Off") );		
	}   
   MenuNewLine();
   

	MenuPrint_P(PSTR("Open Note:"));
	MIDI_NoteString(GetChannelKey(SelectedChannel), outputString);	
   MenuPrint(outputString);
	if( MIDI_Octave(GetChannelKey(SelectedChannel)) == 0 )
	{
      MenuPrint_P( PSTR("0"));  
   }
   else
   {  
   	uint8toa( MIDI_Octave(GetChannelKey(SelectedChannel)), outputString);
 		MenuPrint(outputString); 	
   }   

	MenuNewLine();
	MenuPrint_P(PSTR("Closed Note:"));
	MIDI_NoteString(GetChannelKeyClosed(SelectedChannel), outputString);	
   MenuPrint(outputString);
	if( MIDI_Octave(GetChannelKeyClosed(SelectedChannel)) == 0 )
	{
      MenuPrint_P( PSTR("0"));  
   }
   else
   {  
   	uint8toa( MIDI_Octave(GetChannelKeyClosed(SelectedChannel)), outputString);
 		MenuPrint(outputString); 	
   }   

   
	MenuNewLine();	   
	
	/* Display the channel velocity */
	uint8toa(GetDigitalTrigger(SelectedChannel), outputString);
	MenuPrint_P(PSTR("Activated by: D"));
	MenuPrint(outputString);
   MenuNewLine();   
}




/** Function to setup each individual digial channel */
void DigitalChannelSettings(void* data)
{
   uint8_t* input = 0;
   uint8_t outputString[21];
   uint8_t SelectedChannel = GetState() - ST_DIGITAL_1;

	input = data;

 	if( firstEnter != 1 )
 	{
		stateMachine( currentState ); 
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
	      
	      /*
	      case KP_7:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)-NOTE_COUNT);  
	      break;*/
	      
	      /* Up and down output levels */
	      case KP_B:
				SetDigitalVelocity(SelectedChannel, (int16_t)GetDigitalVelocity(SelectedChannel)+10);
	      break;
	      
	      case KP_C:
				SetDigitalVelocity(SelectedChannel, (int16_t)GetDigitalVelocity(SelectedChannel)-10);  
	      break;
	      
	      /* Setting Modifiers */
	      case KP_A:
				ChannelToggle(SelectedChannel);
	      	break;
	               
	      
			case KB_BACK:
			case KP_BACK:
				firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
				MenuReset();
		   	executeState(currentState);
				firstEnter = 1;
				return;	
			break;
	         
	         
	      default:
	         break;
	   }
	}
   
   firstEnter = 0;
	MenuReset();
		
	/* Indicate the channel selected */
	MenuPrint_P(PSTR("Digital Ch "));
	uint8toa(SelectedChannel + 1, outputString);	
	MenuPrint(outputString);          
   
 	MenuPrint_P(PSTR(": "));
	if( GetChannelStatus(SelectedChannel) == CHANNEL_ON )
	{
		MenuPrint_P( PSTR("On") );
	}
	else
	{
		MenuPrint_P( PSTR("Off") );		
	}   
   MenuNewLine();
   

	MenuPrint_P(PSTR("Note: "));
	MIDI_NoteString(GetChannelKey(SelectedChannel), outputString);	
   MenuPrint(outputString);
	if( MIDI_Octave(GetChannelKey(SelectedChannel)) == 0 )
	{
      MenuPrint_P( PSTR("0"));  
   }
   else
   {  
   	uint8toa( MIDI_Octave(GetChannelKey(SelectedChannel)), outputString);
 		MenuPrint(outputString); 	
   }   
   
	MenuNewLine();	   
	
	/* Display the channel velocity */
	uint8toa(GetDigitalVelocity(SelectedChannel), outputString);
	MenuPrint_P(PSTR("Velocity: "));
	MenuPrint(outputString);
   MenuNewLine();   
}



void SetSwitchType(void* data)
{
	uint8_t* input;
   uint8_t outputString[5];
   uint8_t SelectedChannel = GetState() - ST_TRIGGER_TYPE_D1;
   
	static uint8_t firstEnter = 1;
	
   input = data;

	switch( *input )
	{
			/* Active High/Low toggle */
         case KP_A:
				ActiveStateToggle(SelectedChannel);
         break;
         
         case KP_B:
				TriggerModeToggle(SelectedChannel);         
			break;
	
         case KP_BACK:
         	MenuSetInput(KP_BACK);
            stateMachine(currentState);
            MenuSetInput(0);
            firstEnter = 1;
            executeState(currentState);
         return;
	}
		
	firstEnter = 0;

	/* Switch Type */
	MenuPrint_P(PSTR("Type: Active "));

	if( GetActiveState(SelectedChannel) == ACTIVE_HIGH )
	{
		MenuPrint_P(PSTR("High"));
	}
	else
	{
		MenuPrint_P(PSTR("Low"));
	}
   MenuNewLine();


	/* Trigger Mode */
	MenuPrint_P(PSTR("Mode: "));

	if( GetTriggerMode(SelectedChannel) == SINGLE_SHOT )
	{
		MenuPrint_P(PSTR("Single Shot"));
	}
	else
	{
		MenuPrint_P(PSTR("Continuous"));
	}
}








/* Amplifer Input Select */
void AmpInputSelect(void* data)
{
	uint8_t* input = (uint8_t*)data;
	
	if( firstEnter != 1 )
   {     
		stateMachine( currentState );
      switch( *input )
      {
			case KB_BACK:
			case KP_BACK:
				firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
			   executeState(currentState);
			break;

			default:
			break;
		}	
	}
	
	MenuReset();
	firstEnter = 0;
	MenuPrint_P( PSTR("Trigger Using:") );
	
	MenuNewLine();
	
	if( GetSensorInput() == SENSOR_OUTPUT )
	{
		MenuPrint_P( PSTR("Fixed Gain (1x)") );		
	}
	else
	{
		MenuPrint_P( PSTR("Variable Gain (POTx)") );	
	}
	
   MenuNewLine(); 		
}




void SensorInputChange(void* data)
{
	/* Corresponding to either SENSOR_OUT or SENSOR_OUT2 */
	uint8_t SelectedState = ST_VARIABLE_GAIN - GetState();
	
   SoftTimerStop(SoftTimer1[SC_MIDIOutput]);
	SensorInputSelect( (SelectedState == SENSOR_OUTPUT2) ? SENSOR_OUTPUT2 : SENSOR_OUTPUT );
	SoftTimerStart(SoftTimer1[SC_MIDIOutput]);

   MenuSetInput(KP_BACK);
   stateMachine(currentState);
   MenuSetInput(0);	
	
}




/* Profiles */

void ShowProfile(void* data)
{
	uint8_t outputString[3];
	uint8_t* input = (uint8_t*)data;
	static uint8_t firstEnter = 1;
	
		
	if( firstEnter != 1 )
   {     
		stateMachine( currentState );
      switch( *input )
      {
			case KB_BACK:
			case KP_BACK:
				firstEnter = 1;
				return;
			break;
				
			case KB_ENTER:
			case KP_ENTER:
				MenuReset();
		   	executeState(currentState);
				firstEnter = 1;
				return;	
			break;

			default:
			break;
		}	
	}

	firstEnter = 0;

	MenuPrint_P( PSTR("Current Profile: ") );
   MenuNewLine();
   
   if( SelectedProfile == DEFAULT_PROFILE )
   {
		MenuPrint_P( PSTR("Default Profile") );	
	}
	else
	{
		MenuPrint_P( PSTR("Profile ") );
		uint8toa(SelectedProfile + 1, outputString );
		MenuPrint(outputString);
	}
   MenuNewLine();	
}

/* Save the CurrentProfile into the passed profileIndex */
void SaveProfile(void* data)
{
   uint8_t* input = 0;
   uint8_t  outputString[3];
   uint8_t   ProfileSlot = GetState() - ST_SAVE_PROFILE_1;
 	uint8_t i;
	  
   input = data;

	MenuPrint_P( PSTR("Saving Data") );	
   MenuNewLine();		
	MenuPrint_P( PSTR("Please Wait...") );	

	for( i = 0; i < 4; i++ )
	{
		MenuPrint_P( PSTR(".") );		
		_delay_ms(200);
	}
	
   MenuNewLine();
	Profile_Write(&CurrentProfile, ProfileSlot);
	MenuPrint_P( PSTR("Profile successfully") );
   MenuNewLine();		
	MenuPrint_P( PSTR("saved to: ") );				
	MenuPrint_P( PSTR("Profile ") );	
	uint8toa( ProfileSlot + 1, outputString );
	MenuPrint(outputString);

	for( i = 0; i < 4; i++ )
	{	
		_delay_ms(200);
	}	
	
	MenuReset();
   MenuSetInput(KP_BACK);
   stateMachine(currentState);
   MenuSetInput(0);
}

void LoadProfile(void* data)
{
   uint8_t* input = 0;
   uint8_t  outputString[3];
   uint8_t   ProfileSlot = GetState() - ST_LOAD_PROFILE_1;
	uint8_t i;
	
	   
   input = data;

	MenuPrint_P( PSTR("Retrieving Data") );	
   MenuNewLine();		
	MenuPrint_P( PSTR("Please Wait") );	

	

	for( i = 0; i < 4; i++ )
	{
		MenuPrint_P( PSTR(".") );		
		_delay_ms(100);
	}
	
   MenuNewLine();		

	Profile_Read(ProfileSlot);


   /* Implement the changes */
	MIDI_SetRate(MIDI_GetRate());
	MIDI_SetBaud(MIDI_GetBaud());
	MIDI_SetChannelCode( MIDI_GetChannelCode() );

	/* Update the sensor select */
	SensorInputSelect(GetSensorInput());

   /* Update the Retrigger periods */
   UpdateChannelRetriggers();

	if( ProfileSlot == DEFAULT_PROFILE)
	{
		MenuPrint_P( PSTR("Default Profile"));			
	}
	else
	{
		MenuPrint_P( PSTR("Profile "));
		uint8toa( ProfileSlot + 1, outputString );
		MenuPrint(outputString);			
	}
	
   MenuNewLine();		
	MenuPrint_P( PSTR("successfully loaded!") );

	SelectedProfile = ProfileSlot;
	for( i = 0; i < 4; i++ )
	{	
		_delay_ms(200);
	}	
	
	MenuReset();	
   MenuSetInput(KP_BACK);
   stateMachine(currentState);
   MenuSetInput(0);
}











void ChangeChannelCode(void* data)
{
	uint8_t* input = (uint8_t*)data;
	static int8_t	code;
	uint8_t outputString[4];
	
	if( firstEnter == 1)
	{
		code = MIDI_GetChannelCode();
	}
	
	
   if( firstEnter == 0 )
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
               code = MAX_MIDI_CHANNEL - 1;   
            }
         break;
           
         /* Time Component increment function */
         case KB_BACK:
         case KP_BACK:
				/* Go back up one menu */   
   			MenuSetInput(KB_BACK);
  				stateMachine(currentState);
  				MenuSetInput(0);
  				return;
  				
         break;
                        
         default:
         break;     
      }
   }
   
   firstEnter = 0;
   
   MIDI_SetChannelCode(code);
   
   MenuPrint_P( PSTR("eDrum MIDI Code: "));
	uint8toa( code, outputString);
	MenuPrint(outputString);		
}

