/* Modified code from Pascal Stang and Roland Riegel */


#include "hardwareSpecific.h"
#include "sd.h"
#include "SPI/spi.h"
#include <util/delay.h>
#include "mmculib/uint8toa.h"
#include "PetitFS/diskio.h"

#define SD_DEBUG  1

static uint8_t outputString[10];

DSTATUS SD_Stat = STA_NOINIT;
uint8_t SDVersion = 0;

uint8_t SD_WaitUntilReady(void)
{
   uint16_t i = 0;
   uint8_t result;

   for(i = 0; i < SD_TIMEOUT; i++)
   {
      result = SPI_RxByte();
      
      if( result == 0xFF )
      {
         return result;
      }
   }   
   return result;
}



void SD_Startup(void)
{  
   /* Set Clock Phase to sample on the rising edge */
   /* Set to maximum speed */
   SPCR &= ~((1 << CPHA) | (1 << CPOL));
   SPCR |=  ((1 << SPR1) | (1 << SPR0));

}

void SD_SetMaxSpeed(void)
{
   SPCR &= ~((1 << SPR1) | (1 << SPR0));
   SPSR |= (1 << SPI2X);
}

void SD_Shutdown(void)
{
   /* Disable SPI, Bring all SD lines low */
   SPCR &= ~(1 << SPE);
   
   SPI_PORT &= ~( (1<<MOSI) );
   SD_RELEASE();
   
   /* Re-Enable SPI and Revert Clock Phase to sample on the falling edge */
   SPCR |= ((1 << CPHA) | (1<<SPE));     
}

/* returns 0 if success */
uint8_t SD_Init(void)
{
   uint8_t i;
   uint8_t j;
   uint8_t r1;
   
   SD_RELEASE();
   
   SD_Startup();
   
    /* card needs 74 cycles minimum to start up */
   for(i = 0; i < SD_MAX_RETRIES; ++i)
   {
      /* wait 8 clock cycles */
      SPI_RxByte();
   }
   /* Select the card */
   SD_SELECT();


   /* Reset the card */
   for( i = 0; ; i++)
   { 
      r1 = SD_Command(SD_GO_IDLE_STATE, 0);
		if(r1 == SD_R1_IDLE_STATE)
      {
         
         break;
      } 
      if( i > SD_MAX_RETRIES )
      {
         SD_RELEASE();
         return SD_ERROR;
      }
   }

   /* See if it is a SD V2 Card */
   for( i = 0; ; i++)
   { 
#if SD_DEBUG
      uartTxString_P(PSTR("Testing SDV2"));
#endif
      r1 = SD_Command(SD_SEND_IF_COND, 0x01AA);
		if(r1 == SD_R1_IDLE_STATE)
      {
         
         /* Clock out trailing data */
         for( j = 0; j < 4; j++ )
         {
            r1 = SPI_RxByte();
         }

         SDVersion = CT_SD2;
         break;
      }    
      if( i > SD_MAX_RETRIES || r1 & SD_R1_ILLEGAL_COM )      /* This command is not supported */
      {
#if SD_DEBUG
         uartTxString_P(PSTR("Card is SDv1"));
#endif
         SDVersion = CT_SD1;
         break;
      }
   }

   if( SDVersion == CT_SD2 )
   {
      /* Attempt to initiate the High Capacity card's HC bit */
      for( i = 0; ; i++)
      { 
#if SD_DEBUG
         uartTxString_P(PSTR("Testing SDHC"));
#endif
         r1 = SD_Command(SD_APP_CMD,0); //CMD55, must be sent before sending any ACMD command
         r1 = SD_Command(SD_SEND_OP_COND,0x40000000); //ACMD41
   		if(r1 == SD_R1_READY)
         {
            /* If it works, initiate the High Capacity card's HC bit */
               r1 = SD_Command(SD_READ_OCR,0);
               
               if( r1 == SD_R1_READY )
               {
                  r1 = SPI_RxByte();
                  if( r1 & 0x40 )
                  {
#if SD_DEBUG
                     uartTxString_P(PSTR("Card is SDHC"));
#endif
                     SDVersion = CT_SD2 | CT_BLOCK; /* Its version 2, High Capacity */
                  }
                  else
                  {
#if SD_DEBUG
                     uartTxString_P(PSTR("Card is SDv2"));
#endif
                     SDVersion = CT_SD2; /* Its just Version 2, Normal SDC */
                  }
                  /* Clock out the remaining data */
                  SPI_RxByte();
                  SPI_RxByte();
                  SPI_RxByte();
                  break;
               }
         }    
         if( i > SD_MAX_RETRIES )
         {
            return SD_ERROR;
         }
      }
   }
  
   /* Initialise SDv1 and SDs */
   if( SDVersion == CT_SD1 )
   {

      for( i = 0; ; i++)
      { 
         r1 = SD_Command(SD_APP_CMD,0); //CMD55, must be sent before sending any ACMD command
         r1 = SD_Command(SD_SEND_OP_COND,0x00000000); //ACMD41
   		if(r1 == SD_R1_READY)
         {
            break;
         }
            
         if( i > SD_MAX_RETRIES )
         {
            break;
         }
      }
      if( r1 != SD_R1_READY )
      {
         /* try for MMC */
         for( i = 0; ; i++)
         { 
            r1 = SD_Command(MMC_SEND_OP_COND, 0);
      		if(r1 == SD_R1_READY)
            {
               break;
            }
            
            if( i > SD_MAX_RETRIES )
            {
               return SD_ERROR;
            }
         }
      }
   }
   
   SD_SetMaxSpeed();

   r1 = SD_Command(SD_CRC_ON_OFF, 0);
   r1 = SD_Command(SD_SET_BLOCKLEN, 512);   /* Block len is always 512 */  
   
   /* Release and return clock phase and speed back to default */
   SD_RELEASE();
   
   SD_Stat &= ~(STA_NOINIT); /// Set to initialised
   return SD_SUCCESS;
}





