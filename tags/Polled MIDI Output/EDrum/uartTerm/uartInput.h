/** \file uartInput.h
	 \brief UART Commands Interface
*/

/**	 

	\ingroup avr_datalogger
 	\defgroup uartInput UART Commands Parser
 	\code #include "uartInput.h" \endcode

 	 
	 \par Description
    Provides a buffered interface to receive UART commands.
*/
//@{
   
   
#ifndef	_UART_INPUT
#define	_UART_INPUT

#define UART_COMPARE_MAX_LENGTH  15
#define UART_NULL                '\0'
#define UART_LINEFEED            '\n'
#define UART_CARRIAGE_RETURN     '\r'
#define UART_DELETE              (127)


/** If compareString is matched by the uart input, 
 * function is called */
typedef struct _uart_compare
{
   PGM_P compareString;
   uint8_t  activated;
   void (*function)(void* data);   
} UartCompare;


void switch_uart_input(uint8_t nextChar);
void branch_input(uint8_t* inputStr);
void uartTermHelp(void* data);

#endif

