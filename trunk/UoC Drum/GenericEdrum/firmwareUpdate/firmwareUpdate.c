
#include <stdint.h>
#include "MIDI/midi.h"
#include "hardwareSpecific.h"

#include "firmwareUpdate/firmwareUpdate.h"

#include "flashmem/flashmem.h"

static uint32_t firmwareDataCount;
static uint32_t firmwareByteCount;
static uint32_t firmwareAddress;

void ReceiveFirmwareInit(void)
{
   uint32_t address;
   /* Erase the entire application section */
   for(address = 0; address < APP_END ;address += FLASH_BLOCK_SIZE)
   {
      _flashmem_erase(address);
   }
   firmwareDataCount = 0;
}


void FirmwareCheckForFinalise(void)
{
   if( (firmwareAddress % FLASH_BLOCK_SIZE) == 0 &&
       (firmwareAddress) && 
       (firmwareByteCount % 4) == 0 )
   {
      _flashmem_finalise(firmwareAddress-2);
   }
}

void FirmwareUpdateError()
{

   asm volatile("jmp 0"::);
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
         
      break;

      default:
         
      break;

   }
   
   if( firmwareDataCount >= 3 )
   {
      /* On successful download */
      if( (nextByte == MIDI_SYSEX_STOP) )
      {
         if(firmwareAddress % FLASH_BLOCK_SIZE)
         {
            _flashmem_finalise(firmwareAddress);
         }
         /* Finish here and restart */
         FirmwareUpdateError();
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
            _flashmem_writeWord(firmwareAddress, firmwareData[0] | firmwareData[1] << 8 );
            firmwareAddress = firmwareAddress + 2;
         }
      }
      firmwareByteCount++;
   }   
   firmwareDataCount++;
}
