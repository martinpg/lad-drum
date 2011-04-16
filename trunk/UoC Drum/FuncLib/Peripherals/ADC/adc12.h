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



/* MSP430 ADC12 Routines
 * Adrian Gin
 *
 */

#ifndef _ADC12_H_
#define _ADC12_H_ 


#define ADC12_Enable()  ADC12CTL0 |= ENC
#define ADC12_Disable()  ADC12CTL0 &= ~ENC

void ADC12_Init(void);
void ADC12_SetStartAddress(uint16_t startAdd);
uint8_t ADC12_GetStartAddress(void);
void ADC12_SetupAddress(uint8_t channel, uint8_t settings);
void ADC12_SH_Time(uint16_t SampleHold);

/* Sample and return on selected StartAddress's Pin*/
uint16_t ADC12_Sample(void);





#endif
