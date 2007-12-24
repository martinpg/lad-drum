
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
#include "ADC/adc12.h"
#include "Delay/delay.h"
#include "Profiles/profiles.h"

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
	uint8_t outputString[6];
	
	MenuPrint_P( PSTR("MIDI Output Rate: ") );
   MenuNewLine(); 	
	uint16toa( (MIDI_GetRate() / 10), outputString, 0);
	MenuPrint(outputString);
   MenuPrint_P( PSTR(" ms @ "));
  	MenuPrint_P( (MIDI_GetBaud() == BAUD_31250) ? "31250" : "38400" );
   MenuPrint_P( PSTR(" BPS"));  	
}

void EditMIDIRate(void* data)
{
	uint8_t* input = (uint8_t*)data;
	static uint16_t	Delay;
	uint8_t outputString[4];
	
	if( firstEnter == 1)
	{
		Delay = MIDI_GetRate()/ 10;
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
            if( MIDI_GetBaud() == BAUD_31250 )
            {
               MIDI_SetBaud(BAUD_38400);   
            }
            else
            {
               MIDI_SetBaud(BAUD_31250);
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
	MIDI_SetRate( Delay * 10 );

	MenuNewLine();
   MenuPrint_P( PSTR("Baud Rate: "));
  	MenuPrint_P( (MIDI_GetBaud() == BAUD_31250) ? "31250" : "38400" );
   MenuPrint_P( PSTR(" BPS"));
}














/** Function to setup each individual channel */
void ChannelSettings(void* data)
{
   uint8_t* input = 0;
   uint8_t outputString[21];
   int8_t SelectedChannel = GetState() - ST_CHANNEL_1;

	input = data;

 	if( firstEnter != 1 )
 	{
		stateMachine( currentState ); 
	   switch( *input )
	   {
	      /* Up and down a Key */
	      case KP_3:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)+1);
	      break;
	      
	      case KP_9:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)-1);  
	      break;
	      
	      /* Up and down an octave */
	      case KP_1:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)+NOTE_COUNT);
	      break;
	      
	      case KP_7:
				SetChannelKey(SelectedChannel, GetChannelKey(SelectedChannel)-NOTE_COUNT);  
	      break;
	      
	      /* Up and down a Gain */
	      case KP_B:
				SetChannelGain(SelectedChannel, GetChannelGain(SelectedChannel)-1);
	      break;
	      
	      case KP_C:
				SetChannelGain(SelectedChannel, GetChannelGain(SelectedChannel)+1);  
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
				SoftTimerStart(SC_AutoMenuUpdate);
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
   
   
   /* Indicate the Channel Type */
	MenuPrint_P(PSTR("Octave: "));
	if( MIDI_Octave(GetChannelKey(SelectedChannel)) == 0 )
	{
      MenuPrint_P( PSTR("0"));  
   }
   else
   {  
   	uint8toa( MIDI_Octave(GetChannelKey(SelectedChannel)), outputString);
 		MenuPrint(outputString); 	
   }

	MenuPrint_P(PSTR("  Note: "));
	MIDI_NoteString(GetChannelKey(SelectedChannel), outputString);	
   MenuPrint(outputString);
	MenuNewLine();	   
	
	/* Display the channel 'gain' */
	uint8toa(MAX_GAIN - GetChannelGain(SelectedChannel), outputString);
	MenuPrint_P(PSTR("Gain: "));
	MenuPrint(outputString);
	
   MenuNewLine();   


}


void SetThreshold(void* data)
{
	uint8_t* input;
   uint8_t outputString[21];
   int8_t SelectedChannel = GetState() - ST_THRESHOLD_1;
   
   static uint16_t lastPotValue = 0;
	static uint8_t firstEnter = 1;
	
   input = data;

   SoftTimerStop(SC_MIDIOutput);
   ADC12_SetupAddress(0, INCH_A3);
   uint16_t PotValue = (ADC12_Sample() >> THRESHOLD_LEVELS);
   ADC12_SetupAddress(0, INCH_A0); 
   SoftTimerStart(SC_MIDIOutput);


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
				SC_AutoMenuUpdate.timerEnable ^= 1;  
	      break;  	  
			       
         case KP_BACK:
				SoftTimerStop(SC_AutoMenuUpdate);
         	MenuSetInput(KP_BACK);
            stateMachine(currentState);
            MenuSetInput(0);
            firstEnter = 1;
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


void SetRetrigger(void* data)
{
	uint8_t* input;
   uint8_t outputString[21];
   int8_t SelectedChannel = GetState() - ST_RETRIGGER_1;
   
   static uint16_t lastPotValue = 0;
	static uint8_t firstEnter = 1;
	
   input = data;

   /*SoftTimerStop(SC_MIDIOutput);
   ADC12_SetupAddress(0, INCH_A3);
   uint16_t PotValue = (ADC12_Sample() >> THRESHOLD_LEVELS);
   ADC12_SetupAddress(0, INCH_A0); 
   SoftTimerStart(SC_MIDIOutput);*/


	switch( *input )
	{
         /* Up and down a Trigger Level */
         case KP_A:
				SetChannelReTrig(SelectedChannel, GetChannelReTrig(SelectedChannel) + 1);
         break;
         
         case KP_B:
				SetChannelReTrig(SelectedChannel,  GetChannelReTrig(SelectedChannel) -1 );         break;
	
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
	MenuPrint(outputString);    
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
   int8_t   ProfileSlot = GetState() - ST_SAVE_PROFILE_1;
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
   int8_t   ProfileSlot = GetState() - ST_LOAD_PROFILE_1;
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

	memcpy((Profile_t*)&CurrentProfile, Profile_Read(ProfileSlot), sizeof(Profile_t));

   /* Implement the changes */
	MIDI_SetRate(MIDI_GetRate());
	MIDI_SetBaud(MIDI_GetBaud());
	MIDI_SetChannelCode( MIDI_GetChannelCode() );

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

Profile_t* Profile_Read(uint8_t profileIndex)
{
	
	if( profileIndex >= DEFAULT_PROFILE )
	{
		return (Profile_t*)&Default_Profile; 	
	} 
   return (Profile_t*)PROFILE(profileIndex);   
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

