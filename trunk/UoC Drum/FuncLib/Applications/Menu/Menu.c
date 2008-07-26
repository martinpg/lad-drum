
#include <stdint.h>
#include <string.h>
#include "Menu.h"
#include "UI_KP/UI_KP.h"





/** Updates the state of the menu, if RESET_MENU is passed to options, the screen is wiped.
                                      HIDE_CHILDREN is passed to options, the submenu items are displayed */
void MenuUpdate(Menu_t* menu, uint8_t options)
{
   uint8_t i;   
   uint8_t sequenceIndex = 0;
   uint8_t MenuOffset;
   uint8_t MenuMax;
   
   uint8_t scrollUp = 0;
   uint8_t scrollDown = 0;
   
   uint8_t windowSize = menu->windowSize;
   
   char* outputString;
   
   if( options & RESET_MENU )
   {
      menu->MenuReset();
   }
      

   /** Only switch Menu input IF we are in a menu item which has NO associated
    * function */
   if( menu->structure[ GetIndex(menu, menu->currentState) ].function == 0 )
   {
      stateMachine(menu, menu->currentState ); 
   }
        
   /** Run the associated function if it has one */   
   executeState(menu, menu->currentState);    

   if( !(menu->updateOptions & HIDE_CHILDREN) )
   {

      MenuOffset = SmallestSequence(menu, menu->currentState);
      MenuMax = LargestSequence(menu, menu->currentState);
   
      
      /** Print out the menu's sub-menu's */   
      /* Ensures that the screen limits are not exceeded */
      for( i = 0, sequenceIndex = 0;  menu->states[i].parent != 0 ; i++)
      {  
         /* Find the current state's sub children. */        
         if( menu->states[i].parent == menu->currentState )
         {  
            sequenceIndex = menu->states[i].sequence;
            
            
            if( menu->selectedItem > menu->upperLimit )
            {
               menu->upperLimit = menu->selectedItem;
               menu->lowerLimit = menu->upperLimit - windowSize + MenuOffset;   
            }
               
            if( menu->selectedItem < menu->lowerLimit )
            {
               menu->lowerLimit = menu->selectedItem;
               menu->upperLimit = menu->lowerLimit + windowSize - MenuOffset;               
            }
            
            /* If this is the selected item then prefix an asterix */
            if( (sequenceIndex <= menu->upperLimit) && (sequenceIndex >= menu->lowerLimit) )
            {
               if(sequenceIndex > windowSize)
               {
                  scrollUp = SCROLL_BAR;
               }
               
               if( MenuMax - sequenceIndex + MenuOffset  > windowSize)
               {
                  scrollDown = SCROLL_BAR;
               }
               
               //menu->MenuSetPos(menu->RowPosition, 0); 
               
               if( menu->states[i].sequence == menu->selectedItem)
               {
   #if MENU_DEBUG == 1  
                  printf("*");    
               }
               else
               {
                  printf(" ");   
               }
   #else
                  menu->MenuChar( '*' );    
               }
               else
               {
                  menu->MenuChar( ' ' );   
               }
   #endif
                          
               outputString = MenuDescriptor(menu, menu->states[i].child );
               
   #if MENU_DEBUG == 1                  
               if( outputString )
               {
                  PRINT_FUNC("%s\n", (uint8_t*)outputString );
               }
   #else
   
               menu->MenuPrint((char*)outputString);
               menu->MenuNewLine();            
   #endif            
            }
         }  
         
         
      }
      
      
      if( scrollUp == SCROLL_BAR)
      {
         menu->MenuSetPos(0, 19);
         menu->MenuChar( '^' );  
      }
      
      if( scrollDown == SCROLL_BAR)
      {
         menu->MenuSetPos(windowSize, 19);
         /* Down arrow assumed to be in CGRAM 1 */
         menu->MenuChar( 0 );
      }   
   }
   
}


/** Determines the new state depending on the user input and passed state */
void stateMachine(Menu_t* menu, uint8_t state)
{
   uint8_t maxStateItems;
   uint8_t parentIndex = 0;
   
   
   
   maxStateItems = SubItems(menu, state);
   
   

   switch( menu->MenuInput )
   {
      case KB_UP:
      case KP_UP:
         if(!(menu->selectedItem <= SmallestSequence(menu, menu->currentState)) )
         {
            menu->selectedItem--;
         }
      break;
         
      case KB_DOWN:   
      case KP_DOWN:
         menu->selectedItem++;          
         if( menu->selectedItem - SmallestSequence(menu, menu->currentState) > maxStateItems )
         {
            menu->selectedItem = LargestSequence(menu, menu->currentState);
         }
      break;
      
      case KB_ENTER:
      case KP_ENTER:
         /* Go into child sub menu */
         menu->currentState = GetMenuState(menu, menu->currentState, menu->selectedItem);
         menu->selectedItem = SmallestSequence(menu, menu->currentState);
         menu->upperLimit = menu->windowSize;
         menu->lowerLimit = 0;
         menu->firstEnter = 1; 
         
      break;
      
      case KB_BACK:
      case KP_BACK: 
         /** Need to reset the 'first' enter flag */
         /** This is so that the commands of a function are not
          * executed on entering the associated sub-menu */
         menu->firstEnter = 1;                
         parentIndex = GetParent(menu, menu->currentState);
         menu->lowerLimit = 0;
         menu->upperLimit = menu->windowSize;
         if(parentIndex != INVALID_STATE)
         {
            menu->upperLimit = menu->windowSize;
            menu->lowerLimit = 0;
            
            menu->currentState = menu->states[ parentIndex ].parent;
            menu->selectedItem = menu->states[ parentIndex ].sequence; 
         }
      break;     
      
      default:
      break;   
   }

   /* Revert back to initial state */
   if(menu->currentState == NO_STATE || parentIndex == INVALID_STATE)
   {
      menu->currentState = state;       
   }
}