/* Note that Clock Phase should be set to zero */ 
uint8_t SD_Command(uint8_t cmd, uint32_t arg)
{
	uint8_t r1;
	uint8_t retry = 0;
   uint8_t crcByte = 0x01;

   /* Select the card */
   SD_RELEASE();
   SD_WaitUntilReady();

   SD_SELECT();
   SD_WaitUntilReady();
	
	// send command
	SPI_TxByte(cmd | 0x40);
	SPI_TxByte(arg>>24);
	SPI_TxByte(arg>>16);
	SPI_TxByte(arg>>8);
	SPI_TxByte(arg);

	//for remaining commands, CRC is ignored in SPI mode
   if(cmd == SD_SEND_IF_COND)	 //it is compulsory to send correct CRC for CMD8 (CRC=0x87) & CMD0 (CRC=0x95)
   {
      crcByte = 0x87;
   }

   if(cmd == SD_GO_IDLE_STATE)	 //it is compulsory to send correct CRC for CMD8 (CRC=0x87) & CMD0 (CRC=0x95)
   {
      crcByte = 0x95;
   }

   SPI_TxByte(crcByte);
   
	// end command
	// wait for response
	// if more than 8 retries, card has timed-out
	// return the received 0xFF
	while( (r1 = SPI_RxByte() ) == 0xFF)
   {
		if(retry++ > SD_MAX_RETRIES)
      {
         break;
      }
   }

   
	// return response
#if SD_DEBUG	
   uartNewLine();
   uartTxString("R1 Response: ");
   uint8toa(r1, outputString);
   uartTxString(outputString);
   uartNewLine();   
#endif

	return r1;
}







#ifdef _FATFS
/* Subroutine which reads 'byteCount' bytes from the SPI and writes it to buffer */
uint8_t SD_ReadBlock(uint8_t* buffer, uint16_t byteCount)
{
	uint16_t i;
	uint8_t retry = 0;	
	/** Wait for start block */
	while( SPI_RxByte() != SD_STARTBLOCK_READ)
	{
		if(retry++ > SD_MAX_RETRIES)
		{
			return SD_ERROR;		
		}
	}
	/* read in data */
	for(i=0; i < byteCount; i++)
	{
		*buffer++ = SPI_RxByte();
	}
	SPI_RxByte(); /// read 16-bit CRC
	SPI_RxByte();
	
	return SD_SUCCESS;	
}



/** Subroutine which writes 512 bytes of buffer to the SD Card*/
/* Token is the data token to send */
uint8_t SD_WriteSector(uint8_t* buffer, uint8_t token)
{
	uint16_t i;

	/* Send Dummy Byte */
	SPI_RxByte();

	SPI_TxByte(token);

	/* Write data */
	for(i=0; i<0x200; i++)
	{
		SPI_TxByte(*buffer++);
	}
	SPI_RxByte(); /// read 16-bit CRC
	SPI_RxByte();
	
	uint8_t r1 = SPI_RxByte();
	// read data response token
	if( (r1 & SD_DR_MASK) != SD_DR_ACCEPT)
	{
#if SD_DEBUG		
   	uartNewLine();
   	uartTxString("R1 Response: ");
   	uint8toa(r1, outputString);
   	uartTxString(outputString);
   	uartNewLine();  
#endif   	
		return SD_ERROR;	
	}
		
	
	
	return SD_SUCCESS;	
}


#endif



#ifdef _FATFS
/* START OF FAT FS, TINY FAT FS etc from ChaN 
 *
 *
 *
 *
 *
 */
