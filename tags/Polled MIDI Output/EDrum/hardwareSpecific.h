#ifndef __HARDWARE_SPECIFIC
#define __HARDWARE_SPECIFIC

/* This file is specific to the MSP430 build of Edrum */

#include <io.h>
#include <signal.h>
#include <mspgcc/flash.h>
#include <mspgcc/util.h>

#include "version.h"
#include "UART/uart.h"

/* MIDI Defines */
#define MIDI_Tx(x)   UART_Tx(x)

/* Channel Select Defines */
#define CHSELOUT  P4OUT
#define CHSELDIR  P4DIR
#define CHSELSEL  P4SEL
#define CHSELIN   P4IN

#define CHSELA     (1<<3)
#define CHSELB     (1<<2)
#define CHSELC     (1<<1)
#define CHSELD     (1<<0)

#define CHSELPINS  ( CHSELA | CHSELB | CHSELC | CHSELD )


/* Digital Input Section */
#define DIGITAL_PIN1	P5IN
#define DIGITAL_PIN2	P4IN

#define DIGITAL_PORT1	P5OUT
#define DIGITAL_PORT2	P4OUT

#define DIGITAL_DDR1		P5DIR
#define DIGITAL_DDR2		P4DIR


/* Keypad Defines */
#if VERSION_CODE == VERSION_WITH_PE	  

/* Key Pad Inputs */
#define UI_COL0   (MAX7300_P28)
#define UI_COL1   (MAX7300_P27)
#define UI_COL2   (MAX7300_P26)
#define UI_COL3   (MAX7300_P25)
#define UI_ROW3   (MAX7300_P24)
#define UI_ROW2   (MAX7300_P23)
#define UI_ROW1   (MAX7300_P22)
#define UI_ROW0   (MAX7300_P21)

#define UI_COL_PORT (0x59)
#define UI_ROW_PORT (0x55)

/* KeyPad Defines */
/* 1st Row */
#define KP_1   (0x77)
#define KP_2   (0xB7)
#define KP_3   (0xD7)
#define KP_A   (0xE7)

/* 2nd Row */
#define KP_4   (0x7B)
#define KP_5   (0xBB)
#define KP_6   (0xDB)
#define KP_B   (0xEB)

/* 3rd Row */
#define KP_7   (0x7D)
#define KP_8   (0xBD)
#define KP_9   (0xDD)
#define KP_C   (0xED)

/* 4th Row */
#define KP_STAR   (0x7E)
#define KP_0      (0xBE)
#define KP_HASH   (0xDE)
#define KP_D      (0xEE)

/*static const uint8_t KP_Button[] = {KP_2, KP_8, KP_5, KP_HASH, 
                                  KP_4, KP_6, KP_A, KP_B, 
                                  KP_C, KP_D, KP_1, KP_3,
                                  KP_7, KP_9, KP_0, KP_STAR};
*/

#endif


#if VERSION_CODE == VERSION_WITHOUT_PE	  
/* Key Pad Inputs */
#define UI_COL0   (UI_INT_COL0)
#define UI_COL1   (UI_INT_COL1)
#define UI_COL2   (UI_INT_COL2)
#define UI_COL3   (UI_INT_COL3)
#define UI_ROW3   (1<<5)
#define UI_ROW2   (1<<4)
#define UI_ROW1   (1<<3)
#define UI_ROW0   (1<<1)

#define UI_ROWS	(UI_ROW0 | UI_ROW1 | UI_ROW2 | UI_ROW3)
#define UI_COLS	(UI_COL0 | UI_COL1 | UI_COL2 | UI_COL3)

#define UI_COL_OUT (P1OUT)
#define UI_ROW_OUT (P2OUT)

#define UI_COL_IN (P1IN)
#define UI_ROW_IN (P2IN)

#define UI_COL_DIR (P1DIR)
#define UI_ROW_DIR (P2DIR)

/* KeyPad Defines */
/* 1st Row */
#define KP_1   (0xEE)
#define KP_2   (0xDE)
#define KP_3   (0xBE)
#define KP_A   (0x7E)

/* 2nd Row */
#define KP_4   (0xED)
#define KP_5   (0xDD)
#define KP_6   (0xBD)
#define KP_B   (0x7D)

/* 3rd Row */
#define KP_7   (0xEB)
#define KP_8   (0xDB)
#define KP_9   (0xBB)
#define KP_C   (0x7B)

/* 4th Row */
#define KP_STAR   (0xE7)
#define KP_0      (0xD7)
#define KP_HASH   (0xB7)
#define KP_D      (0x77)
#endif



/* LCD Defines */
#define LCD_BL_PORT  P1OUT
#define LCD_BL_PIN   2
#define LCD_BL_DDR   P1DIR
#define LCD_BL_SEL   P1SEL


#define MENU_TEXT    char //for the AVR use prog_char if you want strings in ROM
#define PSTR(x)   (x) //for AVR include the pgmspace.h for this macro



#endif
