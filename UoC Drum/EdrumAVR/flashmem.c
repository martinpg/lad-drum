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

/* Both in Byte sizes */
//Normally defined in hardwareSpecific.h
//#define FLASH_END
//#define FLASH_BLOCK_SIZE

#define FLASH_TEMP_BUFFER  (FLASH_END - FLASH_BLOCK_SIZE + 1)
#define FLASH_TEMP_BUFFER_BASE (FLASH_TEMP_BUFFER & ~(FLASH_BLOCK_SIZE - 1))

#define FLASH_GET_PGM_WORD(address) pgm_read_word(address)
#define FLASH_GET_PGM_BYTE(address) pgm_read_byte(address)

/* Erases one page */
#define FLASH_PAGE_ERASE(address)         boot_page_erase_safe(address)
#define FLASH_WORD_WRITE(address, data)      boot_page_fill_safe(address, data)
#define FLASH_FINALISE_WRITE(address)     boot_page_write_safe(address)

#define FLASH_RELEASE()                   boot_rww_enable_safe()

void _page_write(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM)
{
   cli();
   _flashmem_write(address, buffer, len, isPGM);


}

void flashmem_bufferedWrite(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM)
{
   _flashmem_write(FLASH_TEMP_BUFFER, buffer, len, isPGM);
   _flashmem_write(address, FLASH_TEMP_BUFFER, len, 1); 
}

/* A raw write to anywhere */
BOOTLOADER_SECTION void _flashmem_write(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM)
{
   uint32_t i;
   uint16_t overflow;
   uint32_t baseAddr;


   while( len > 0)
   {
      /* Do not try to write to invalid addresses */
      if(address + len > FLASH_END)
      {
         return;
      } 
      overflow = address % (FLASH_BLOCK_SIZE);
      baseAddr = (address - overflow);

      FLASH_PAGE_ERASE(baseAddr);

      /* Retain the bytes before */
      for(i = 0; i < overflow; i+=2)
      {
         uint16_t w = FLASH_GET_PGM_BYTE(baseAddr + i);
         w += FLASH_GET_PGM_BYTE(baseAddr + i + 1) << 8;
         FLASH_WORD_WRITE((baseAddr + i), w);
      }
      /* write the actual bytes, until the end of the block */
      for (; i < FLASH_BLOCK_SIZE; i+=2)
      {
         if( len > 0 )
         {
         // Set up little-endian word
            uint16_t w;
            if( isPGM )
            {
               boot_spm_busy_wait();
               w = FLASH_GET_PGM_BYTE(baseAddr + i);
               w += FLASH_GET_PGM_BYTE(baseAddr + i + 1) << 8;
               buffer += 2;
            }
            else
            {
               w = (*buffer++);
               w += (*buffer++) << 8;
            }
            FLASH_WORD_WRITE((baseAddr + i), w);
            len = len - 2;
         }
         else
         {
            boot_spm_busy_wait();
            uint16_t w = FLASH_GET_PGM_BYTE(baseAddr + i);
            w += FLASH_GET_PGM_BYTE(baseAddr + i + 1) << 8;
            FLASH_WORD_WRITE((baseAddr + i), w);
         }
      }

      FLASH_FINALISE_WRITE(baseAddr);
      boot_spm_busy_wait();

      address = baseAddr + FLASH_BLOCK_SIZE;
   }
  
   FLASH_RELEASE();
}





