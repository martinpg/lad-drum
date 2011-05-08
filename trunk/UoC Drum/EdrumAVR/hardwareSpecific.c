/*

Copyright (c) 2011 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "hardwareSpecific.h"


volatile AVR_USART_t PrimaryUART = {
   &UCSRA,
   &UCSRB,
   &UCSRC,
   &UBRRH,
   &UBRRL,
   &UDR,

   &ReceiveBuffer,
   &TransmitBuffer
};


typedef struct
{
   volatile uint8_t* DDR; 
   volatile uint8_t* PORT;
   volatile uint8_t* PIN;
   uint8_t pinMask;
} DigitalInput;


DigitalInput DIGITAL_INPUT_MAP[] =
{
   {&DIGITAL0_DDR, &DIGITAL0_PORT, &DIGITAL0_PIN, DIGITAL0},
   {&DIGITAL1_DDR, &DIGITAL1_PORT, &DIGITAL1_PIN, DIGITAL1},
   {&DIGITAL2_DDR, &DIGITAL2_PORT, &DIGITAL2_PIN, DIGITAL2},
   {&DIGITAL3_DDR, &DIGITAL3_PORT, &DIGITAL3_PIN, DIGITAL3},
   {&DIGITAL4_DDR, &DIGITAL4_PORT, &DIGITAL4_PIN, DIGITAL4},
   {&DIGITAL5_DDR, &DIGITAL5_PORT, &DIGITAL5_PIN, DIGITAL5},
   {&DIGITAL6_DDR, &DIGITAL6_PORT, &DIGITAL6_PIN, DIGITAL6},
   {&DIGITAL7_DDR, &DIGITAL7_PORT, &DIGITAL7_PIN, DIGITAL7}
};



/* This is pretty hardware Specific */
void DigitalInputInit(void)
{
	/* Turn all associated pins to inputs & enable pull ups*/
   uint8_t i;
   for(i = 0; i < DIGITAL_INPUTS; i++)
   {
      *DIGITAL_INPUT_MAP[i].DDR &= ~(DIGITAL_INPUT_MAP[i].pinMask);
      *DIGITAL_INPUT_MAP[i].PORT |= (DIGITAL_INPUT_MAP[i].pinMask);
   }
}


/* Returns the state of the passed digital input channel */
uint8_t getDigitalState(uint8_t DigitalChannel)
{
   if( DigitalChannel < DIGITAL_INPUTS)
	{
      return (*DIGITAL_INPUT_MAP[DigitalChannel].PIN) & (DIGITAL_INPUT_MAP[DigitalChannel].pinMask) ? 1 : 0;
   }
   else
   {
      return 0;
   }
}



void SoftTimer_TimerInit(void)
{
   /* Primary Timer prescaler (high priority) */
   TCCR1B |= (0x03);
   TIMSK |= (1 << OCIE1A);

   /* Secondary Timer (non critical) */
   TCCR0 |= (0x05);
   TIMSK |= (1 << OCIE0);
}


void enableFlashProgramming(uint8_t mode)
{
   uint8_t interuptControlState;
   interuptControlState = GICR;
   /* IVSEL = 1, bootloader interrupts */
   GICR = (1 << IVCE);
   GICR = (mode << IVSEL);
   GICR = interuptControlState;
}

void SensorChannel(uint8_t channel)
{
	
	uint8_t channelState;
	
   if( channel >= CHANNEL_COUNT )
   {
      return;    
   }
   
   channel = ~channel;
   
   channelState = (((channel & (0x08)) ? 1 : 0) << CHSELD) |
                  (((channel & (0x04)) ? 1 : 0) << CHSELC) |
                  (((channel & (0x02)) ? 1 : 0) << CHSELB) |
                  (((channel & (0x01)) ? 1 : 0) << CHSELA);

   CHSELOUT = (CHSELOUT & ~(CHSELPINS)) | channelState;
   return;
}


uint16_t Sample_Channel(uint8_t channel)
{
   ADC_SetupAddress(DEFAULT_ADC_CHANNEL);
   return ADC_Sample();
}



