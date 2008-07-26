/* SysEx Data Routines */



#include <stdint.h>
#include "main.h"
#include "MIDI/midi.h"
#include "Profiles/profiles.h"
#include "Menu/Menu.h"
#include "Delay/delay.h"
#include "MenuSettings.h"
#include "UART/UART.h"
#include "mmculib/uint16toa.h"
#include "Sample/sample.h"
#include "SysEx.h"

static uint16_t DataCount = 0;

void SysexSend(Profile_t* data)
{
   uint16_t i = 0;
   uint8_t* buffer = (uint8_t*)data;
   
   UART_Tx(MIDI_SYSEX_START);
   UART_Tx(MIDI_MANUFACTURER);
   UART_Tx(MIDI_DEVICE_CODE);
   

	while( i++ < sizeof(Profile_t) )
	{
      /* Ensure all bytes sent are less than 128 or 0x7F */
      if( (*buffer & 0x80) )
      {
         UART_Tx(1);
      }
      else
      {
         UART_Tx(0);   
      }
      
      UART_Tx( (*buffer++) & 0x7F );
	}

   UART_Tx(MIDI_SYSEX_STOP);
}

uint8_t IsReceivingSysExData(uint8_t state)
{  
   static uint8_t ReceivingSysExData = 0;     
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


void ReceiveSysEx(void)
{
   while( ringbuffer_len((RINGBUFFER_T*)&ReceiveBuffer)  )
   {
      ParseSysExData(ringbuffer_get((RINGBUFFER_T*)&ReceiveBuffer));
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
            uint8_t outputString[4];
            
              
            primaryMenu.MenuPrint_P( PSTR("Download Successful!"));
      
            uint16toa( DataCount, outputString, 0);
      
            primaryMenu.MenuNewLine();
            
            primaryMenu.MenuPrint(outputString);
      
            /* Implement the changes */
         	MIDI_SetRate(MIDI_GetRate());
         	MIDI_SetBaud(MIDI_GetBaud());
         	MIDI_SetChannelCode( MIDI_GetChannelCode() );
         
         	/* Update the sensor select */
         	SensorInputSelect(GetSensorInput());
         	
         
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
      
      /* For the first byte of the 2 byte SysEx data */    
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
   TimerInit();
}














