/* MSP430 UART Routines for MSP430F2X devices
 * Adrian Gin
 *
 */

#ifndef _UART_H_
#define _UART_H_ 
 
#include <stdint.h>
#include "RingBuffer/ringbuffer.h"

#define USART0    (0)
#define USART1    (1)

#define TXBUFFER_SIZE   (30)
#define RXBUFFER_SIZE   (30)
#define USEBUFF	(1)

extern RINGBUFFER_T ReceiveBuffer;

void UART_Select(uint8_t module);
void UART_Init(void);
void UART_ValidateModule(void);
void UART_SetBaudRate(uint8_t BR_Hi, uint8_t BR_Lo);

void wait_for_tx_ready(void);

void UART_Tx(char byte);
void UART_TxString(char *string);
void UART_TxString_P(const char *string);

void UART_TxDump(uint8_t* buffer, uint16_t nbytes );



#endif



