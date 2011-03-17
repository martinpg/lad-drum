/** \file UI_KP.h
	 \brief UART Commands Interface
*/

/**	 

	\ingroup edrum
 	\defgroup UI_KP Keypad Interface
 	\code #include "UI_KP.h" \endcode

 	 
	 \par Description
*/
//@{
   


#ifndef	_UI_KP_ROUTINES
#define	_UI_KP_ROUTINES

#include "version.h"


#define KP_ButtonCount  (16)
#define KP_INVALID   (0xFF)
#define KP_UPDATE    (0x0A)


void UI_KP_Init(void);

/** Returns the KP button pressed */
uint8_t UI_KP_GetPress(void);

/* Returns 0->15 depending on input keypad input*/
uint8_t UI_KP_ReturnID(uint8_t KP_Button);



#endif
