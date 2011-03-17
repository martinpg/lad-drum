#include "hardwareSpecific.h"



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
      return (*DIGITAL_INPUT_MAP[DigitalChannel].PIN) & (DIGITAL_INPUT_MAP[DigitalChannel].pinMask);
   }
   else
   {
      return 0;
   }
}



void SoftTimer_TimerInit(void)
{
   /* Primary Timer 1024 prescaler (high priority) */
   TCCR2 |= (0x03);
   TIMSK |= (1 << OCIE2);   
   /* Secondary Timer (non critical) */
   TCCR0 |= (0x05);
   TIMSK |= (1 << OCIE0);
}


