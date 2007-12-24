/* UART Routines for the USART MSP430 Module 
 *
 * Adrian Gin
 */
 
 
#include <io.h>
#include "uart.h"
#include <signal.h>

static int8_t selectedModule = -1;

static uint8_t* UxCTL = (uint8_t*)U0CTL_;
static uint8_t* UxTCTL = (uint8_t*)U0TCTL_;
static uint8_t* UxRCTL = (uint8_t*)U0RCTL_;
static uint8_t* UxMCTL = (uint8_t*)U0MCTL_;   
static uint8_t* UxBR0 = (uint8_t*)U0BR0_;
static uint8_t* UxBR1 = (uint8_t*)U0BR1_;
static uint8_t* UxRXBUF = (uint8_t*)U0RXBUF_;
static uint8_t* UxTXBUF = (uint8_t*)U0TXBUF_; 


void UART_Select(uint8_t module)
{
   UxCTL   =  (uint8_t*)( &U0CTL + module*8 );
   UxTCTL  =  (uint8_t*)( &U0TCTL + module*8);
   UxRCTL  =  (uint8_t*)( &U0RCTL + module*8);
   UxMCTL  =  (uint8_t*)( &U0MCTL + module*8);   
   UxBR0   =  (uint8_t*)( &U0BR0 + module*8);
   UxBR1   =  (uint8_t*)( &U0BR1 + module*8);
   UxRXBUF =  (uint8_t*)( &U0RXBUF + module*8);
   UxTXBUF =  (uint8_t*)( &U0TXBUF + module*8);
   
   selectedModule = module;
}

void UART_Init(void)
{  
   /* Setup the clocks */
  // BCSCTL1 &= ~XT2OFF;
   
   /* SMCLK is XT2 and so is the MCLK */
  // BCSCTL2 |= SELM1 | SELS; ;

   UART_ValidateModule();

   /* Disable the USART Module */
   /* Select UART Mode with 8-bit data and No Parity */
   *UxCTL = (CHAR) | (SWRST);

   /* Transmit Register , Baud Rate Generator Clock */
   *UxTCTL = SSEL1;
  
   /* Set Baud Rate */  
   /* For a speed of 9600 Baud, asssuming a 8192000 Clk speed */
   *UxBR1 = 0x03;
   *UxBR0 = 0x55;
   *UxMCTL = 0x00;
   
   /* Activate the USART Module */
   *UxCTL &= ~(SWRST);
   
   if( selectedModule == 0)
   {
      P3DIR &= ~(1 << 5);
      P3DIR |= (1 << 4);
      
      P3SEL |= 0x30; // P3.4,5 = USART0 TXD/RXD
      ME1 |= (UTXE0 | URXE0);
      
      /* Enable Receive Interrupts */
      IE1 |= URXIE0;
   }
   else
   {
      P3DIR &= ~(1 << 6);
      P3DIR |= (1 << 7);      
      
      P3SEL |= 0xC0;  // P3.6 & 7 = USART1 TXD / RXD
      ME2 |= (UTXE1 | URXE1);
      IE2 |= URXIE1;
   }   
   
   
}


void UART_SetBaudRate(uint8_t BR_Hi, uint8_t BR_Lo)
{
   UART_ValidateModule();
   *UxBR1 = BR_Hi;
   *UxBR0 = BR_Lo;   
}


void UART_ValidateModule(void)
{
   if( selectedModule == -1 )
   {
      UART_Select(USART0);   
   }  
}

/* UART_Tx: Transfers a byte out UARTx */
void UART_Tx(char byte)
{
   UART_ValidateModule();
   *UxTXBUF = byte;
   
   if( selectedModule == USART0 )
   {
      while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?    
   }
   else
   {
      while (!(IFG2 & UTXIFG1));                // USART1 TX buffer ready? 
   }
}


/** Writes nbytes of buffer to the UART */
void UART_TxDump(uint8_t* buffer, uint16_t nbytes )
{
	uint16_t i = 0;
	while( i++ < nbytes )
	{
		UART_Tx(*buffer++);
	}
}

/* Sends a string until a null char is reached */
void UART_TxString(char *string)
{
   while( *string )
   {
      UART_Tx(*string++);  
   }
   
   /* For an extra null char */
   //UART_Tx(0x00);
   
}

/* Sends a string until a null char is reached */
void UART_TxString_P(const char *string)
{
   while( *string )
   {
      UART_Tx(*string++);  
   }
   
   /* For an extra null char */
   //UART_Tx(0x00);
   
}


int putchar(int c)
{
   UART_Tx(c);
   return c;
}

/*

interrupt (UART0RX_VECTOR) usart0_rx()
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
}

interrupt (UART1RX_VECTOR) usart1_rx()
{
  while (!(IFG2 & UTXIFG1));                // USART0 TX buffer ready?
}

*/

