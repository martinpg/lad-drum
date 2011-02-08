/** \file Menu.h
	 \brief Menu Machine for AVR
*/

/**	 

	\ingroup avr_datalogger
 	\defgroup menu Menu State Machine
 	\code #include "menu.h" \endcode

 	 
	 \par Description
	Implements a menu function.
 	 
*/
//@{


#ifndef	_MENU_ROUTINES
#define	_MENU_ROUTINES

#define MENU_DEBUG 0

#define PRINT_FUNC   UI_LCD_String_P
/* These should reside in the hardwareSpecific.h */
//#define MENU_GET_PTR(x)  pgm_read_word(x)
//#define MENU_GET_BYTE(x)  pgm_read_byte(x)
//#define MENU_SPACE PROGMEM // for the AVR
//#define MENU_TEXT    char //for the AVR use prog_char if you want strings in ROM
//#define MENU_SPACE_PTR	PGM_P
//#define PSTR(x)   (x) //for AVR include the pgmspace.h for this macro

#define NO_STATE     0
#define INVALID_STATE 255
#define INVALID_SEQUENCE 255

#define MAIN_MENU    ST_MAIN
#define MENU_LCD  0
#define MENU_UART    (1)
#define RESET_MENU     (1)
#define SHOW_CHILDREN (0x00)
#define HIDE_CHILDREN (0x02)
#define SCROLL_BAR   (1)


#define WINDOW_SIZE   (3)
#define WINDOW_LENGTH (20)

#if MENU_DEBUG == 1

#define KP_1   '1'
#define KP_2   '2'
#define KP_3   '3'
#define KP_4   '4'
#define KP_5   '5'
#define KP_6   '6'
#define KP_7   '7'
#define KP_8   '8'
#define KP_9   '9'



#endif

#define KB_UP '8'
#define KB_DOWN '2'
#define KB_ENTER '6'
#define KB_BACK  '4'

#define KP_UP  KP_2
#define KP_DOWN KP_8
#define KP_ENTER KP_6
#define KP_BACK  KP_4



typedef struct
{
   uint8_t  parent;
   uint8_t  child;
   uint8_t  sequence; /// Effectively, which line of the LCD the item is listed on.
   
} menu_list;


typedef struct
{
   uint8_t  menu_item; 
   const char* descriptor;
   void (*function)(void* data);

} menu_data;


typedef struct Menu_t
{

/* Initial State of the Menu */
   uint8_t currentState;

/* Menu Input */
   uint8_t MenuInput;
/* Initial Selected Item, in terms of sequence in Menu Structure */
   uint8_t selectedItem;

   uint8_t windowSize;   
      
   
/* Size of Window */
   uint8_t upperLimit;
   uint8_t lowerLimit;
/* Flag */
   uint8_t firstEnter;   
   
   struct Menu_t* parentMenu;
   
   uint8_t updateOptions;
   
   menu_list* states;
   menu_data* structure;
   
   void (*MenuPrint)(char* string);
   /* For a string residing in FLASH */
   void (*MenuPrint_P)(const char* string);
   void (*MenuNewLine)(void);
   void (*MenuReset)(void);
   void (*MenuChar)(uint8_t data);
   void (*MenuSetPos)(uint8_t row, uint8_t col);
      
} Menu_t;

/** General Menu Functions */


void MenuSetInput(Menu_t* menu, uint8_t NewInput);

void MenuUpdate(Menu_t* menu, uint8_t resetMenu);

char* MenuDescriptor(Menu_t* menu, uint8_t menuItem);

void stateMachine(Menu_t* menu, uint8_t CurrentState);
uint8_t GetMenuState(Menu_t* menu, uint8_t state, uint8_t Sequence);
uint8_t GetParent(Menu_t* menu, uint8_t state);
uint8_t SubItems(Menu_t* menu, uint8_t state);
uint8_t GetIndex(Menu_t* menu, uint8_t parent);
void executeState(Menu_t* menu, uint8_t state);
uint8_t LargestSequence(Menu_t* menu, uint8_t state);
uint8_t SmallestSequence(Menu_t* menu, uint8_t state);

uint8_t GetState(Menu_t* menu);

#endif








