/*
   MSP430F16x Electronic Drum Kit
   
*/
#include <io.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <mspgcc/flash.h>
#include <mspgcc/util.h>


#include "main.h"
#include "Delay/delay.h"
#include "UART/uart.h"
#include "Sample/sample.h"
#include "Sensor/sensor.h"
#include "ADC/adc12.h"
#include "mmculib/uint16toa.h"
#include "SoftI2C/softi2c.h"
#include "UI_LCD/UI_LCD.h"
#include "UI/UI.h"
#include "UI_KP/UI_KP.h"
#include "Softtimer/Softtimer.h"
#include "Menu/Menu.h"
#include "MIDI/midi.h"
#include "Profiles/profiles.h"
#include "VUMeter/vumeter.h"
#include "UserFunctions/userFunctions.h"
#include "version.h"

#if VERSION_CODE == VERSION_WITH_PE
const char VersionId[] = "1.0W 4/1/08";
#endif

#if VERSION_CODE == VERSION_WITHOUT_PE
const char VersionId[] = "1.0 4/1/08";
#endif


/**
Main function with some blinking leds
*/
int main(void) 
{

   WDTCTL = WDTPW + WDTHOLD;
   
   /* Setup the clocks */
   BCSCTL1 &= ~XT2OFF;
   IFG1 &= ~OFIFG;
   while( IFG1 & OFIFG )
   {
      IFG1 &= ~OFIFG;
      _delay_us(50);
   }

   /* SMCLK is XT2 and so is the MCLK */
   BCSCTL2 |= (SELM1 | SELS);   

   ProfileInit();    
   SensorInit();
	DigitalInputInit();
   /* Setup the communications module */   
   UART_Select(0);
   UART_Init();
	
	Profile_Read(DEFAULT_PROFILE);

   /* Implement the changes */
	MIDI_SetRate(MIDI_GetRate());
	MIDI_SetBaud(MIDI_GetBaud());
	MIDI_SetChannelCode( MIDI_GetChannelCode() );

   /* Update the Retrigger periods */
   UpdateChannelRetriggers();

   /* Make all ADC inputs as inputs and select as special function */
   P6SEL |= (0xFF);
   P6DIR &= ~(0xFF);
   

   
   /* ADC Module Init */
   ADC12_Init();
   ADC12_SetStartAddress(CSTARTADD_0);
   ADC12_SetupAddress(0, INCH_A0);
   ADC12_Enable();   
   
   TimerInit();
   


   /* Enable Interrupt detection on INTP1.3 for a Low to High only for
	 * MAX7300 */
#if VERSION_CODE == VERSION_WITH_PE  
   P1DIR &= ~(UI_INT_PIN);
   P1IES &= ~(UI_INT_PIN);
   P1IE  |=  (UI_INT_PIN);

   I2CInit();   
#endif

   /* Enable Keypad */
   UI_KP_Init();   
   UI_Activate();

   /* Enable LCD */
   UI_LCD_HWInit();
	UI_LCD_Init();

   LCD_BL_DDR |= (1 << LCD_BL_PIN);
   LCD_BL_PORT &= ~(1 << LCD_BL_PIN);

   MenuSetDisplay(MENU_LCD);
   /* Menu Setup */
   MenuSetInput(0); 
	
	
	/* Menu must be Initialised first */
	aboutScroll(MAIN_SCREEN);
	UI_LCD_BL_On();
	SoftTimerStart( SoftTimer2[SC_LCD_BL_Period] );	
	_delay_ms(900);	
	UI_LCD_LoadDefaultChars();					  
   /* Reprint Menu */   
   MenuUpdate();   
   

   uint16_t sample;
   

   /* Enable interrupts */
   eint();  
   
   while(1)
   {     
	   uint8_t i;
		eint();  
		for( i = 0; i < NUMBER_OF_INPUTS; i++ )
	   {
	      if(GetChannelStatus(i) && (RetriggerPeriod[i].timerEnable == 0) )
	      {
	         /* Change the channel */              
	         SensorChannel(i);
	         _delay_us(SensorSettings.CrosstalkDelay);
	         /* Take a sample */
	         sample = ADC12_Sample();                              
	         /* Obtain Peak */
	         ObtainPeak(i, sample);
	      }
	   } 
   }
   
   return 0;

}



