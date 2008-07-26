#ifndef	_SYSEX_ROUTINES
#define	_SYSEX_ROUTINES



#define GET_SYSEX_DATA (0xFF)
#define RECEIVING_SYSEX_DATA (1)
#define SYSEX_DATA_ERROR (3)

void SysexSend(Profile_t* data);
uint8_t IsReceivingSysExData(uint8_t state);
void ReceiveSysEx(void);
void ParseSysExData(uint8_t nextByte);
void SysExFlush(void);
void SysEx_ReceiveError(void);


#endif
