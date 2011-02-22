
#include <stdint.h>
#include "MIDI/midi.h"
#include "hardwareSpecific.h"

#include "firmwareUpdate/firmwareUpdate.h"

#include "flashmem/flashmem.h"

static uint32_t firmwareDataCount;
static uint32_t firmwareByteCount;
static uint32_t firmwareAddress;

void ReceiveFirmware(void)
{


   /*Enable the pull up on RXD */
   PORTD |= (1 << PD0);

   UBRRL = 39;


   uint32_t address;
   /* Erase the entire application section */
   for(address = 0; address < FLASH_END ;address += FLASH_BLOCK_SIZE)
   {
      _flashmem_erase(address);
   }

   firmwareDataCount = 0;

   //_flashmem_write(0, "Hello", 4, 0);

   while(1)
   {
      while(!(UART_STATUS_REG & (1 << RECEIVE_COMPLETE_BIT)));  // wait for data
      //UART_DATA_REG = UART_DATA_REG;
      ParseFirmwareData(UART_DATA_REG);
      if( (firmwareAddress % FLASH_BLOCK_SIZE) == 0 &&
          (firmwareAddress) && 
          (firmwareByteCount % 4) == 0 )
      {
         UDR = 0xFE;
         _flashmem_finalise(firmwareAddress-2);
      }
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
