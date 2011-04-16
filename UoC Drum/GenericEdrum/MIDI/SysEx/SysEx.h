#ifndef	_SYSEX_ROUTINES
#define	_SYSEX_ROUTINES

#include "Profiles\profiles.h"

#define GET_SYSEX_DATA (0xFF)
#define SEND_SYSEX_STOP (0xFFFF)
#define RECEIVING_SYSEX_DATA (1)
#define SYSEX_DATA_ERROR (3)

void SysExSendNextByte(void* data, uint16_t count);
uint8_t IsReceivingSysExData(uint8_t state);
void ParseSysExData(uint8_t nextByte);
void SysExFlush(void);
void SysEx_ReceiveError(void);


#endif
