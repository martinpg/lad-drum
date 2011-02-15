#ifndef 	_FLASHMEM_ROUTINES
#define	_FLASHMEM_ROUTINES


void _page_write(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM);
void _flashmem_write(uint32_t address, uint8_t* buffer, int16_t len, uint8_t isPGM);


#endif
