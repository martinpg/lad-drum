#ifndef __HARDWARE_SPECIFIC
#define __HARDWARE_SPECIFIC

/* This file is specific to the AVR build of Edrum */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "version.h"
#include "hardUART/hardUart.h"


/* Interrupts */
//#define ENABLE_INTERRUPT() (sei())
//#define DISABLE_INTERRUPT() (cli())

/* MIDI Defines */
#define MIDI_Tx(x)   uartTx(x)


/* Uart Defines */
#define UART_Tx   uartTx
#define UART_TxDump   uartTxDump
#define UART_TxString_P uartTxString_P
#define UART_TxString uartTxString

/* Channel Select Defines */
#define CHSELOUT  PORTB
#define CHSELDIR  DDRB
#define CHSELSEL  P4SEL
#define CHSELIN   PINB

#define CHSELA     (1<<0)
#define CHSELB     (1<<1)
#define CHSELC     (1<<2)
#define CHSELD     (1<<3)

#define CHSELPINS  ( CHSELA | CHSELB | CHSELC | CHSELD )


/* Sensor Defines */
#define SENSOR_OUTPUT	(0)

/* Variable Output */
#define SENSOR_OUTPUT2	(0)
#define POT_INPUT			(1)


/* Digital Input Section */
#define DIGITAL_PIN1	(PINA)
#define DIGITAL_PIN2	(PINC)

#define DIGITAL_PORT1	(PORTA)
#define DIGITAL_PORT2	(PORTC)

#define DIGITAL_DDR1		(DDRA)
#define DIGITAL_DDR2		(DDRC)


/* Keypad Defines */
/* Key Pad Inputs */
#define UI_COL0   (1<<4)
#define UI_COL1   (1<<5)
#define UI_COL2   (1<<6)
#define UI_COL3   (1<<7)
#define UI_ROW3   (1<<8)
#define UI_ROW2   (1<<7)
#define UI_ROW1   (1<<6)
#define UI_ROW0   (1<<5)

#define UI_ROWS	(UI_ROW0 | UI_ROW1 | UI_ROW2 | UI_ROW3)
#define UI_COLS	(UI_COL0 | UI_COL1 | UI_COL2 | UI_COL3)

#define UI_COL_OUT (PORTB)
#define UI_ROW_OUT (PORTD)

#define UI_COL_IN (PINB)
#define UI_ROW_IN (PIND)

#define UI_COL_DIR (DDRB)
#define UI_ROW_DIR (DDRD)

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


/* LCD Defines */
#define LCD_BL_PORT  (PORTD)
#define LCD_BL_PIN   (1)
#define LCD_BL_DDR   (DDRD)
#define LCD_BL_SEL   (DDRD)


/* Not used in Direct Mode */
#define UI_LCD_PORT     (0x00)

/* LCD Inputs */
#define UI_LCD_PRS      (7)
#define UI_LCD_RS       (1<<5)
#define UI_LCD_E        (1<<4)
#define UI_LCD_D4       (1<<0)
#define UI_LCD_D5       (1<<1)
#define UI_LCD_D6       (1<<2)
#define UI_LCD_D7       (1<<3)    

#define UI_LCD_CONTROL_DIR   (DDRC)
#define UI_LCD_DATA_DIR      (DDRC)

#define UI_LCD_DATA	(UI_LCD_D4 | UI_LCD_D5 | UI_LCD_D6 | UI_LCD_D7)
#define UI_LCD_CONTROL	(UI_LCD_RS | UI_LCD_E)

#define UI_LCD_CONTROL_PORT	(PORTC)
#define UI_LCD_DATA_PORT		(PORTC)

/* END OF VERSION WITHOUT PE Defines */

#endif