uint8_t SD_disk_Init(void)
{
	if( SD_Init() == SD_SUCCESS)
	{
		return RES_OK;	
	}	
	else
	{
		return RES_ERROR;	
	}
}


/* Used by SD_DISKREAD */
/** Writes the buffer to the sector. secCount is the number of sectors to write */	
uint8_t SD_Read(uint8_t* buffer, uint32_t sector, uint8_t secCount )
{
	// assert chip select
   SD_CS_PORT &= ~(1 << SD_CS_PIN);
   
   
   sector = sector << 9;
   
	if (secCount == 1) {									// Single block read 
		if ((SD_Command(SD_READ_SINGLE_BLOCK, sector) == 0)		// READ_SINGLE_BLOCK 
			&& SD_ReadBlock(buffer, 512))
			secCount = 0;
	}
	else {	// Multiple block read
		if (SD_Command(SD_READ_MULTIPLE_BLOCK, sector) == 0) 
		{	// READ_MULTIPLE_BLOCK 
			do 
			{
				if (!SD_ReadBlock(buffer, 512)) break;
				buffer += 512;
			} while (--secCount);
			SD_Command(SD_STOP_READ_TRANS, 0);				// STOP_TRANSMISSION 
		}
	}
  
	// wait until card not busy
	while(!SPI_RxByte());	

   /* Release and return clock phase and speed back to default */
   SD_CS_PORT |= (1 << SD_CS_PIN);
  
	// return success
	return secCount ? RES_ERROR : RES_OK;
}








/* Writes 'secCount' sectors of buffer to the address sector */
uint8_t SD_Write(const uint8_t* buffer, uint32_t sector, uint8_t secCount)
{   
	// assert chip select
   SD_CS_PORT &= ~(1 << SD_CS_PIN);
   
   sector = sector << 9;
   
	if (secCount == 1) {									// Single block write 
		if ((SD_Command(SD_WRITE_BLOCK, sector) == 0)
			&& SD_WriteSector( (uint8_t*) buffer, SD_STARTBLOCK_WRITE))
			secCount = 0;
	}
	else {	// Multiple block Write
		if (SD_Command(SD_WRITE_MULTIPLE_BLOCK, sector) == 0) 
		{ 
			do 
			{
				if (!SD_WriteSector( (uint8_t*)buffer, SD_STARTBLOCK_MWRITE)) break;
				buffer += 512;
			} while (--secCount);
			SPI_TxByte(SD_STOPTRAN_WRITE);				// STOP_TRANSMISSION 
		}
	}
  
	// wait until card not busy
	while(!SPI_RxByte());	

   /* Release and return clock phase and speed back to default */
   SD_CS_PORT |= (1 << SD_CS_PIN);
  
	// return success
	return secCount ? RES_ERROR : RES_OK;
}





