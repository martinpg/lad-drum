/** \file UI.h
	 \brief User Interface Routines
*/

/**	 

	\ingroup edrum
 	\defgroup UI User Interface Module
 	\code #include "UI.h" \endcode

 	 
	 \par Description
    Sets up the User Interface Module for interrupts
*/
//@{

#ifndef	_UI_ROUTINES
#define	_UI_ROUTINES

#include "MAX7300/max7300.h"

#define UI_MAX7300_ADDRESS   (0x80)
#define UI_HIGH   (1)
#define UI_LOW    (0)
#define UI_INVALID_TRANSITION (8)


/* Interrupt Inputs */
#define UI_INTERRUPT    (MAX7300_P31)

#define UI_RTC_INT0     (MAX7300_TM_P29)
#define UI_RTC_INT1     (MAX7300_TM_P30)

#define UI_INT_COL0  (MAX7300_TM_P28)
#define UI_INT_COL1  (MAX7300_TM_P27)
#define UI_INT_COL2  (MAX7300_TM_P26)
#define UI_INT_COL3  (MAX7300_TM_P25)



#define UI_ReadRegister(addr) MAX7300_ReadRegister(UI_MAX7300_ADDRESS, addr)
#define UI_SetRegister(addr, data) MAX7300_SetRegister(UI_MAX7300_ADDRESS, addr, data)
#define UI_SetBlock(data, bytes) MAX7300_SetBlock_C(UI_MAX7300_ADDRESS, data, bytes)


void UI_Activate(void);
#define UI_EnableInterrupt() UI_SetRegister(MAX7300_CONFIG, (1 << MAX7300_SHUTDOWN_CONTROL) | (1 << MAX7300_TRANSITION_ENABLE))



#endif
