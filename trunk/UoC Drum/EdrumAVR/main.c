#include "hardwareSpecific.h" 
#include "UI_LCD/UI_LCD.h"
#include "UI_KP.h"
#include "LCDSettings.h"
#include "MenuSettings.h"
#include "Profiles/profiles.h"
#include "UserFunctions/userfunctions.h"

#include "SPI/spi.h"
#include "VUmeter/vumeter.h"
#include "Sample/sample.h"
#include "Sensor/sensor.h"

#include "mmculib/uint8toa.h"
#include <stdlib.h>

const prog_char VersionId[] = "1.4a 31/1/11";

uint16_t BenchMarkCount = 0;

uint8_t ActiveProcess = 0;

int main(void)
{
   DDRD &= ~(1 << 3);
   PORTD &= ~(1<<3);

   MCUCSR = (1 << JTD);
   MCUCSR = (1 << JTD);

   /*Activate Interrupt */
   MCUCR |= ((1 << ISC11) | (1 << ISC10));
   GICR |= (1 << INT1);

   ProfileInit();
   
   /* Make profile 1 the default profile on start up */
   //Profile_Read(0);
   
   SPI_Init();
   SensorInit();
   SensorInputSelect(GetSensorInput());
   //DigitalInputInit();
   /* Setup the communications module */   
   UART_Init(0);

   /* Implement the changes */
   MIDI_SetRate(MIDI_GetRate());
   MIDI_SetBaud(MIDI_GetBaud());
   MIDI_SetChannelCode( MIDI_GetChannelCode() );

   /* Update Activated Analogue Channels */
   UpdateActiveChannels();

   /* Update the Retrigger periods */
   UpdateChannelRetriggers();

   /* ADC Module Init */
   ADC_Init();
   
   SoftTimer_TimerInit();

   /* Enable Keypad */
   UI_KP_Init();   

   /* Enable LCD */
   UI_LCD_HWInit();
   _delay_ms(20);
   UI_LCD_Init(&PrimaryDisplay);
   UI_LCD_LoadDefaultChars();
	
   LCD_BL_DDR |= (1 << LCD_BL_PIN);
   LCD_BL_PORT |= (1 << LCD_BL_PIN);

   MenuSetDisplay(&primaryMenu, MENU_LCD);
   MenuSetDisplay(&analogueMenu, MENU_LCD);
   MenuSetDisplay(&digitalMenu, MENU_LCD);      
   /* Menu Setup */
   MenuSetInput(&primaryMenu, 0); 
	
	/* Menu must be Initialised first */
	/* Backlight 'off' is at 5% */
   //UI_LCD_BLInit(5);
   //UI_LCD_BL_On();
	
   aboutScroll(MAIN_SCREEN);
   //SoftTimerStart( SoftTimer2[SC_LCD_BL_Period] );	
	

	
   _delay_ms(900);
   _delay_ms(900);	
   UI_LCD_LoadDefaultChars();					  
   /* Reprint Menu */  
   MenuUpdate(&primaryMenu, RESET_MENU);


   sei();

   uint8_t SelectedChannel = 0;
   uint16_t sample;

   while (1)
   {   

      

      uint8_t i = 0;
   
   	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
   	{
         SelectedChannel = ActiveChannels[i++];
      
         if( !(RetriggerPeriod[ SelectedChannel ].timerEnable) )
         {
            SensorChannel(SelectedChannel);
            _delay_us(SensorSettings->CrosstalkDelay);
            /* Take a sample */
            sample = ADC_Sample();
                           
            /* Obtain Peak */
            ObtainPeak(SelectedChannel, sample);
         } 
      }   
 
   }

   return 0;

}


ISR(INT1_vect)
{

   char outputString[5];
   static uint8_t j;
   uint8_t result, result2;

   result = UI_KP_GetPress();
   _delay_ms(1);
   result2 = UI_KP_GetPress();

   _delay_ms(40);

   if( result == result2 )
   {
      if( result != KP_INVALID )
      {
         MenuSetInput(ActiveMenu, result);
         GICR &= ~(1 << INT1);  
         sei();
         MenuUpdate(ActiveMenu, RESET_MENU);
               /*UF_MenuPrint_P( PSTR("KP:") );
               uint8toa(result, outputString);
               UF_MenuPrint( outputString );*/

         //MenuSetInput(ActiveMenu, KP_UPDATE);   
         //MenuUpdate(ActiveMenu, RESET_MENU);
         //
         //SensorChannel(result);
      }
   }
   
   GIFR |= (1 << INTF1);
   GICR |= (1 << INT1);
}


