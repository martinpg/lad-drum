
#include <stdint.h>
#include "MIDI/midi.h"
#include "hardwareSpecific.h"

#include "firmwareUpdate/firmwareUpdate.h"

#include "flashmem/lowlevel_flashmem.h"

static uint32_t firmwareDataCount;
static uint32_t firmwareByteCount;
static uint32_t firmwareAddress;

void FirmwareCheckForErase(void)
{

   if( ((firmwareAddress & (FLASH_BLOCK_SIZE - 1)) == 0) &&
       (firmwareAddress <= APP_END))
   {
      _flashmem_erase(firmwareAddress);
   }
}


void FirmwareCheckForFinalise(void)
{
   if( (firmwareAddress & (FLASH_BLOCK_SIZE - 1)) == 0)
   {
      PORTD ^= (1 << 7);
      _flashmem_finalise(firmwareAddress-2);
   }
}

void FirmwareUpdateError()
{
   firmwareDataCount = 0;
   firmwareByteCount = 0;
   firmwareAddress = 0;
   //asm volatile("jmp 0"::);
}

void ParseFirmwareData(uint8_t nextByte) 
{
   /* data is sent as 2x 7 bit bytes, so the first byte needs to be stored */
   static uint8_t data;
   static uint8_t firmwareData[2];
 

   switch (firmwareDataCount)
   {
      case 0:
         if( nextByte != MIDI_SYSEX_START )
         {
            FirmwareUpdateError();
            return;
         }         
      break;   
      
      case 1:
         if( nextByte != MIDI_MANUFACTURER )
         {
            FirmwareUpdateError();
            return;
         }   
      break;
         
      case 2:
         if( nextByte != MIDI_DEVICE_CODE )
         {
            FirmwareUpdateError();
            return;
         }
      break;

      case 3:
         //ReceiveFirmwareInit();
      break;

      default:
         
      break;

   }
   
   if( firmwareDataCount >= 3 )
   {
      /* On successful download */
      if( (nextByte == MIDI_SYSEX_STOP) )
      {
         if(firmwareAddress & (FLASH_BLOCK_SIZE - 1))
         {
            _flashmem_finalise(firmwareAddress);
         }
         asm volatile("jmp 0"::);
         /* Finish here and restart */
         //FirmwareUpdateError();
      }
      
      /* Check if Datacount is odd */    
      if( !(firmwareByteCount & 0x01) )
      {
         if( nextByte )
         {
            data |= 0x80;
         }
      }
      else
      {
         data |= nextByte;
         uint8_t index = (firmwareByteCount >> 1) % 2;
         firmwareData[index] = data;
         data = 0;

         if( index ) 
         {
            if( firmwareAddress < APP_END )
            {
               /* See if we need to erase the current page */
               FirmwareCheckForErase();
               _flashmem_writeWord(firmwareAddress, firmwareData[0] | firmwareData[1] << 8 );
               firmwareAddress = firmwareAddress + 2;
               FirmwareCheckForFinalise();
            }
         }
      }
      firmwareByteCount++;
   }   
   firmwareDataCount++;
}
