/* SysEx Data Routines */



#include <stdint.h>
#include <stdlib.h>
#include "EDrum.h"
#include "hardwareSpecific.h"
#include "MIDI/midi.h"
#include "Profiles/profiles.h"
#include "Menu/Menu.h"
#include "MenuSettings.h"
#include "SoftTimer/softtimer.h"
#include "SysEx.h"

#include "hardUart/hardUart.h"
#include "edrumAVRsharedfunctions.h"

static uint16_t DataCount;
#define putMIDI(x) MIDI_Tx(x)

void SysExSendNextByte(void* data, uint16_t count)
{
   uint8_t* buffer = (uint8_t*)data;


   switch(count)
   {
      case 0:
         MIDI_Tx(MIDI_SYSEX_START);
         break;
      case 1:
         MIDI_Tx(MIDI_MANUFACTURER);
         break;
      case 2:
         MIDI_Tx(MIDI_DEVICE_CODE);
         break;
      case (sizeof(Profile_t)+3):
         MIDI_Tx(MIDI_SYSEX_STOP);
         break;
      default:
         if( ((buffer[count-3]) & 0x80) )
         {
            MIDI_Tx(1);
         }
         else
         {
            MIDI_Tx(0);
         }
         MIDI_Tx( (buffer[count-3]) & 0x7F );
         break;
   }

}
/*
void SysexSend(void* data, uint16_t len, uint8_t outStream)
{
   uint16_t i = 0;
   uint8_t* buffer = (uint8_t*)data;

   putMIDI(MIDI_SYSEX_START);
   putMIDI(MIDI_MANUFACTURER);
   putMIDI(MIDI_DEVICE_CODE);
   

	while( i++ < len )
	{
      // Ensure all bytes sent are less than 128 or 0x7F
      if( (*buffer & 0x80) )
      {
         putMIDI(1);
      }
      else
      {
         putMIDI(0);
      }
      
      putMIDI( (*buffer++) & 0x7F );
	}

	putMIDI(MIDI_SYSEX_STOP);
}*/

uint8_t IsReceivingSysExData(uint8_t state)
{  
   static uint8_t ReceivingSysExData;     
   if( state == GET_SYSEX_DATA )
   {
       return ReceivingSysExData;
   }
   else
   {
      ReceivingSysExData = state;
      return ReceivingSysExData;   
   }   
}


void ParseSysExData(uint8_t nextByte)
{
   
   uint8_t* profilePtr;
   
   /* data is sent as 2x 7 bit bytes, so the first byte needs to be stored */
   static uint8_t data;
   
   switch (DataCount)
   {
      case 0:
         if( nextByte != MIDI_SYSEX_START )
         {
            SysEx_ReceiveError();
            return;
         }         
      break;   
      
      case 1:
         if( nextByte != MIDI_MANUFACTURER )
         {
            SysEx_ReceiveError();
            return;
         }   
      break;
         
      case 2:
         if( nextByte != MIDI_DEVICE_CODE )
         {
            SysEx_ReceiveError();
            return;
         }
      break;

      default:
         
      break;

   }
   
   profilePtr = (uint8_t*)&CurrentProfile;
   
   if( DataCount >= 3 )
   {
      
      /* On successful download */
      if( (nextByte == MIDI_SYSEX_STOP) )
      {
         
         if(  DataCount == ((sizeof(Profile_t)*2) + 3) )
         {
            uint8_t i;
            char outputString[4];
            
              
            primaryMenu.MenuPrint_P( PSTR("Download Successful!"));
      
            utoa( DataCount, outputString, 10);
      
            primaryMenu.MenuNewLine();
            
            primaryMenu.MenuPrint(outputString);
      
            /* Implement the changes */
         	MIDI_SetBaud(MIDI_GetBaud());
         	MIDI_SetChannelCode( MIDI_GetChannelCode() );
         
         	/* Update the sensor select */
         	SensorInputSelect(GetSensorInput());
         	
         	/* Update Activated Analogue Channels */
            UpdateActiveChannels();
            /* Update the Retrigger periods */
            UpdateChannelRetriggers();
            
      
            for( i = 0; i < 5; i++ )
         	{	
         		_delay_ms(200);
         	}
         	
         	/* To exit the Receive SysEx Sub-menu */
   	      SysExFlush(); 
         	Menu_UpOneLevel(&primaryMenu);
       		MenuUpdate(&primaryMenu, RESET_MENU);
       		return;
         }
         else
         {
            SysEx_ReceiveError();
            return;
         }
      }
      
      if( (DataCount >  (sizeof(Profile_t)*2) + 3) )
      {
         SysEx_ReceiveError();
         return;
      }
      
      /* Check if Datacount is odd */    
      if( !((DataCount - 3) & 0x01) )
      {
         if( nextByte )
         {
            data |= 0x80;
         }
      }
      else
      {
         data |= nextByte;
         profilePtr[(DataCount - 3) >> 1] = data;
         data = 0;
      }
   }   
   DataCount++;
}

void SysEx_ReceiveError(void)
{
   uint8_t i;
   
   SysExFlush();
   IsReceivingSysExData(SYSEX_DATA_ERROR);
 
   primaryMenu.MenuPrint_P( PSTR("Invalid Data!"));
   
   for( i = 0; i < 5; i++ )
	{	
		_delay_ms(200);
	}
	
	/* To exit the Receive SysEx Sub-menu */
	
	Menu_UpOneLevel(&primaryMenu);
	MenuUpdate(&primaryMenu, RESET_MENU);
     
}

void SysExFlush(void)
{
   DataCount = 0;
   IsReceivingSysExData(!RECEIVING_SYSEX_DATA);
   ActiveProcess = DEFAULT_PROCESS;
   SoftTimer_TimerInit();
}














