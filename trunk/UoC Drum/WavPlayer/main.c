 
#include "hardwareSpecific.h"
#include "SD_MMC/sd.h"
#include "PetitFS/pff.h"
#include "SPI/spi.h"
#include "mmculib/uint16toa.h"
#include "waveplayer.h"



waveHeader_t wavefile;

uint8_t outputString[20];
WORD bytesWritten = 0;




int main(void) 
{
   DWORD fileptr = 0;  
   uint8_t ret = 0;   

   _delay_ms(100);

   sei(); 

   uartInit(10,0);
   SPI_Init();

   _delay_ms(100);

   DDRC |= (1<<4);
   


   DDRB |= (1<<0);




   //uartTxString_P( PSTR("Entering Loop") );

      /* Initialise SD Card */
   if( SD_Init() == SD_SUCCESS )
   {
      ret = pf_mount(&filesys);
   }


   if( ret == RES_OK )
   {
      uartTxString_P( PSTR("Mount success") );
   }

   /* Goto start of file */
   if( pf_lseek(0) == FR_OK )
   {
      ret = waveParseHeader(&wavefile, "1.wav");
   }

   pf_read(Buff, WAVE_OUTBUFFER_SIZE, &bytesWritten);
   if( bytesWritten != WAVE_OUTBUFFER_SIZE )
   {
      uartTxString_P(PSTR("Read ErrorStart!"));
   }

   
   uartTxString_P(PSTR("Loop Starting"));



   for( ;; )
   {
      
      //uartTx(OCR2);
      /* Is a mutliple of WAVE_OUTBLOCK_SIZE */
      /* If we are ready to receive the next bytes then do it */
      if( waveContinuePlaying(&wavefile) == 0)
      {
         waveAudioOff();
         uartTxString_P( PSTR("Wave Finished!"));
         break;
      }
      //waveContinuePlaying();
      /* Goto start of file */
       //
   }
   
   if( pf_mount(0) == FR_OK )
   {
      uartTxString_P( PSTR("Unmounted!") );
   }

   while(1);

   return 0;
}






ISR(SIG_OUTPUT_COMPARE2)
{
   //sei();
   waveProcessBuffer(&wavefile);

}


ISR(SIG_UART_RECV)
{
   uint8_t buffer = UDR;
   sei();
	uartTx(buffer);


   switch( buffer )
   {
      case '1':

      break;

   }

}
