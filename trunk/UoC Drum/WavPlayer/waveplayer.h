#ifndef _WAVE_PLAYER_H
#define _WAVE_PLAYER_H



/* Four Bytes to a Long */
#define FCC(b1,b2,b3,b4)	(((uint32_t)b4<<24)+((uint32_t)b3<<16)+((uint16_t)b2<<8)+(uint8_t)b1)	/* FourCC */
//#define GET32(x)            (uint32_t)(*(uint32_t*)x)

#define WAVE_IO_ERROR       (2)
#define WAVE_INVALID_FILE   (4)
#define WAVE_MINIMUM_SAMPLES    (1024)
/* 2:I/O error, 4:Invalid file, >=1024:Ok(number of samples) */

#define FMT_FORMAT    (0)
#define FMT_NUM_CHANNELS (2)
#define FMT_SAMPLERATE  (4)
#define FMT_RESOLUTION  (14)

#define WAVE_MINSAMPLE_RATE (8000)
#define WAVE_MAXSAMPLE_RATE (48000)

#define WAVE_OUTBLOCK_SIZE   (256)
#define WAVE_OUTBLOCK_COUNT       (2)
#define WAVE_OUTBUFFER_SIZE   (WAVE_OUTBLOCK_SIZE*WAVE_OUTBLOCK_COUNT)

typedef struct waveHeader
{
   uint8_t channelCount;
   uint8_t resolution;
   uint16_t sampleRate;
   uint32_t dataSize;
} waveHeader_t;




extern FATFS filesys;
extern uint8_t Buff[];
extern volatile uint16_t audioReadptr;

void waveAudioSetup(uint8_t hasRightChannel);
void waveAudioOn(void);
void waveAudioOff(void);

uint8_t waveParseHeader(waveHeader_t* wavefile, uint8_t* filename);
void waveProcessBuffer(waveHeader_t* wavefile);
uint8_t waveContinuePlaying(waveHeader_t* wavefile);

#endif
