#include "hardwareSpecific.h" 
#include "UI_LCD/UI_LCD.h"
#include "UI_KP.h"
#include "LCDSettings.h"

#include "SPI/spi.h"
#include "VUmeter/vumeter.h"
#include "Sample/sample.h"
#include "Sensor/sensor.h"
#include <stdlib.h>

const char VersionId[] = "1.4a 31/01/11";
uint16_t BenchMarkCount = 0;

uint8_t ActiveProcess = 0;

int main(void)
{


   SPI_Init();
   
   LCD_BL_DDR |= (1<<LCD_BL_PIN);
   
   UI_LCD_HWInit();
   _delay_ms(20);

   UI_LCD_Init(&PrimaryDisplay);

   UI_LCD_String(&PrimaryDisplay, "Hullo");

   LCD_BL_PORT |= (1 << LCD_BL_PIN);

   DDRD &= ~(1 << 3);
   PORTD &= ~(1<<3);

   MCUCSR = (1 << JTD);
   MCUCSR = (1 << JTD);

   /*Activate Interrupt */
   MCUCR |= ((1 << ISC11) | (1 << ISC10));
   GICR |= (1 << INT1);

   UI_KP_Init();

   /* ADC Module Init */
   ADC_Init();
   ADC_SetupAddress(SENSOR_OUTPUT2);

   SensorInit();
   SensorChannel(7);

   /* Load the VU meter stuff */
   UI_LCD_LoadDefaultChars();

   VUSetPosition(0,0);
   VUSetRows(MAX_ROWS);


   sei();

   uint16_t k;

   while (1)
   {   

      k++;
      uint16_t result;
      char outputString[6];
      /* Do the VU Meter*/
      uint16_t i;
      uint8_t  VURows = GetVURows();


      if( (k % 20) == 0 )
      {
         result = ADC_Sample();
         VUSetLevel(0, result >> 3, VURows);
      }



		   			           
      VUMeterPrint(SEQUENTIAL_METERS | 0x01, VURows);

      if( (k % 10) == 0)
      {
         VULevelDecay(ALL_METERS);
         //ResetVUValues();
      }
      _delay_ms(1);
   }

   return 0;

}


ISR(INT1_vect)
{

   char outputString[5];
   static uint8_t j;
   uint8_t result = 0;

   result = UI_KP_ReturnID(UI_KP_GetPress());
   
   if( result != KP_INVALID )
   {
      UI_LCD_Clear(&PrimaryDisplay);
      UI_LCD_Pos(&PrimaryDisplay, 1, 0);
      utoa(result, outputString, 10);
      UI_LCD_String(&PrimaryDisplay,"R:");
      UI_LCD_String(&PrimaryDisplay,outputString);
   
      j++;
      utoa(j, outputString, 10);
      UI_LCD_Pos(&PrimaryDisplay, 2, 0);
      UI_LCD_String(&PrimaryDisplay,"j=");
      UI_LCD_String(&PrimaryDisplay,outputString);

      SensorChannel(result);
   }


   _delay_ms(40);
  

   GIFR |= (1 << INTF1);

}