interrupt (USART0RX_VECTOR) usart0_rx(void)
{
   uint8_t buffer = U0RXBUF;
   static uint8_t channel = 0;
   
   if( buffer == '+' )
   {
      SensorChannel(channel++);
      if(channel == 16)
      {
         channel = 0;  
      }
   }
  
  
   if( buffer == 'D' )
   {
      UART_TxDump((uint8_t*)PROFILE_FLASH_ADDRESS(0), 512 );
      UART_TxDump((uint8_t*)PROFILE_FLASH_ADDRESS(1), 512 );
      UART_TxDump((uint8_t*)PROFILE_FLASH_ADDRESS(2), 512 );		      
   }

	if( buffer == 'E' )
   {
      UART_TxDump((uint8_t*)PROFILE_IMAGE_ADDRESS(0), 512 );
      UART_TxDump((uint8_t*)PROFILE_IMAGE_ADDRESS(1), 512 ); 
      UART_TxDump((uint8_t*)PROFILE_IMAGE_ADDRESS(2), 512 );		     
   }

   if( buffer == 'b' )
   {
		SoftTimerStart(SoftTimer2[SC_VUMeterUpdate]);
      //MAX7300_SetRegister(UI_MAX7300_ADDRESS, MAX7300_P27, 0x01);
   }
   
   if( buffer == 'c' )
   {
		UI_LCD_Pos(3, 10);
		UI_LCD_Char(0);
		UI_LCD_Char(1);
		UI_LCD_Char(2);
		UI_LCD_Char(3);		
		UI_LCD_Char(4);
		UI_LCD_Char(5);
		UI_LCD_Char(6);
		UI_LCD_Char(7);		
	}
   
}

/* Handle a key press from a MAX7300 interrupt / or directly  */
interrupt (PORT1_VECTOR)   port1_int(void)
{  
   dint(); 
#if VERSION_CODE == VERSION_WITH_PE     
   if( (UI_INT_PORT & UI_INT_PIN) )
   {
		/* Reset Interrupt */
      UI_INT_IFG &= ~(UI_INT_PIN);  
#endif
      
#if VERSION_CODE == VERSION_WITHOUT_PE
   if( (UI_COL_IN & UI_COLS) )
   {
#endif      
      uint8_t IntResult;
      static uint8_t LCD_BL_Mode = LCD_BL_TIMED;
      /* Reset Interrupt on UI */
      IntResult = UI_KP_GetPress();      
      
      /* Toggles the permanent state of the LCD BL */
      if( IntResult == KP_5 )
      {
         LCD_BL_Mode ^= 1;
         
         if( LCD_BL_Mode == LCD_BL_TIMED )
         {
            UI_LCD_BL_Off();    
         }
         
      }      
      
      if( IntResult != KP_INVALID)
      {
			UI_LCD_BL_On();
			
			if( LCD_BL_Mode == LCD_BL_TIMED )
			{
            SoftTimerReset( SoftTimer2[SC_LCD_BL_Period] );
			   SoftTimerStart( SoftTimer2[SC_LCD_BL_Period] );
         }
         MenuSetInput(IntResult);   
         MenuUpdate();       
      }
      
          
#if VERSION_CODE == VERSION_WITH_PE    
   	UI_SetRegister(UI_INTERRUPT, 0);
#endif   

#if VERSION_CODE == VERSION_WITHOUT_PE 
		/* Reset Interrupt */
      UI_INT_IFG &= ~(UI_COLS);
#endif      
	
      UI_Activate();
   }
	eint();
}

