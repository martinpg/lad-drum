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
   volatile uint32_t i;
   volatile uint16_t overflow;
   volatile uint32_t baseAddr;

   uint8_t sreg = SREG;

   cli();

        //FLASH_PAGE_ERASE(address);


        for (i=0; i<FLASH_BLOCK_SIZE; i+=2)
        {
            _delay_ms(5);
            uint16_t w;
            // Set up little-endian word.
            if( isPGM )
            {
               //do{}while(boot_rww_busy());
               w = FLASH_GET_PGM_BYTE(buffer + i);
               
               //UDR = w;
               //while( (UCSRA & (1<<UDRE)) == 0 );

               w += FLASH_GET_PGM_BYTE(buffer + i + 1) << 8;
               //do{}while(boot_rww_busy());
               //UDR = w >> 8;
               //while( (UCSRA & (1<<UDRE)) == 0 );
               //buffer += 2;
            }
            else
            {
               w = *buffer++;
               w += (*buffer++) << 8;
            }
            //do{}while(boot_rww_busy());
            FLASH_WORD_WRITE(address + i, w);
            //do{}while(boot_rww_busy());
        }

        FLASH_FINALISE_WRITE (address);     // Store buffer in flash page.



   FLASH_RELEASE();
   SREG = sreg;
   sei();
}



uint8_t getPGMbyte(PGM_P address)
{
   return pgm_read_byte(address);
}

