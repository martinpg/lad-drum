#include <avr/io.h>
#include "hardwareSpecific.h"
#include "EdrumBootloader.h"
#include "flashmem/flashmem.h"

#include "firmwareUpdate/firmwareUpdate.h"

#include "hardUart/hardUart.h"


//Must be a power of two
#define RX_BUFFER_SIZE (32)
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
   RxBuffer[rxWritePtr++] = buffer;
   rxWritePtr = (rxWritePtr & RX_BUFFER_MASK);

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
}


int main(void)
{
   MCUCSR = (1 << JTD);
   MCUCSR = (1 << JTD);

   sei();
   bootloader_Init();

   /* If bootloader condition */
   if( BOOTLOADER_CONDITION )
   {
      uartInit(0);
      uartSetBaud(0, 39);
      
      GICR = (1 << IVCE);
      GICR = (0 << IVSEL);
      sei();

      while(1)
      {
           /* Process messages in the UART Rx buffer is there are any */
           if( rxReadPtr != rxWritePtr )
           {
              uint8_t nextByte = RxBuffer[rxReadPtr++];
              rxReadPtr = (rxReadPtr & RX_BUFFER_MASK);
              ParseFirmwareUpgrade(nextByte);
           }
      }

   }
   else
   {
      bootloader_leave();
   }


   return 0;
}

