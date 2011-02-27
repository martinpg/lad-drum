
#include "hardwareSpecific.h" 
#include "edrumAVRsharedfunctions.h"

#include "USBMIDI/USBMIDI.h"


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



// This must be declared "naked" because we want to let the
// bootloader function handle all of the register push/pops
// and do the RETI to end the handler.
void INT0_vect(void) __attribute__((naked));
ISR(INT0_vect)
{
    asm("jmp 0x7004");
}

ISR(SIG_UART_RECV)
{
   uint8_t buffer;

   buffer = UDR;
   sei();
   USBMIDI_PutByte(buffer);
}


int main(void)
{
   uartInit();

   UBRRL = 39;
   usbMIDIMessage_t message;
   message.header = 0x09;
   message.MIDIData[0] = 0x99; 
   message.MIDIData[1] = 0x55;
   message.MIDIData[2] = 0x55;


   usbInit();

   sei();

   while (1)
   {   
      usbPoll();
      USBMIDI_EnableRequests();
      //USBMIDI_ProcessBuffer();

      uint8_t nextByte;
      nextByte = USBMIDI_GetByte();
      if( nextByte != NO_DATA_BYTE )
      {
         USBMIDI_PutByte(nextByte);
      }

      USBMIDI_OutputData();



   }
}
