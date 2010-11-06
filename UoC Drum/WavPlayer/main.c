 
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


   uartInit(10,0);
   SPI_Init();

   DDRC |= (1<<4);
   
   sei();  

   //uartTxString_P( PSTR("Entering Loop") );

      /* Initialise SD Card */
   if( SD_Init() == SD_SUCCESS )
   {
      ret = pf_mount(&filesys);
   }


   if( ret == RES_OK )
   {
      uartTxString_P( PSTR("Mount success") );
      if( pf_open("1.wav") == RES_OK )
      {
         //uartTxString_P( PSTR("File Opened!") );
      }
   }

   /* Goto start of file */
   if( pf_lseek(0) == FR_OK )
   {
      ret = waveParseHeader(&wavefile, "1.wav");
   }


   uartNewLine();

   uint16toa(wavefile.channelCount, outputString, 0);
   uartTxString_P( PSTR("Channel Count: ") );
   uartTxString(outputString);
   uartNewLine();

   uint16toa(wavefile.resolution, outputString, 0);
   uartTxString_P( PSTR("Res: ") );
   uartTxString(outputString);
   uartNewLine();

   uint16toa(wavefile.sampleRate, outputString, 0);
   uartTxString_P( PSTR("SampleRate: ") );
   uartTxString(outputString);
   uartNewLine();

   uartTxString_P( PSTR("DataSize: ") );
   uint16toa(((uint32_t)(wavefile.dataSize)>>16), outputString, 0);
   uartTxString(outputString);
   uartNewLine();
   uint16toa(wavefile.dataSize, outputString, 0);
   uartTxString(outputString);
   uartNewLine();


   uint8_t i;
   uint8_t direction = 1;

   pf_read(Buff, WAVE_OUTBUFFER_SIZE, &bytesWritten);
   if( bytesWritten != WAVE_OUTBUFFER_SIZE )
   {
      uartTxString_P(PSTR("Read ErrorStart!"));
   }

   
   uartTxString_P(PSTR("Loop Starting"));

   waveAudioSetup(0);
   waveAudioOn();


   for( ;; )
   {
      
      //uartTx(OCR2);
      /* Is a mutliple of WAVE_OUTBLOCK_SIZE */
      /* If we are ready to receive the next bytes then do it */
      if( waveContinuePlaying(&wavefile) == 0)
      {
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
}
