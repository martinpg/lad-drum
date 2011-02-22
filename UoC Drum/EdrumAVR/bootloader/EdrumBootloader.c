#include <avr/io.h>
#include "hardwareSpecific.h"
#include "EdrumBootloader.h"
#include "flashmem/flashmem.h"

#include "firmwareUpdate/firmwareUpdate.h"


//Must be a power of two
#define RX_BUFFER_SIZE (128)
#define RX_BUFFER_MASK (RX_BUFFER_SIZE - 1)

#define TX_BUFFER_SIZE (32)
#define TX_BUFFER_MASK (TX_BUFFER_SIZE - 1)

uint8_t RxBuffer[RX_BUFFER_SIZE];
volatile uint8_t rxWritePtr;
volatile uint8_t rxReadPtr;


uint8_t TxBuffer[TX_BUFFER_SIZE];
volatile uint8_t txWritePtr;
volatile uint8_t txReadPtr;


ISR(SIG_UART_RECV)
{
   uint8_t buffer = UDR;
   if( rxWritePtr + 1 == rxReadPtr )
   {
      UDR = '!';
   }

   RxBuffer[rxWritePtr++] = buffer;
   rxWritePtr = (rxWritePtr & RX_BUFFER_MASK);
   UDR = (rxWritePtr - rxReadPtr);
}


ISR(USART_TXC_vect)
{
   
}

ISR(SPM_RDY_vect)
{

}




#define	PARITY_MASK		(0x30)
#define	NOPARITY			(0x00)
#define	EVEN				(0x02)
#define	ODD				(0x03)

#define	CHARSIZE_MASK	(0x06)
#define 	BIT8				(0x03)
#define	BIT7				(0x02)
#define	BIT6				(0x01)
#define	BIT5				(0x00)

#define	UCSRCMASK		(0x7F)

void uartInit(void)
{
	UCSRB |= (1<<RXEN) | (1<<TXEN) | (1<<TXCIE) | (1<<RXCIE);	/*Enable Rx and Tx modules*/
	UCSRB &= ~(1<<UCSZ2);				/*Set to 8bit mode*/
	

	/*Select UCSRC to be written to*/	
	/* Set to Asynchronous Mode
	 *			 1 Stop-bit
	 *			 No Parity
	 *			 8-bit char mode
	 */
	UCSRC = (UCSRC & ~( UCSRCMASK ))
				|  (NOPARITY<<UPM0)
				|	(BIT8 << UCSZ0) 
				|  (1<<URSEL);

   /*Enable the pull up on RXD */
   PORTD |= (1 << PD0);
}



void bootloader_Init(void)
{
   /* Monitor the interrupt pin */
   DDRD &= ~(1 << 3);
   PORTD &= ~(1<<3);

   /* Columns as outputs */  
   UI_COL_DIR |= (UI_COLS);
   UI_COL_OUT |= (UI_COLS);

   UI_ROW_DIR &= ~(UI_ROWS);
   UI_ROW_OUT &= ~(UI_ROWS); 
}


void bootloader_leave(void)
{
   cli();
   _flashmem_release();

   GICR = (1 << IVCE);     /* enable change of interrupt vectors */
   GICR = (0 << IVSEL);    /* move interrupts to application flash section */

   asm volatile("jmp 0"::);
}


int main(void)
{
   MCUCSR = (1 << JTD);
   MCUCSR = (1 << JTD);

   bootloader_Init();

   /* If bootloader condition */
   if( BOOTLOADER_CONDITION )
   {
      SPI_DDR |= ((1 << nSS) | (1 << MOSI) );
      SPCR |= ((1 << SPE) | (1 << MSTR)); 
      
      uartInit();
      UBRRL = 39;

      ReceiveFirmwareInit();
      
      GICR = (1 << IVCE);
      GICR = (1 << IVSEL);
      sei();

      while(1)
      {
           /* Process messages in the UART Rx buffer is there are any */
           if( rxReadPtr != rxWritePtr )
           {
              uint8_t nextByte = RxBuffer[rxReadPtr++];
              rxReadPtr = (rxReadPtr & RX_BUFFER_MASK);
              ParseFirmwareData(nextByte);
              FirmwareCheckForFinalise();
           }
      }

   }
   else
   {
      bootloader_leave();
   }


   return 0;
}

