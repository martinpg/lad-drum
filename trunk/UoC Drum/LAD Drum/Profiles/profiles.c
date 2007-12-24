
#include <io.h>
#include <mspgcc/flash.h>
#include "profiles.h"



/* Sets up the clock with MCLK*/
void ProfileInit(void)
{
   /* Divide 8MHZ by 20 as clk range must be within 257 -> 457kHz*/
   FCTL2 = FWKEY | FSSEL_MCLK | 0x1F;
}


void Profile_Copy(void)
{
   flash_erase_segment( (uint16_t*)PROFILE_IMAGE_ADDRESS);
   

   flash_write((uint16_t*)PROFILE_IMAGE_ADDRESS, 
               (uint16_t*)PROFILE_FLASH_ADDRESS, 
               FLASH_BLOCK_SIZE);
}


/* Writes profile data to the profile Index,
   Profile Index (0 -> 3) */
void Profile_Write(Profile_t* profile, uint8_t profileIndex)
{
   uint8_t i;
   
   Profile_Copy();   
   flash_erase_segment( (uint16_t*)PROFILE_FLASH_ADDRESS);
   
   for( i = 0; i < NUMBER_OF_PROFILES; i++ )
   {
      if( i == profileIndex )
      {
         flash_write((uint16_t*)(PROFILE_FLASH_ADDRESS + (sizeof(Profile_t)*profileIndex)), 
                     profile, 
                     sizeof(Profile_t));      
      }
      else
      {
         flash_write((uint16_t*)(PROFILE_FLASH_ADDRESS + (sizeof(Profile_t)*i)), 
                     (uint16_t*)(PROFILE_IMAGE_ADDRESS + (sizeof(Profile_t)*i)), 
                     sizeof(Profile_t));              
      }
   }
}


Profile_t* Profile_Read(uint8_t profileIndex)
{
	if( profileIndex >= DEFAULT_PROFILE )
	{
		return (Profile_t*)&Default_Profile; 	
	} 
   return (Profile_t*)PROFILE(profileIndex);   
}
