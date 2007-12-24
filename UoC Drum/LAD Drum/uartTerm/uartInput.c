

#include <avr/pgmspace.h>
#include "uartInput.h"
#include "UserFunctions/userFunctions.h"
#include "hardUart/hardUart.h"
#include "main.h"



static const char uartReset[] PROGMEM = "Reset";
static const char uartTestMenuUart[] PROGMEM = "Set UART";
static const char uartHelp[] PROGMEM = "Help";

/* Gain and Sensor Select */


static const UartCompare UartFunctions[] PROGMEM= { 
   {uartReset, 1, Reset},    
   {uartTestMenuUart, 1, MenuSetUartMode},      
   {uartHelp, 1, uartTermHelp},  

   {0,0,0}
};


/** Returns a Uart Compare type given an address in PGM space */
UartCompare pgm_read_uartFunction(uint16_t* address_short)
{
   UartCompare returnUartFn;
   UartCompare* uartFnPtr;
   
   uartFnPtr = (UartCompare*)address_short;
 

   returnUartFn.compareString = (PGM_P)pgm_read_word( &uartFnPtr->compareString );
   returnUartFn.activated     = pgm_read_byte( &uartFnPtr->activated );
   returnUartFn.function      = (void*)pgm_read_word( &uartFnPtr->function );        
        
   return returnUartFn;   
   
}

/** Receives input from the Uart, byte by byte and determines
 * the function to be called */
void switch_uart_input(uint8_t nextChar)
{
   static uint8_t charCounter = 0;
   static uint8_t inputBuffer[UART_COMPARE_MAX_LENGTH+1];
   
   /** Ensure the maximum input length is not exceeded */
   if( charCounter > UART_COMPARE_MAX_LENGTH)
   {
      charCounter = 0;   
   }
   
   
   inputBuffer[charCounter] = nextChar;
   /* *A compare is initiated once a new line char is received */
   if( nextChar == UART_LINEFEED || nextChar == UART_CARRIAGE_RETURN)
   {
      
      inputBuffer[charCounter] = UART_NULL;  
      charCounter = 0;
      
      branch_input(inputBuffer);
      return;
   }

   if( nextChar == UART_DELETE)
   {
      inputBuffer[charCounter] = UART_NULL;  
      
      if(charCounter == 0)
      {
         charCounter = 1;
      } 
      charCounter -= 1;
      return;
   } 

   charCounter++;
   

}
   
/** Looks through all the valid Uart Commands and tests against the input string
  */
void branch_input(uint8_t* inputStr)
{
   uint8_t i = 0;
   UartCompare compareUnit;
   
   compareUnit = pgm_read_uartFunction((uint16_t*)&UartFunctions[i]);

   for( i = 0; 
        compareUnit.compareString; 
        (compareUnit = pgm_read_uartFunction((uint16_t*)&UartFunctions[++i])))
   {
      
      
      if(!strcmp_P( (const char*)inputStr, (compareUnit.compareString)))
      {
         compareUnit.function(0);
         return;
      }
   }
   
   
   uartTxString_P( PSTR("Invalid Command!") );
   uartNewLine();
   uartTxString_P( PSTR("Type 'Help' for a list of commands") );   
   uartNewLine();   
}


/** Prints out all the possible commands available */
void uartTermHelp(void* data)
{
   uint8_t i = 0;
   UartCompare compareUnit;   
   
   uartNewLine();
   uartTxString_P( PSTR("Valid Commands are:") );     
   
   compareUnit = (UartCompare)pgm_read_uartFunction((uint16_t*)&UartFunctions[i]);

   for( i = 0; 
        compareUnit.compareString; 
        (compareUnit = (UartCompare)pgm_read_uartFunction((uint16_t*)&UartFunctions[++i])))
   {
      
      uartNewLine();
      uartTxString_P(compareUnit.compareString);

   }   
}
