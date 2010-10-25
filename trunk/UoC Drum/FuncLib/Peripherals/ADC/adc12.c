/* MSP430 ADC12 Routines
 * Adrian Gin
 *
 */
 
 
#include <io.h>
#include "adc12.h"
#include "Delay/delay.h"

void ADC12_Init(void)
{
   /* Make all ADC inputs as inputs and select as special function */
   P6SEL |= (0xFF);
   P6DIR &= ~(0xFF);
   P6OUT &= ~(0xFF);

   /* Try a sample time of 512 clks */
   ADC12_SH_Time(0xAA);
   
   /* Activate on ADC12SC bit, 1x Clock Divider, ADC12MEM0 result start,
    * Based on non-inverting pulses, Single Channel, single conversion
    * and using the MCLK */
   ADC12CTL1 = ADC12SSEL_MCLK | CONSEQ_SINGLE | ADC12DIV_1;
   
   /* Activate ADC12 */
   ADC12CTL0 |= (ADC12ON);
}

/* Sample and return on selected StartAddress's Pin*/
uint16_t ADC12_Sample(void)
{
   uint16_t channel; 
   /* Create a sample */
   ADC12CTL0 |= (ENC | ADC12SC);
   ADC12CTL0 &= ~ADC12SC; 
   
   channel = ADC12_GetStartAddress();
   
   /* Poll corresponding interrupt flag until conversion is complete */
   while( (ADC12CTL1 & (1 << ADC12BUSY)) );

   return *(&ADC12MEM0 + (channel * 2));
}

/* startAdd accepts input from CSTARTADD_0 -> CSTARTADD_15 */
void ADC12_SetStartAddress(uint16_t startAdd)
{
   ADC12CTL1 &= ~(CSTARTADD_15);
   ADC12CTL1 |= (startAdd);
}

uint8_t ADC12_GetStartAddress(void)
{
   uint16_t startAdd;
   startAdd = ((ADC12CTL1 & 0xF000) >> 12) & 0xFF;
   return (uint8_t)startAdd;
}

/* All functions which edit the Multiplexer must take place when the ADC12
 * is set to 0, or DISABLED (call ADC12_Disable() ); */

/* Routes memory slot to a pin INCH_A0 -> INCH_A7 or INCH_VEREF_PLUS etc (see adc12.h)
 * Channel accepts input from #0 -> #15 *
 * The settings is SREFx | INCHx eg. INCH_0 | SREF_0 */
void ADC12_SetupAddress(uint8_t channel, uint8_t settings)
{
   uint8_t* address;
 
   ADC12_Disable();
   address = (uint8_t*)(&ADC12MCTL0 + (channel * 2));
   
   (*address) &= ~(0x7F);
   (*address) |= (settings);
   ADC12_Enable();
}


void ADC12_SH_Time(uint16_t SampleHold)
{
   ADC12CTL0 &= ~(0xFF00); 
   ADC12CTL0 |= (SampleHold << 8); 
}








