/* MSP430 UART Routines
 * Adrian Gin
 *
 */

#ifndef _UART_H_
#define _UART_H_ 
 
#include <stdint.h>

#define USART0    (0)
#define USART1    (1)


void UART_Select(uint8_t module);
void UART_Init(void);
void UART_ValidateModule(void);
void UART_SetBaudRate(uint8_t BR_Hi, uint8_t BR_Lo);

void wait_for_tx_ready(void);

void UART_Tx(uint8_t byte);
void UART_TxString(uint8_t *string);
void UART_TxString_P(const uint8_t *string);

void UART_TxDump(uint8_t* buffer, uint16_t nbytes );



#endif



