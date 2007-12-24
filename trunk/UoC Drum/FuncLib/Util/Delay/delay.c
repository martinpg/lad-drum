
#include <io.h>
#include <stdint.h>

/* General Delay Routines */



void _delay_us(uint16_t us)
{
   uint16_t i;
   
   while( us-- )
   {
      for( i = 0; i < (F_CPU / 1048576); i++)
      {
         nop();  
      }
   }
}

void _delay_ms(uint16_t ms)
{
   uint16_t i;
   
   while( ms-- )
   {
      _delay_us(1000);
   }
}
