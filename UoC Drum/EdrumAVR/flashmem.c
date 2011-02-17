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

#include <avr/boot.h>
#include <stdint.h>

#include "hardwareSpecific.h"

#include <util/delay.h>
#include "flashmem.h"


void _page_write(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM)
{
   cli();
   _flashmem_write(address, buffer, len, isPGM);


}

void flashmem_bufferedWrite(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM)
{
   _flashmem_write(FLASH_TEMP_BUFFER, address, FLASH_BLOCK_SIZE, 1);
   _flashmem_write(FLASH_TEMP_BUFFER, buffer, len, isPGM);
   _delay_ms(5);
   _flashmem_write(address, FLASH_TEMP_BUFFER, len, 1); 
}

BOOTLOADER_SECTION void _flashmem_erase(uint32_t address)
{
   cli();
   FLASH_PAGE_ERASE(address);
   FLASH_RELEASE();
   sei();
}


/* A raw write to anywhere */
BOOTLOADER_SECTION void _flashmem_write(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM)
{
   uint32_t i;
   uint16_t overflow;
   uint32_t baseAddr;

   cli();

   for (i=0; i<FLASH_BLOCK_SIZE; i+=2)
   {
      uint16_t w;
      // Set up little-endian word.
      if( isPGM )
      {
         w = FLASH_GET_PGM_BYTE(buffer + i);
         w += FLASH_GET_PGM_BYTE(buffer + i + 1) << 8;
      }
      else
      {
         w = *buffer++;
         w += (*buffer++) << 8;
      }
      FLASH_WORD_WRITE(address + i, w);
   }

   FLASH_FINALISE_WRITE(address);     // Store buffer in flash page.
   FLASH_RELEASE();
   sei();
}



