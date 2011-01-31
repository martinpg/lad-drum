/* SPI Routines for the ATMEGA Micro
 * Author: Adrian Gin
 * Date: 27/06/07
 *
 */

/** \file spi.h
	 \brief SPI Drivers for the AVR Core.
*/

/**	 

	\ingroup avr_peripheral
 	\defgroup spi SPI Hardware Driver.
 	\code #include "spi.h" \endcode

 	 
	 \par Description
	 SPI Communications protocol for the AVR core.
 	 
*/
//@{
 

#ifndef	_SPI_ROUTINES
#define	_SPI_ROUTINES
 
/* SPI Interface, these should be defined in hardwareSpecific.h */
/*
#define SPI_DDR   (DDRB)
#define SPI_PORT  (PORTB)
#define SCK       (PB5)
#define MISO      (PB4)
#define MOSI      (PB3)
#define nSS       (PB2) */


#define TRANSFER_COMPLETE     (1)
#define TRANSFER_INCOMPLETE   (0)


void SPI_Init(void);

uint8_t SPI_TxByte(uint8_t data);

/** SPI_RxByte, the same as SPI_TxByte(0xFF) */
uint8_t SPI_RxByte(void);




void SPI_RxBlock(uint8_t* data, uint8_t n);

void SPI_TxBlock(uint8_t* data, uint8_t n);

#endif
