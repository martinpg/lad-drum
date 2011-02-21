#ifndef 	_FLASHMEM_ROUTINES
#define	_FLASHMEM_ROUTINES

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
#define FLASH_WORD_WRITE(address, data)   boot_page_fill_safe(address, data)
#define FLASH_FINALISE_WRITE(address)     boot_page_write_safe(address)

#define FLASH_RELEASE()                   boot_rww_enable_safe()

void flashmem_bufferedWrite(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM);


void _flashmem_erase(uint32_t address);

void _page_write(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM);
void _flashmem_write(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM);


#endif