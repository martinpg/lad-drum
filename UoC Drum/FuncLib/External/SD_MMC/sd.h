/*! \file SD.h \brief MultiMedia and SD Flash Card Interface. */
//*****************************************************************************
//
// File Name	: 'SD.h'
// Title		: MultiMedia and SD Flash Card Interface
// Author		: Pascal Stang - Copyright (C) 2004 edited by Adrian Gin
// Created		: 2004.09.22
// Revised		: 2004.09.22
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
///	\ingroup avr_external
/// \defgroup SD MultiMedia and SD Flash Card Interface (SD.c)
/// \code #include "SD.h" \endcode
/// \par Description
///		This library offers some simple functions which can be used
///		to read and write data on a MultiMedia or SecureDigital (SD) Flash
///		Card.  Although MM and SD Cards are designed to operate with their own
///		special bus wiring and protocols, both types of cards also provide a
///		simple SPI-like interface mode which is exceptionally useful when 
///		attempting to use the cards in embedded systems.
///
/// \par Wiring
///		To work with this library, the card must be wired to the SPI port of
///		the Atmel microcontroller as described below.  Typical cards can
///		operate at up to 25MHz maximum SPI clock rate (thus faster than most
///		AVR's maximum SPI clock rate).
/// <pre>
///	      _________________
///	     /  1 2 3 4 5 6 78 |	<- view of SD/SD card looking at contacts
///	    / 9                |	Pins 8 and 9 are present only on SD cards
///	    |    SD/SD Card   |
///	    |                  |
///	    
///	    1 - CS   (chip select)          - wire to any available I/O pin(*)
///	    2 - DIN  (data in, card<-host)  - wire to SPI MOSI pin
///	    3 - VSS  (ground)               - wire to ground
///	    4 - VDD  (power, 3.3V only?)    - wire to power (MIGHT BE 3.3V ONLY!)
///	    5 - SCLK (data clock)           - wire to SPI SCK pin
///	    6 - VSS  (ground)               - wire to ground
///	    7 - DOUT (data out, card->host) - wire to SPI MISO pin
///
///	    (*) you must define this chip select I/O pin in SDconf.h
/// </pre>
/// \note This code is currently below version 1.0, and therefore is considered
/// to be lacking in some functionality or documentation, or may not be fully
/// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
//@{

#ifndef SD_H
#define SD_H

#include "PetitFS/diskio.h"

#define SD_ERROR 		0
#define SD_SUCCESS 	1

/* These should reside in hardwareSpecific.h */
/*
#define SD_CS_DDR   DDRD
#define SD_CS_PORT  PORTD
#define SD_CS_PIN   PD4

#define SD_PWR_DDR  DDRB
#define SD_PWR_PORT PORTB
#define SD_PWR_PIN  PB0 */

#define SD_MAX_RETRIES 100
#define SD_TIMEOUT   (65000)

#define SD_SELECT()    SD_CS_PORT &= ~(1 << SD_CS_PIN); 
#define SD_RELEASE()   SD_CS_PORT |= (1 << SD_CS_PIN);


