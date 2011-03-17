/* File Name	: 'max7300.h'
 	Title			: MAXIM-IC 7300 Port Expander Interface.
 	Author		: Adrian Gin


*/

/** \file max7300.h
	 \brief MAX7300 Port Expander Interface.
*/

/**	 

	\ingroup avr_external
 	\defgroup max7300 MAX7300 Port Expander Interface.
 	\code #include "max7300.h" \endcode

 	 
	 \par Description
	This library provides functions which allow a MAX7300 to be used
	over I2C. Read and write capabilities are available.

	Consequently, I2C must be initialised before using this device driver.
 	 
 	 
*/
//@{

#ifndef	_MAX7300_ROUTINES
#define	_MAX7300_ROUTINES

#include <stdint.h>


#define MAX7300_CONFIG    (0x04)
#define MAX7300_SHUTDOWN_CONTROL  (0)
#define MAX7300_TRANSITION_ENABLE (7)

#define MAX7300_TRANSITION_MASK   (0x06)


/** Transition Mask Bits */
#define MAX7300_TM_P31  (7)
#define MAX7300_TM_P30  (6)
#define MAX7300_TM_P29  (5)
#define MAX7300_TM_P28  (4)
#define MAX7300_TM_P27  (3)
#define MAX7300_TM_P26  (2)
#define MAX7300_TM_P25  (1)
#define MAX7300_TM_P24  (0)

/**  Data direction Registers */

#define MAX7300_DDRA1 (0x0B) /** Ports P12 - P15 */


#define MAX7300_DDRA2 (0x0C) /** Ports P16 - P19 */


#define MAX7300_DDRB1 (0x0D) /** Ports P20 - P23 */

/** Individual Port Register Addresses */
#define MAX7300_P12   (0x2C)
#define MAX7300_P13   (0x2D)
#define MAX7300_P14   (0x2E)
#define MAX7300_P15   (0x2F)
#define MAX7300_P16   (0x30)
#define MAX7300_P17   (0x31)
#define MAX7300_P18   (0x32)
#define MAX7300_P19   (0x33)
#define MAX7300_P20   (0x34)
#define MAX7300_P21   (0x35)
#define MAX7300_P22   (0x36)
#define MAX7300_P23   (0x37)
#define MAX7300_P24   (0x38)
#define MAX7300_P25   (0x39)
#define MAX7300_P26   (0x3A)
#define MAX7300_P27   (0x3B)
#define MAX7300_P28   (0x3C)
#define MAX7300_P29   (0x3D)
#define MAX7300_P30   (0x3E)
#define MAX7300_P31   (0x3F)


#define MAX7300_DDRB2 (0x0E) /** Ports P24 - P27 */




#define MAX7300_DDRC1 (0x0F) /* Ports P28 - P31 */




#define MAX7300_PORTA  (0x4C) /* Ports P12 - P19 */


#define MAX7300_PORTB  (0x54) /* Ports P20 - P27 */


#define MAX7300_PORTC  (0x5C) /* Ports P28 - P31 */


#define MAX7300_PORTINT (0x58) /* Ports P24 - P31 */


uint8_t MAX7300_ReadRegister(uint8_t address, uint8_t reg);
void MAX7300_SetRegister(uint8_t address, uint8_t reg, uint8_t data);
void MAX7300_SetBlock_C(uint8_t address, const uint8_t* data, uint8_t bytes);



#endif


