 
#include "hardwareSpecific.h"
#include "SD_MMC/sd.h"
#include "PetitFS/pff.h"
#include "SPI/spi.h"
#include "mmculib/uint16toa.h"
#include "waveplayer.h"



volatile waveHeader_t wavefile;
volatile uint8_t newSongFlag = 0;
volatile uint8_t ProcessBufferFlag = 0;

uint8_t outputString[20];



int main(void) 
{  
   uint8_t ret;
   _delay_ms(10);

   DDRC |= ((1<<4) | (1<<5));
   PORTC |= (1 << 4);

   uartInit(10,0);
   sei(); 

   uartTxString_P( PSTR("1") );

   SPI_Init();

   _delay_ms(100);

   

   uartTxString_P( PSTR("Entering Loop") );

      /* Initialise SD Card */
   if( SD_Init() == SD_SUCCESS )
   {
      ret = pf_mount(&filesys);
   }

   uartTxString_P(PSTR("Loop Starting"));


   for( ;; )
   {
      //uartTx(OCR2);
      /* Is a mutliple of WAVE_OUTBLOCK_SIZE */
      /* If we are ready to receive the next bytes then do it */
      if( (waveIsPlaying()) && (waveContinuePlaying((waveHeader_t*)&wavefile) == 0) )
      {     
         waveAudioOff();
         uartTxString_P( PSTR("Wave Finished!"));
      }
      if( newSongFlag )
      {
         uartTxString_P( PSTR("Playing\n"));
         wavePlayFile( (waveHeader_t*)&wavefile, outputString);

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

         newSongFlag = 0;
      }
      //waveContinuePlaying();
      /* Goto start of file */
       //
   }
   
   if( pf_mount(0) == FR_OK )
   {
      uartTxString_P( PSTR("Unmounted!") );
   }

   return 0;
}






ISR(SIG_OUTPUT_COMPARE2)
{
   //sei();
   waveProcessBuffer((waveHeader_t*)&wavefile);
}


ISR(SIG_UART_RECV)
{
   uint8_t buffer = UDR;
   static uint8_t readPtr = 0;
   sei();

   uartTx(buffer);


   if( buffer == 13 || buffer == 10 )
   {
      waveAudioOff();
      strcpy_P( (char*)&outputString[readPtr] , PSTR(".wav") );
      readPtr = 0;
      uartNewLine();
      //uartTxString_P( PSTR("Now playing: ") );
      uartTxString(outputString);
      newSongFlag = 1;
      return;
   }

   outputString[readPtr++] = buffer;

}