// constants/macros/typdefs
// SD commands (taken from sandisk SD reference)
#define SD_GO_IDLE_STATE			0		///< initialize card to SPI-type access
#define MMC_SEND_OP_COND			1		///< set card operational mode replaced with ACMD 41 (SDS dont use ACMD41)
#define SD_SEND_IF_COND      8     // Required for SDHC cards
#define SD_SEND_CSD				9		///< get card's CSD
#define SD_SEND_CID				10		///< get card's CID
#define SD_STOP_READ_TRANS		12
#define SD_SEND_STATUS				13
#define SD_SET_BLOCKLEN			16		///< Set number of bytes to transfer per block
#define SD_READ_SINGLE_BLOCK		17		///< read a block
#define SD_READ_MULTIPLE_BLOCK	18
#define SD_WRITE_BLOCK				24		///< write a block
#define SD_WRITE_MULTIPLE_BLOCK	25
#define SD_PROGRAM_CSD				27
#define SD_SET_WRITE_PROT			28
#define SD_CLR_WRITE_PROT			29
#define SD_SEND_WRITE_PROT			30
#define SD_TAG_SECTOR_START		32
#define SD_TAG_SECTOR_END			33
#define SD_UNTAG_SECTOR			34
#define SD_TAG_ERASE_GROUP_START 	35		///< Sets beginning of erase group (mass erase)
#define SD_TAG_ERARE_GROUP_END		36		///< Sets end of erase group (mass erase)
#define SD_UNTAG_ERASE_GROUP		37		///< Untag (unset) erase group (mass erase)
#define SD_ERASE					38		///< Perform block/mass erase
#define SD_SEND_OP_COND			 41   //ACMD
#define SD_APP_CMD              55
#define SD_READ_OCR					58
#define SD_CRC_ON_OFF				59		///< Turns CRC check on/off
// R1 Response bit-defines
#define SD_R1_BUSY					0x80	///< R1 response: bit indicates card is busy
#define SD_R1_PARAMETER			0x40
#define SD_R1_ADDRESS				0x20
#define SD_R1_ERASE_SEQ			0x10
#define SD_R1_COM_CRC				0x08
#define SD_R1_ILLEGAL_COM			0x04
#define SD_R1_ERASE_RESET			0x02
#define SD_R1_IDLE_STATE			0x01
#define SD_R1_READY             0x00
// Data Start tokens
#define SD_STARTBLOCK_READ		0xFE	///< when received from card, indicates that a block of data will follow
#define SD_STARTBLOCK_WRITE		0xFE	///< when sent to card, indicates that a block of data will follow
#define SD_STARTBLOCK_MWRITE		0xFC
// Data Stop tokens
#define SD_STOPTRAN_WRITE			0xFD
// Data Error Token values
#define SD_DE_MASK					0x1F
#define SD_DE_ERROR				0x01
#define SD_DE_CC_ERROR				0x02
#define SD_DE_ECC_FAIL				0x04
#define SD_DE_OUT_OF_RANGE			0x04
#define SD_DE_CARD_LOCKED			0x04
// Data Response Token values
#define SD_DR_MASK					0x1F
#define SD_DR_ACCEPT				0x05
#define SD_DR_REJECT_CRC			0x0B
#define SD_DR_REJECT_WRITE_ERROR	0x0D

/* Card type flags (CardType) */
#define CT_SD				0x01	/* SD ver 3 */
#define CT_SD1				0x02	/* SD ver 1 */
#define CT_SD2				0x04	/* SD ver 2 */
#define CT_SDC				(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK			0x08	/* Block addressing */


void SD_SetMaxSpeed(void);
uint8_t SD_WaitUntilReady(void);

uint8_t SD_Init(void);
void SD_Startup(void);
void SD_Shutdown(void);

uint8_t SD_disk_Init(void);
uint8_t SD_ReadBlock(uint8_t* buffer, uint16_t byteCount);
uint8_t SD_WriteSector(uint8_t* buffer, uint8_t token);

uint8_t SD_Command(uint8_t cmd, uint32_t arg);

/** SD DISK IO Functions */
#ifdef _FATFS
uint8_t SD_Write(const uint8_t* buffer, uint32_t sector, uint8_t secCount );
uint8_t SD_Read(uint8_t* buffer, uint32_t sector, uint8_t secCount );
DSTATUS SD_disk_status(void);
DRESULT SD_disk_ioctl(uint8_t ctrl,	void *buff);
#endif


#ifdef _PETITFATFS
/* Petit Disk IO Functions, some functions are not implemented, the write is also slightly different */
DSTATUS disk_initialize (void);
DRESULT disk_readp (BYTE*, DWORD, WORD, WORD);
DRESULT disk_writep (const BYTE*, DWORD);
#endif

#endif
