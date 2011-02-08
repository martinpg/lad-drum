#ifndef __HARDWARE_SPECIFIC
#define __HARDWARE_SPECIFIC

/* This file is specific to the AVR build of Edrum */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "version.h"
#include "hardUART/hardUart.h"
#include "avrADC/adc.h"


/* Interrupts */
//#define ENABLE_INTERRUPT() (sei())
//#define DISABLE_INTERRUPT() (cli())
#define PROGRAM_SPACE PROGMEM
#define PROGRAM_CHAR  prog_char
#define PROGRAM_PTR   PGM_P

/* MIDI Defines */
#define MIDI_Tx(x)   uartTx(x)
#define MIDI_SetBaudRate(high,low) uartSetBaud(high,low)
#define MIDI_BAUD_31250      (F_CPU / 31250)
#define MIDI_BAUD_38400      (F_CPU / 38400)
#define MIDI_BAUD_115200		(F_CPU / 115200)
#define MIDI_BAUD_1M			(F_CPU / 1000000)

/* SPI Defines */
#define SPI_DDR   (DDRB)
#define SPI_PORT  (PORTB)
#define SCK       (PB7)
#define MISO      (PB6)
#define MOSI      (PB5)
#define nSS       (PB4)


/* Uart Defines */
#define UART_Init(x) uartInit(x)
#define UART_Tx   uartTx
#define UART_TxDump   uartTxDump
#define UART_TxString_P uartTxString_P
#define UART_TxString uartTxString

/* Channel Select Defines */
#define CHSELOUT  PORTB
#define CHSELDIR  DDRB
#define CHSELSEL  P4SEL
#define CHSELIN   PINB

#define CHSELA     (0)
#define CHSELB     (1)
#define CHSELC     (2)
#define CHSELD     (3)

#define CHSELPINS  ( (1 << CHSELA) | (1 << CHSELB) | (1 << CHSELC) | (1 << CHSELD) )
#define CHANNEL_COUNT (15)

/* Sensor Defines */
#define SENSOR_OUTPUT	(0)

/* Variable Output */
#define SENSOR_OUTPUT2	(7)
#define POT_INPUT			(6)


/* Digital Input Section */
#define DIGITAL_PIN1	(PINA)
#define DIGITAL_PIN2	(PINC)

#define DIGITAL_PORT1	(PORTA)
#define DIGITAL_PORT2	(PORTC)

#define DIGITAL_DDR1		(DDRA)
#define DIGITAL_DDR2		(DDRC)

/* ADC Defines */
#define ADC_Init()       adcInitialise()     
#define ADC_Sample()     adc10()
#define ADC_SetupAddress(channel) adcSetPin(channel)


#define ADC_REF_DEFAULT ADC_REF_AVCC
#define ADC_PRESCALE_DEFAULT ADC_PRESCALE_DIV128

/* Keypad Defines */
/* Key Pad Inputs */
#define UI_COL0   (1<<0)
#define UI_COL1   (1<<2)
#define UI_COL2   (1<<1)
#define UI_COL3   (1<<0)
#define UI_ROW3   (1<<6)
#define UI_ROW2   (1<<5)
#define UI_ROW1   (1<<4)
#define UI_ROW0   (1<<3)

#define UI_ROWS	(UI_ROW0 | UI_ROW1 | UI_ROW2 | UI_ROW3)
#define UI_COLS	(UI_COL1 | UI_COL2 | UI_COL3)

#define UI_COL_OUT (PORTA)
#define UI_ROW_OUT (PORTC)

#define UI_COL_IN (PINA)
#define UI_ROW_IN (PINC)

#define UI_COL_DIR (DDRA)
#define UI_ROW_DIR (DDRC)

/* KeyPad Defines */
/* 1st Row */
#define KP_1   (0x18)
#define KP_2   (0x14)
#define KP_3   (0x12)
#define KP_A   (0x11)

/* 2nd Row */
#define KP_4   (0x28)
#define KP_5   (0x24)
#define KP_6   (0x22)
#define KP_B   (0x21)

/* 3rd Row */
#define KP_7   (0x48)
#define KP_8   (0x44)
#define KP_9   (0x42)
#define KP_C   (0x41)

/* 4th Row */
#define KP_STAR   (0x88)
#define KP_0      (0x84)
#define KP_HASH   (0x82)
#define KP_D      (0x81)


/* LCD Defines */
#define LCD_BL_PORT  (PORTD)
#define LCD_BL_PIN   (7)
#define LCD_BL_DDR   (DDRD)
#define LCD_BL_SEL   (DDRD)
#define UI_LCD_GET_FLASHBYTE(x) pgm_read_byte(x)

#define MENU_SPACE PROGRAM_SPACE
#define MENU_GET_PTR(x)  pgm_read_word(&x)
#define MENU_GET_BYTE(x)  pgm_read_byte(&x)
#define MENU_TEXT    prog_char //for the AVR use prog_char if you want strings in ROM
#define MENU_SPACE_PTR	PGM_P
//#define PSTR(x)   (x) //for AVR include the pgmspace.h for this macro


/* Not used in Direct Mode */
#define UI_LCD_PORT     (0x00)

/* LCD Inputs */
#define UI_LCD_RS_PIN      (5)
#define UI_LCD_RS       (1<<5)
#define UI_LCD_E        (1<<7)
#define UI_LCD_D4       (1<<0)
#define UI_LCD_D5       (1<<1)
#define UI_LCD_D6       (1<<2)
#define UI_LCD_D7       (1<<3)    

/* Use 8bit mode via 74HC164 */
#define UI_LCD_8BITMODE
#define UI_LCD_SERIAL_DATA (UI_LCD_RS)

#define UI_LCD_CONTROL_DIR   (DDRC)
#define UI_LCD_DATA_DIR      (DDRC)

#define UI_LCD_DATA	(UI_LCD_D4 | UI_LCD_D5 | UI_LCD_D6 | UI_LCD_D7)
#define UI_LCD_CONTROL	(UI_LCD_RS | UI_LCD_E)

#define UI_LCD_CONTROL_PORT	(PORTC)
#define UI_LCD_DATA_PORT		(PORTC)


#define LCD_FUNCTION_DEFAULT	((1<<LCD_FUNCTION) | (1<<LCD_FUNCTION_2LINES) | (1 << LCD_FUNCTION_8BIT))
#define LCD_MODE_DEFAULT		((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC))
#define LCD_DISPLAY_DEFAULT   ((1<<LCD_DISPLAY) | (1<<LCD_ON_DISPLAY))

/* END OF VERSION WITHOUT PE Defines */

#endif
