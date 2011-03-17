
#include <io.h>
#include <stdint.h>
#include "pwm.h"



void PWMInit(void)
{
   PWM_SEL |= (1 << PWM_PIN);
   PWM_DIR |= (1 << PWM_PIN);
   /* Use the SMCLK and Continuous Operation */
   PWM_Enable();
   PWM_TopCount(0);
   PWM_CTL = OUTMOD_RESET_SET;
}


void PWM_TopCount(uint16_t topCount)
{
   TACCR0 = topCount; 
}


void PWM_Duty(uint16_t duty)
{
   TACCR1 = duty;  
}