/* Returns the Menu descriptor given the menu Item / Index */
char* MenuDescriptor(Menu_t* menu, uint8_t menuItem)
{

   uint8_t index;
   static char buffer[21];
      
   index = GetIndex(menu, menuItem);
   
   if( index  != INVALID_STATE )
   {
      strcpy(buffer, menu->structure[index].descriptor);
      return buffer;
   }
   return 0;
}


/* Returns the corresponding menu->states 
 * given the Parent State and its sequnce. */
uint8_t GetMenuState(Menu_t* menu, uint8_t state, uint8_t Sequence)
{
   int i;
   for( i = 0; menu->states[i].parent; i++)
   {
      if( menu->states[i].parent == state
       && menu->states[i].sequence == Sequence ){      
         return menu->states[i].child;
      }
   }
   return NO_STATE;
}

/* Get out the first instance of where the given state
 * origninates from */
uint8_t GetParent(Menu_t* menu, uint8_t state)
{
 
   int i;  
   for( i = 0; menu->states[i].parent; i++)
   {
      if( menu->states[i].child == state) {      
         return i;
      }
   }
   return INVALID_STATE;
}

/* Updates the input last received by the menu */
void MenuSetInput(Menu_t* menu, uint8_t NewInput)
{
   menu->MenuInput = NewInput;    
}

/** Calls the associated menu's function */
void executeState(Menu_t* menu, uint8_t state)
{
   uint8_t index;
   void (*funcPtr)(void* data);
   
   index = GetIndex(menu, state);
   
   if(index != INVALID_STATE)
   {
      if( menu->structure[index].function != 0)
      {
         funcPtr = (void*)menu->structure[index].function;
         funcPtr(&menu->MenuInput);
      }
   }
   
   
}

/*& Get the element in MenuData which has 'parent' */
uint8_t GetIndex(Menu_t* menu, uint8_t parent)
{
   uint8_t i;
   for( i = 0; menu->structure[i].menu_item; i++)
   {
      if( menu->structure[i].menu_item == parent)
      {
         return i;   
      } 
   }
   
   return INVALID_STATE;
   
}

/** Get the number of sub items in the passed state */
uint8_t SubItems(Menu_t* menu, uint8_t state)
{
   int i;
   uint8_t StateItems = 0;    
   for( i = 0; menu->states[i].parent; i++)
   {
      if( menu->states[i].parent == state)
      {
         StateItems++;
      }
   }
   return (StateItems - 1);
}

/** Get the maximum sequence of the the passed state */
uint8_t LargestSequence(Menu_t* menu, uint8_t state)
{
   int i;
   uint8_t StateItem = 0;    
   for( i = 0; menu->states[i].parent; i++)
   {
      if( menu->states[i].parent == state)
      {
         /* Obtain the number of Menu Items in the given state */
         if( menu->states[i].sequence >= StateItem )
         {
            StateItem = menu->states[i].sequence;
         } 
      }
   }
   return (StateItem);
}

uint8_t SmallestSequence(Menu_t* menu, uint8_t state)
{
   int i;
   uint8_t StateItem = 0xFF;    
   for( i = 0; menu->states[i].parent; i++)
   {
      if( menu->states[i].parent == state)
      {
         /* Obtain the number of Menu Items in the given state */
         if( menu->states[i].sequence <= StateItem )
         {
            StateItem = menu->states[i].sequence;
         } 
      }
   }
   return (StateItem);
}


/* Returns the sequence given the menu state */
uint8_t GetSequence(Menu_t* menu, uint8_t parent, uint8_t child)
{
   uint8_t parentIndex;
   uint8_t i;
   
   parentIndex = GetIndex(menu, parent);
   

   for( i = 0; menu->states[i].parent; i++)
   {
      if( menu->states[i].parent == parent &&
          menu->states[i].child  == child )
      {
         return menu->states[i].sequence;     
      }
   }      
   return INVALID_SEQUENCE;
}


uint8_t GetState(Menu_t* menu)
{
   return menu->currentState;  
}
