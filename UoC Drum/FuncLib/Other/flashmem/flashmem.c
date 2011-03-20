/* flashmem.c
 * Generic Flash Write routines
 * 
 * Date: 15/02/2011
 * Author : Adrian Gin (adrian.gin@gmail.com)
 *
 * The routine uses 1 page of the flash as a temporary scratch pad
 * Make sure the code is placed in a 'bootloader section',
 * Interrupts should be disabled to prevent flash corruption
 */

#include <stdint.h>

#include "hardwareSpecific.h"
#include "flashmem/flashmem.h"

#include "edrumavrsharedfunctions.h"

/* A raw write to anywhere */
void _flashmem_write(uint32_t address, void* buffer, int16_t len, uint8_t isPGM)
{
   uint16_t i;

   _flashmem_erase(address);

   for (i=0; i<len; i+=2)
   {
      uint16_t w;
      // Set up little-endian word.
      if( isPGM )
      {
         w = FLASH_GET_PGM_WORD(buffer + i);
      }
      else
      {
         w = *(uint8_t*)buffer++;
         w += (*(uint8_t*)buffer++) << 8;
      }
      _flashmem_writeWord(address + i, w);
   }
   _flashmem_finalise(address);     // Store buffer in flash page.
}


// Writes data to the temporary buffer first
void flashmem_bufferedWrite(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM)
{

   uint16_t overflow;
   uint32_t baseAddr;
   
   /* Because we goto the bootloader, we need to stop all interrupts */
   enableFlashProgramming(FLASH_PROGRAMMING_ENTER);
   
   while( len > 0 )
   {
      //Copy destination location to the temporary buffer (can be RAM)
      overflow = address % FLASH_BLOCK_SIZE;
      baseAddr = address - overflow;
      _flashmem_write(FLASH_TEMP_BUFFER, baseAddr, FLASH_BLOCK_SIZE, 1);

      //Erase the destination location
      _flashmem_erase(baseAddr);

   
      //Copy back the parts of the destination which are not to be changed.
      //Start parts
      uint16_t i;
      uint8_t data[2];
      for( i = 0 ; i < overflow; i++ )
      {
         data[i%2] = FLASH_GET_PGM_BYTE(FLASH_TEMP_BUFFER + i);
         // We can only write to the flash in WORDS (16 bits)
         if( i % 2 ) 
         {
            _flashmem_writeWord(baseAddr + i, data[0] | data[1] << 8);
         }
      }

      
      for( ; i < FLASH_BLOCK_SIZE; i++ )
      {
         // Write the actual data from the source
         if( len > 0 )
         {
            if( isPGM )
            {
               data[i%2] = FLASH_GET_PGM_BYTE((uint16_t*)buffer++);
            }
            else
            {
               data[i%2] = *buffer++;
            }
            len--;
         }
         // Don't change the remainder of the destination
         else
         {
            data[i%2] = FLASH_GET_PGM_BYTE(FLASH_TEMP_BUFFER + i);
         }
         // We can only write to the flash in WORDS (16 bits)
         if( i % 2 ) 
         {
            _flashmem_writeWord(baseAddr + i, data[0] | data[1] << 8);
         }
      }
      _flashmem_finalise(baseAddr);
      address = baseAddr + FLASH_BLOCK_SIZE;
   }

   /* Because we goto the bootloader, we need to stop all interrupts */
   enableFlashProgramming(FLASH_PROGRAMMING_EXIT);
}

