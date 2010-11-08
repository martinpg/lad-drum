 
#include "hardwareSpecific.h"
#include "SD_MMC/sd.h"
#include "PetitFS/pff.h"
#include "SPI/spi.h"
#include "mmculib/uint16toa.h"
#include "waveplayer.h"



volatile waveHeader_t wavefile;
volatile uint8_t newSongFlag = 1;
volatile uint8_t ProcessBufferFlag;

uint8_t outputString[20] = "448s.wav";



int main(void) 
{  
   uint8_t ret;

   uartInit(10,0);
   sei(); 

   SPI_Init();

   DDRC |= (1 << 4);

   /* Initialise SD Card */
   if( SD_Init() == SD_SUCCESS )
   {
      ret = pf_mount(&filesys);
   }

   //uartTxString_P(PSTR("Loop Starting"));


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






ISR(SIG_OUTPUT_COMPARE2, ISR_NOBLOCK)
{
   if( fastMode )
   {
      OCR1A = Buff[(audioReadptr + 1)] + 0x80;
      OCR1B = Buff[(audioReadptr + 3)] + 0x80;
      audioReadptr = (audioReadptr + 4) & WAVE_OUTMASK;
   }
   else
   {
      waveProcessBuffer((waveHeader_t*)&wavefile);
   }
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
