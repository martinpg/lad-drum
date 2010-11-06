#include "PetitFS/pff.h"
#include "hardwareSpecific.h"
#include "waveplayer.h"

uint8_t Buff[WAVE_OUTBUFFER_SIZE];		/* Audio output FIFO */
FATFS filesys;



volatile uint16_t audioReadptr = 0;

/* Using Timer2 */
void waveAudioOn(void)
{
   TCCR2 |= ((1 << WGM21) | (1 << CS21) | (1 << CS20));
   TCCR2 &= ~((1 << WGM20) | (1 << COM21) | (1 << COM20));
   TIMSK |= (1 << OCIE2);
}

void waveAudioOff(void)
{
   /* Simply turn off the timer */
   TIMSK &= ~(1 << OCIE2);
   OCR1A = 128;
   OCR1B = 128;
}

/* AudioSetup must be called at the start of a new wave file */
void waveAudioSetup(void)
{
   TCCR1A |= (1 << COM1A0) | (1 << COM1A1) | (1 << WGM10);
   TCCR1B |= (1 << CS10) | (1 << WGM12);

#if WAVE_STEREO_ENABLED
   DDRB |= (1 << 2);
   TCCR1A |= (1 << COM1B0) | (1 << COM1B1);
#endif

   DDRB |= (1 << 1);
   /* Set levels back to centre */
   OCR1A = 128;
   OCR1B = 128;
   audioReadptr = 0;
}


/* This assumes the filesystem has been mounted */
uint8_t wavePlayFile(waveHeader_t* wavefile, uint8_t* filename)
{
   waveAudioSetup();
   waveParseHeader(&wavefile, filename);
   waveAudioOn();


}

/* If this returns false, then the song has finished */
uint8_t waveContinuePlaying(waveHeader_t* wavefile)
{
   uint16_t bytesWritten = 0;
   uint32_t bytesToPlay = 0;

   if( (audioReadptr % WAVE_OUTBLOCK_SIZE) == 0)
   {
      uint16_t outputBlock;

      if( (wavefile->dataSize - 1024) > WAVE_OUTBLOCK_SIZE)
      {
         bytesToPlay = WAVE_OUTBLOCK_SIZE;
      }
      else
      {
         bytesToPlay = wavefile->dataSize;
      }  

      /* Wrap the audio read ptr around */
      outputBlock = audioReadptr;
      if( outputBlock == 0 )
      {
         outputBlock = WAVE_OUTBUFFER_SIZE;
      }

      /* Write to the previous block address that just finished playing */
      outputBlock = outputBlock - WAVE_OUTBLOCK_SIZE;
   
      pf_read(Buff+outputBlock, bytesToPlay, &bytesWritten);
      wavefile->dataSize = wavefile->dataSize - bytesToPlay;

      if( bytesWritten != bytesToPlay )
      {
         uartTxString_P(PSTR("Read Error1!"));
      }
   }

   return (uint8_t)wavefile->dataSize;

}


void waveProcessBuffer(waveHeader_t* wavefile)
{

   /* Here we should determine whether the wave we are playing
    * is a 8/16bit */
   uint8_t byteOffset = 0;
   uint8_t valueOffset = 0;

   /* For Mono / Stereo */
   uint8_t offsetMultiplier = 0;

   /* Setup the resolution byte offsets */
   if( wavefile->resolution == 16 )
   {
      byteOffset = 1;
      valueOffset = 128;
   }

   /* Setup the offsets for stereo */
   if( wavefile->channelCount == 2 )
   {
      offsetMultiplier = 1;
   }

   /* Left is first */
   OCR1A = Buff[(audioReadptr + byteOffset)] + valueOffset;

   /* Right is second */
   /* This will not do anything if WAVE_STEREO_ENABLED is not set to 1 */
   OCR1B = Buff[audioReadptr + (byteOffset << offsetMultiplier) + offsetMultiplier] + valueOffset;

   audioReadptr = audioReadptr + ((1 + byteOffset) << offsetMultiplier);

   if( audioReadptr >= WAVE_OUTBUFFER_SIZE)
   {
      audioReadptr = 0;
   }
}


