/*
   MSP430F16x Electronic Drum Kit
   
*/
#include <io.h>
#include <stdint.h>
#include <signal.h>
#include <mspgcc/flash.h>
#include <mspgcc/util.h>


#include "main.h"
#include "Delay/delay.h"
#include "UART/uart.h"
#include "Sample/sample.h"
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








const Profile_t Default_Profile = 
{
	/* 5ms output rate */
	30,
	DEFAULT_BAUD_RATE,
	/* MIDI Channel Instrument # */
	0x99,
	
	/* Channel Status */
	{0x0F, 0x00},
	
	/* Default Keys */
	{0x42, 0x28, 0x53, 0x34,
    0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30},
   
	/* Default Gains */                                     
	{3,3,3,3,
    3,3,3,3,
    3,3,3,3,
    3,3,3,3},
            
	/* Default Thresholds */
	{MIN_THRESHOLD, MIN_THRESHOLD, MIN_THRESHOLD, MIN_THRESHOLD, 
	 MIN_THRESHOLD, MIN_THRESHOLD, MIN_THRESHOLD, MIN_THRESHOLD,
	 MIN_THRESHOLD, MIN_THRESHOLD, MIN_THRESHOLD, MIN_THRESHOLD,
	 MIN_THRESHOLD, MIN_THRESHOLD, MIN_THRESHOLD, MIN_THRESHOLD},
	 
	{MIN_RETRIGGER, MIN_RETRIGGER, MIN_RETRIGGER, MIN_RETRIGGER,
	 MIN_RETRIGGER, MIN_RETRIGGER, MIN_RETRIGGER, MIN_RETRIGGER,
	 MIN_RETRIGGER, MIN_RETRIGGER, MIN_RETRIGGER, MIN_RETRIGGER,
	 MIN_RETRIGGER, MIN_RETRIGGER, MIN_RETRIGGER, MIN_RETRIGGER}
};


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

   _delay_ms(10);
   

   /* SMCLK is XT2 and so is the MCLK */
   BCSCTL2 |= (SELM1 | SELS);   

   ProfileInit();    
   SampleInit();
   CurrentProfile = Default_Profile;
   
   P6SEL |= (0xFF);
   P6DIR &= ~(0xFF);
   
   /* Setup the communications module */   
   UART_Select(0);
   UART_Init();
   UART_SetBaudRate(0x01,0x00);
   
   /* ADC Module Init */
   ADC12_Init();
   ADC12_SetStartAddress(CSTARTADD_0);
   ADC12_SetupAddress(0, INCH_A0);
   ADC12_Enable();   
   
   TimerInit();
   
   I2CInit();
   /* Enable Keypad */
   UI_KP_Init();   
   UI_Activate();


   /* Enable Interrupt detection on INTP1.3 for a Low to High */
   P1DIR &= ~(UI_INT_PIN);
   P1IE  |=  (UI_INT_PIN);
   
   
   /* Enable LCD */
   UI_LCD_HWInit();
   UI_LCD_Init();
   UI_LCD_String("Adrian");
      
   LCD_BL_DDR |= (1 << LCD_BL_PIN);
   LCD_BL_PORT &= ~(1 << LCD_BL_PIN);
   
   MenuSetDisplay(MENU_LCD);
   /* Menu Setup */
   MenuSetInput(0);   
   /* Reprint Menu */   
   MenuUpdate();   
   
   dint();
 
   uint16_t sample;
   
   SoftTimer_16 ReadADC = {0, 0, 1};
   //ChannelThreshold[0] = 0;
   


   /* Enable interrupts */
   eint();  
   
   SampleChannel(0);
   
   while(1)
   {     
      while( ReadADC.timerEnable )
      {
         uint8_t i;
         for( i = 0; i < NUMBER_OF_INPUTS; i++ )
         {
            if(GetChannelStatus(i) && (RetriggerPeriod[i].timerEnable != 0) )
            {
               /* Change the channel */              
               SampleChannel(i);
               _delay_us(200);
               /* Take a sample */
               sample = ADC12_Sample();                              
               /* Obtain Peak */
               ObtainPeak(i, sample);
            }
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
      SampleChannel(channel++);
      if(channel == 16)
      {
         channel = 0;  
      }
   }
  
  
   if( buffer == 'D' )
   {
      UART_TxDump((uint8_t*)PROFILE_FLASH_ADDRESS, 512 );
   }

	if( buffer == 'E' )
   {
      UART_TxDump((uint8_t*)PROFILE_IMAGE_ADDRESS, 512 );
   }

   if( buffer == 'b' )
   {
      UART_TxString("You typed a b!");
      //MAX7300_SetRegister(UI_MAX7300_ADDRESS, MAX7300_P27, 0x01);
   }
}


/* Handle a key press */
interrupt (PORT1_VECTOR)   port1_int(void)
{  
   dint(); 
   if( P1IN & UI_INT_PIN )
   {
      P1IFG &= ~(UI_INT_PIN);
      
      uint8_t IntResult;
      /* Reset Interrupt on UI */
      IntResult = UI_KP_GetPress();      
      /*
      if( IntResult == KP_D )
      {
         LCD_BL_PORT ^= (1 << LCD_BL_PIN);  
      }*/
            
      if( IntResult != KP_INVALID)
      {
         MenuSetInput(IntResult);   
         MenuUpdate();         
      }  
      
      UI_SetRegister(UI_INTERRUPT, 0);      
      UI_Activate();
   }
   
   eint();
}


