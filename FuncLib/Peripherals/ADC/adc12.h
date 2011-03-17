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
