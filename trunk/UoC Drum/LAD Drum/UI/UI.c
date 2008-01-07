#include <stdint.h>
#include "UI.h"


#if USE_MAX7300 == 1

#include "MAX7300/max7300.h"


/* Turns the UI MAX7300 on and turns on interrupts */
void UI_Activate(void)
{       
   /* Enable Transition detection on Keypad Columns */   
   UI_SetRegister(MAX7300_TRANSITION_MASK, (1 << MAX7300_TM_P28) |
                                           (1 << MAX7300_TM_P27) |
                                           (1 << MAX7300_TM_P26) |
                                           (1 << MAX7300_TM_P25));
	   
   /* Set Config to turn on and Enable Transition detect */
   UI_SetRegister(MAX7300_CONFIG, (1 << MAX7300_SHUTDOWN_CONTROL) | (1 << MAX7300_TRANSITION_ENABLE));
}

#else


/* Activates and Enables Interrupts of the Keypad Columns */
void UI_Activate(void)
{       
	/* Set Keypad Columns as inputs */
	UI_KP_DIR &= ~(UI_INT_COL0 | UI_INT_COL1 | UI_INT_COL2 | UI_INT_COL3);
	
	/* Trigger interrupts on a logic high to low transition */
	UI_INT_IES |= (UI_INT_COL0 | UI_INT_COL1 | UI_INT_COL2 | UI_INT_COL3);
	/* Enable Interrupts */
	UI_INT_IE |= (UI_INT_COL0 | UI_INT_COL1 | UI_INT_COL2 | UI_INT_COL3);
}




#endif