/* Returns the number of data bytes otherwise returns an error code */
uint8_t waveParseHeader(waveHeader_t* wavefile, uint8_t* filename)
{
    
    uint8_t bytesRead;
    uint32_t chunkSize;

    if(pf_open(filename) != FR_OK) return WAVE_IO_ERROR;
    
        
    /* Check RIFF-WAVE file header */
	if (pf_read(Buff, 12, &bytesRead)) return WAVE_IO_ERROR;

    /* Make sure it is a WAVE file */
	if (bytesRead != 12 || LD_DWORD(Buff+8) != FCC('W','A','V','E')) return WAVE_INVALID_FILE;

   for (;;) 
   {
        /* Get Chunk ID and size */
		if (pf_read(Buff, 8, &bytesRead)) return WAVE_IO_ERROR;		
		if (bytesRead != 8) return WAVE_INVALID_FILE;
		chunkSize = LD_DWORD(Buff+4);		/* Chunk size */
    
        /* Switch by chunk type */
		switch (LD_DWORD(Buff)) 
      {	
		   case FCC('f','m','t',' ') :		/* 'fmt ' chunk */
		      if (chunkSize & 1) chunkSize++;
			   if (chunkSize > 128 || chunkSize < 16) return WAVE_INVALID_FILE;		/* Check chunk size */
			   if (pf_read(Buff, chunkSize, &bytesRead)) return WAVE_IO_ERROR;	/* Get the chunk content */
			   if (bytesRead != chunkSize) return WAVE_INVALID_FILE;
			   if (Buff[FMT_FORMAT] != 1) return WAVE_INVALID_FILE;				/* Check coding type (1: LPCM) */
			   if (Buff[FMT_NUM_CHANNELS] < 1 && Buff[FMT_NUM_CHANNELS] > 2) return WAVE_INVALID_FILE; 			/* Check channels (1/2: Mono/Stereo) */
			   
            wavefile->channelCount = Buff[FMT_NUM_CHANNELS];						/* Save channel flag */
			   if (Buff[FMT_RESOLUTION] != 8 && Buff[FMT_RESOLUTION] != 16) return WAVE_INVALID_FILE;		/* Check resolution (8/16 bit) */
			   wavefile->resolution = Buff[FMT_RESOLUTION];							/* Save resolution flag */
			   wavefile->sampleRate = LD_DWORD(&Buff[FMT_SAMPLERATE]);					/* Check sampling freqency (8k-48k) */
			   if (wavefile->sampleRate < WAVE_MINSAMPLE_RATE || wavefile->sampleRate > WAVE_MAXSAMPLE_RATE) return WAVE_INVALID_FILE;
			   /* Set interval timer (sampling period) */
            /* Use OCR2 */
            OCR2 = (F_CPU/32/wavefile->sampleRate) - 1;		
			   break;

		   case FCC('d','a','t','a') :		/* 'data' chunk (start to play) */
			   if (chunkSize < WAVE_MINIMUM_SAMPLES) return WAVE_INVALID_FILE;	/* Check size */
			   wavefile->dataSize = chunkSize;
            return chunkSize;

		   case FCC('D','I','S','P') :		/* 'DISP' chunk (skip) */
		   case FCC('f','a','c','t') :		/* 'fact' chunk (skip) */
		   case FCC('L','I','S','T') :		/* 'LIST' chunk (skip) */
			   if (chunkSize & 1) chunkSize++;
			   if (pf_lseek(filesys.fptr + chunkSize)) return WAVE_IO_ERROR;
			   break;

		   default:						/* Unknown chunk */
			   return WAVE_INVALID_FILE;
      }
   }
}