/** Disk IO Functions */
DSTATUS SD_disk_status(void)
{
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
/** Ctrl is the control command to send */
/** buff is the memory to read or write the control data to */
DRESULT SD_disk_ioctl(	uint8_t ctrl,	void *buff )
{
	DRESULT res;
	uint8_t n, csd[16], *ptr = buff;
	uint16_t csize;

	res = RES_ERROR;


	if (SD_Stat & STA_NOINIT) return RES_NOTRDY;

   /* Select the card */
   SD_CS_PORT &= ~(1 << SD_CS_PIN);

	switch (ctrl) 
	{
		case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
			if ((SD_Command(SD_SEND_CSD, 0) == 0) && SD_ReadBlock(csd, 16)) {
				if ((csd[0] >> 6) == 1) {	/* SDC ver 2.00 */
					csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
					*(uint32_t*)buff = (uint32_t)csize << 10;
				} else {					/* SD or SDC ver 1.XX */
					n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
					csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
					*(uint32_t*)buff = (uint16_t)csize << (n - 9);
				}
				res = RES_OK;
			}
			break;
	
		case GET_SECTOR_SIZE :	/* Get sectors on the disk (WORD) */
			*(uint16_t*)buff = 512;
			res = RES_OK;
			break;
	
		case CTRL_SYNC :	/* Make sure that data has been written */
				while(!SPI_RxByte());
				res = RES_OK;
			break;
	
		case SD_GET_CSD :	/* Receive CSD as a data block (16 uint8_ts) */
			if (SD_Command(SD_SEND_CSD, 0) == 0		/* READ_CSD */
				&& SD_ReadBlock(ptr, 16))
				res = RES_OK;
			break;
	
		case SD_GET_CID :	/* Receive CID as a data block (16 uint8_ts) */
			if (SD_Command(SD_SEND_CID, 0) == 0		/* READ_CID */
				&& SD_ReadBlock(ptr, 16))
				res = RES_OK;
			break;
	
		case SD_GET_OCR :	/* Receive OCR as an R3 resp (4 uint8_ts) */
			if (SD_Command(SD_READ_OCR, 0) == 0) {	/* READ_OCR */
				for (n = 0; n < 4; n++)
					*ptr++ = SPI_RxByte();
				res = RES_OK;
			}
	
		case SD_GET_TYPE :	/* Get card type flags (1 uint8_t) */
			*ptr = 0;
			res = RES_OK;
			break;
	
		default:
			res = RES_PARERR;
	}

   /* Deselect the card */
   SD_CS_PORT |= (1 << SD_CS_PIN);
	SPI_RxByte();			/* Idle (Release DO) */


	return res;
}

#endif



/* END OF FAT FS */




#ifdef _PETITFATFS
/* START OF PETIT FAT FS
 *
 *
 * wrapper functions 
 */

DSTATUS disk_initialize (void)
{
   
   if (SD_Init() == SD_SUCCESS )
   {
      return RES_OK;
   }
   else
   {
      return RES_ERROR;
   }


}
#if _USE_READ
/*-----------------------------------------------------------------------*/
/* Read partial sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	BYTE *buffer,		/* Pointer to the read buffer (NULL:Read bytes are forwarded to the stream) */
	DWORD sector,		/* Sector number (LBA) */
	WORD offset,		/* Byte offset to read from (0..511) */
	WORD byteCount		/* Number of bytes to read (offset + byteCount mus be <= 512) */
)
{
		
   uint16_t bytesRemaining = 514;
   uint8_t ret = RES_ERROR;
   
   /* Convert sectors to bytes, if it is NOT an SDHC card */
   if( !(SDVersion & CT_BLOCK) )
   {
      sector = sector << 9;
   }
   
   // Single block read 
	if (SD_Command(SD_READ_SINGLE_BLOCK, sector) == 0)		// READ_SINGLE_BLOCK
   {
   	uint8_t retry = SD_MAX_RETRIES;	
      bytesRemaining = bytesRemaining - offset - byteCount;
   	/** Wait for start block */
   	while( SPI_RxByte() != SD_STARTBLOCK_READ && retry)
   	{
   		if(retry--)
   		{
   			ret = RES_ERROR;		
   		}
   	}
      if( retry )
      {
      	/* read in data */
      	while( offset-- )
         {
      		SPI_RxByte();
      	}

         while( byteCount-- )
         {
      		*buffer++ = SPI_RxByte();
      	}
         
         /* Skip CRCs, and remaining bytes */
      	while( bytesRemaining-- )
         {
      		SPI_RxByte();
      	}
      	ret = RES_OK;
      }
	}

   // wait until card not busy
	while(!SPI_RxByte());
   
   /* Release and return clock phase and speed back to default */
   SD_RELEASE();
 

	// return success
	return ret;


}
#endif
#if _USE_WRITE
DRESULT disk_writep (
	const BYTE *buffer,	/* Pointer to the bytes to be written (NULL:Initiate/Finalize sector write) */
	DWORD byteCount			/* Number of bytes to send, or Sector number (LBA) if buffer is zero */
)
{
   uint8_t ret = RES_ERROR;
	static WORD WriteCounter;
   DWORD sectorNumber;

	if (buffer) 
   {		/* Send data bytes */
		while (byteCount--) 
      {		/* Send data bytes to the card */
			SPI_TxByte(*buffer++);
         WriteCounter--;
		}
		return RES_OK;
	} 
   else 
   {
		if (byteCount) 
      {
         sectorNumber = byteCount;
      	/* Initiate sector write process */
         /* Convert sectors to bytes, if it is NOT an SDHC card */
         if( !(SDVersion & CT_BLOCK) )
         {
            sectorNumber = sectorNumber << 9;
         }
         
         /* WRITE_SINGLE_BLOCK */
			if (SD_Command(SD_WRITE_BLOCK, sectorNumber) == 0) 
         {		
         	/* I don't like this infinite loop */
				SD_WaitUntilReady(); 
            SPI_TxByte(SD_STARTBLOCK_WRITE);		/* Data block header */
				WriteCounter = 512;							/* Set byte counter */
				ret = RES_OK;
			}
		} 
      else 
      {	/* Finalize sector write process */
			byteCount = WriteCounter + 2;
			while (byteCount--) 
         {
            SPI_TxByte(0);	/* Fill left bytes and CRC with zeros */
			}     
         if( (SPI_RxByte() & 0x1F) == SD_DR_ACCEPT) 
         {	/* Receive data resp and wait for end of write process in timeout of 300ms */
            SD_WaitUntilReady();
            ret = RES_OK;
			}

         /* Release SPI */
         SD_RELEASE();
	   }
   }

   return ret;
}
#endif


#endif
