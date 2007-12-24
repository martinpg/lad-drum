#ifndef	PWM_H
#define	PWM_H

#include <io.h>

#define PWM_OUT   P1OUT
#define PWM_DIR   P1DIR
#define PWM_SEL   P1SEL
#define PWM_PIN   2
#define PWM_CTL   TACCTL1
#define PWM_CNT   TACCR1

/* Using Timer A1 to generate PWM waveform */
#define PWM_Disable() TACTL = TASSEL_SMCLK | MC_STOP
#define PWM_Enable() TACTL = TASSEL_SMCLK | MC_UPTO_CCR0

void PWMInit(void);
void PWM_TopCount(uint16_t topCount);
void PWM_Duty(uint16_t duty);



#endif
