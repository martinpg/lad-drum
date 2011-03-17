#ifndef _EDRUM_AVR_SHARED_FUNCTIONS
#define _EDRUM_AVR_SHARED_FUNCTIONS

#include <stdint.h>
#include "hardwareSpecific.h"


typedef void (*PF_VOID)(void);
typedef void (*PF_usbFunctionWriteOut)(uint8_t*, uint8_t);
typedef void (*PF_USBMIDI_PutByte)(uint8_t);
typedef void (*PF_usbSetInterrupt)(uint8_t*, uint8_t);
typedef uint8_t (*PF_USBMIDI_GetByte)(void);
typedef void (*PF_flashmem_erase)(uint32_t);
typedef void (*PF_flashmem_finalise)(uint32_t);
typedef void (*PF_flashmem_writeWord)(uint32_t, uint16_t);

#define JUMP_TABLE_FUNCTION(x) ((JUMP_TABLE_LOCATION + (4*x))/2)


static __inline__ void usbPoll(void)                                          {((PF_VOID)(JUMP_TABLE_FUNCTION(0)))();}
static __inline__ void usbFunctionWriteOut(uint8_t* data, uint8_t len)        {((PF_usbFunctionWriteOut)(JUMP_TABLE_FUNCTION(1)))(data, len);}
static __inline__ void usbSetInterrupt(uint8_t* data, uint8_t len)            {((PF_usbSetInterrupt)(JUMP_TABLE_FUNCTION(2)))(data, len);}
static __inline__ void bootloader_enter()                                     {((PF_VOID)(JUMP_TABLE_FUNCTION(3)))();}
static __inline__ void usbInit()                                              {((PF_VOID)(JUMP_TABLE_FUNCTION(4)))();}
static __inline__ void USBMIDI_PutByte(uint8_t inbyte)                        {((PF_USBMIDI_PutByte)(JUMP_TABLE_FUNCTION(5)))(inbyte);}
static __inline__ void USBMIDI_OutputData()                                   {((PF_VOID)(JUMP_TABLE_FUNCTION(6)))();}
static __inline__ void USBMIDI_EnableRequests()                               {((PF_VOID)(JUMP_TABLE_FUNCTION(7)))();}
static __inline__ uint8_t USBMIDI_GetByte()                                   {return ((PF_USBMIDI_GetByte)(JUMP_TABLE_FUNCTION(8)))();}
static __inline__ void _flashmem_erase(uint32_t address)                      {((PF_flashmem_erase)(JUMP_TABLE_FUNCTION(9)))(address);}
static __inline__ void _flashmem_finalise(uint32_t address)                   {((PF_flashmem_finalise)(JUMP_TABLE_FUNCTION(10)))(address);}
static __inline__ void _flashmem_writeWord(uint32_t address, uint16_t data)   {((PF_flashmem_writeWord)(JUMP_TABLE_FUNCTION(11)))(address, data);}
static __inline__ void _flashmem_release()                                    {((PF_VOID)(JUMP_TABLE_FUNCTION(12)))();}


#endif
