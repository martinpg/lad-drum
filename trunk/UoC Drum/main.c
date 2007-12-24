/*
   MSP430F16x Electronic Drum Kit
   
*/
#include <io.h>
#include <stdint.h>

#include "UART/uart.h"

/**
Delay function.
*/
void delay(unsigned int d) {
   int i;
   for (i = 0; i<d; i++) {
      nop();
      nop();
   }
}


/**
Main function with some blinking leds
*/
int main(void) {
    int i;
    int o = 0;

    WDTCTL = WDTHOLD;               //Init watchdog timer

    P1OUT  = 0;                //Init output data of port1
    P2OUT  = 0;                //Init output data of port2

    P1SEL  = 0;                //Select port or module -function on port1
    P2SEL  = 0;                //Select port or module -function on port2

    P1DIR  = 0;                //Init port direction register of port1
    P2DIR  = 0;                //Init port direction register of port2

    P1IES  = 0;                //init port interrupts
    P2IES  = 0;
    P1IE   = 0;
    P2IE   = 0;

    while (1) {                         //main loop, never ends...
        for (i=0; i<8; i++, o++) {
            P1OUT = (1<<i) | (0x80>>(o&7));
            delay(0x4fff);
        }
    }
}

