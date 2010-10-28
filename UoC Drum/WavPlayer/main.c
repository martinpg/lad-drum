 
#include "hardwareSpecific.h"
#include "SD_MMC/sd.h"
#include "PetitFS/pff.h"
#include "SPI/spi.h"
#include "mmculib/uint8toa.h"
#include "mmculib/uint16toa.h"

int main(void) 
{

   FATFS filesys;
		
	uint8_t outputString[20];
   DWORD fileptr = 0;  

   uint8_t ret = 0;   
   WORD bytesWritten = 0;

   uartInit(39,0);
   SPI_Init();

   DDRC |= (1<<4);
   
   sei();
   //_delay_ms(30);

   

   //uartTxString_P( PSTR("Entering Loop") );

      /* Initialise SD Card */
   if( SD_Init() == SD_SUCCESS )
   {
      /* Mount the drive */
      //uartTxString_P( PSTR("Mounting..") );
      ret = pf_mount(&filesys);
   }


   if( ret == RES_OK )
   {
      uartTxString_P( PSTR("Opening File") );
      if( pf_open("hello.txt") == RES_OK )
      {
         //uartTxString_P( PSTR("File Opened!") );
      }
   }

   /* Goto start of file */
   if( pf_lseek(0) == FR_OK )
   {
      //uartTxString_P( PSTR("lseek") );
   }

   while( 1 )
   {
      _delay_ms(30);
      _delay_ms(30);
      /* Goto start of file */
      fileptr = fileptr + 512;

      uint16toa(fileptr, outputString, 0);
      uartTxDump(outputString, 5);


      if( pf_lseek(fileptr) == FR_OK )
      {
         //uartTxString_P( PSTR("lseek") );
      }

      if( pf_write("Hello", 5, &bytesWritten) == FR_OK )
      {
         //fileptr = fileptr + bytesWritten;
         uartTxString_P( PSTR("Write Okay!") );
      }
      else
      {
         uartTxString_P( PSTR("Write fail") );
      }

     if( pf_write(" Adrian", 7, &bytesWritten) == FR_OK )
      {
         //fileptr = fileptr + bytesWritten;
         uartTxString_P( PSTR("Write Okay!") );
      }
      else
      {
         uartTxString_P( PSTR("Write fail") );
      }
      
      if( pf_write(0, 0, &bytesWritten) == FR_OK )
      {
         uartTxString_P( PSTR("Write Finalised!") );
      }
      else
      {
         uartTxString_P( PSTR("SD Card fail!") );
      }


     	MMC_CS_PORT |= (1 << MMC_CS_PIN);
   	//SPCR |= (1 << CPHA) | (1 << SPR1);
      
   }
   


   if( pf_mount(0) == FR_OK )
   {
      //uartTxString_P( PSTR("Unmounted!") );
   }
   //uartTxString_P( PSTR("SD Card Initialised!") );



   //return 0;

}


ISR(SIG_UART_RECV)
{
   uint8_t buffer = UDR;
   sei();
	//uartTx(UDR);
}
