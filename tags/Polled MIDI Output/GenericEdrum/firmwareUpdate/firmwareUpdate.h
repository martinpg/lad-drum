/* 20/02/2011 EDrum
 * Allows the updating of Firmware over SysEx messages */

#ifndef 	_FIRMWAREUPDATE_H
#define	_FIRMWAREUPDATE_H


#define	UART_STATUS_REG	UCSRA
#define	TRANSMIT_COMPLETE_BIT	TXC
#define	RECEIVE_COMPLETE_BIT	RXC
#define	UART_DATA_REG	UDR

void ReceiveFirmwareInit(void);
void FirmwareUpdateError();
void ParseFirmwareData(uint8_t nextByte);
void FirmwareCheckForFinalise(void);





#endif
