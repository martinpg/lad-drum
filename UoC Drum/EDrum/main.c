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
#include "MIDI/SysEx/SysEx.h"
#include "Profiles/profiles.h"
#include "VUMeter/vumeter.h"
#include "UserFunctions/userFunctions.h"
#include "version.h"
#include "LCDSettings.h"
#include "MenuSettings.h"

#if VERSION_CODE == VERSION_WITH_PE
const char VersionId[] = "1.0W 4/1/08";
#endif

#if VERSION_CODE == VERSION_WITHOUT_PE
/* Initial Release: 
   const char VersionId[] = "1.0 4/1/08";
*/
const char VersionId[] = "1.2 23/3/09";
/* Change Log 23/03/09
 * Note: Benchmarks performed and speed has been increase 10 fold.
 * Added Controller Mode process and Keypad inputs as extra digital channels.
 * Controller mode allows analogue channels have have potentiometer and
 * variable voltage input. MIDI is only output if the value detected is different.
 *
 * Hardware Change: USB Powered.
 *
 */

/* Change Log: 26/07/08
 * Optimised Menu System for Sub menus. Added SysEx Input & Output Capability 
 * Added ability to change MIDI output command code 
 *    Hence support for an Analogue Foot controller in the place of a
 *    piezo input has been added.
 * Better mutlitasking support, added the use of 'Processes' 
 * LCD Device and Menu structures have been optimised 
 * Smaller code size 
 *
 * Next revision: General clean up
 *                Port to Atmega series (MEGA32)
 *                Improve Multiplexing of signals to improve sample rates 
 */
 
 
/* Benchmark Results */
/* 28/02/09: Number of samples per millisecond.
Function is:
ADC12_Sample();
BenchMarkCount++;

50ms = 6300 samples
20ms = 2540 samples
10ms = 1280 samples
8ms = 1030 samples
5ms = 660 samples
2ms = 280 samples
1ms = 150 samples

Note that these samples are the 'maximum' possible. No Drum processing has occured.

==== BENCHMARK 2 ======
1. Removed retrigger period
2. Ignored Crosstalk delay.
3. Note that each 'sample' includes 13 samples (13 activated analogue channels)
4. Results:

50ms = 71 (923) samples
20ms = 30 (390) samples
15ms = 22 (286) samples
10ms = 15 (195) samples
8ms = 13 (169) samples
5ms = 9 (117) samples
2ms = 4 (52) samples
1ms = 3 (39) samples


void Benchmark(void)
{
   uint8_t i = 0;
   uint8_t SelectedChannel = 0;
   uint16_t sample;
   
	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      SelectedChannel = ActiveChannels[i++];
      SensorChannel(SelectedChannel);
      //_delay_us(SensorSettings->CrosstalkDelay);
      sample = ADC12_Sample();
      ObtainPeak(SelectedChannel, sample);
   }   
}


======= BENCHMARK 3 =======
1. Removed retrigger period
2. Ignored Crosstalk delay.
3. Note that each 'sample' includes 13 samples (13 activated analogue channels)
4. Results:

50ms = 408 (5304) samples
20ms = 167 (2171) samples
15ms = 127 (1651) samples
10ms = 87 (1131) samples
8ms = 71 (923) samples
5ms = 47 (611) samples
2ms = 23 (299) samples
1ms = 15 (195) samples

Perhaps ObtainPeak or SensorChannel uses many clock cycles

void Benchmark(void)
{
   uint8_t i = 0;
   uint8_t SelectedChannel = 0;
   uint16_t sample;
   
	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      SelectedChannel = ActiveChannels[i++];
      sample = ADC12_Sample();
   }   
}

======= BENCHMARK 4 =======
1. Removed retrigger period
2. Ignored Crosstalk delay. Added Channel Select Function.
3. Note that each 'sample' includes 13 samples (13 activated analogue channels)
4. Results:

50ms = 75 (975) samples
20ms = 31 (403) samples
15ms = 24 (312) samples
10ms = 16 (208) samples
8ms = 13 (169) samples
5ms = 9 (117) samples
2ms = 5 (65) samples
1ms = 3 (39) samples

Perhaps SensorChannel uses many clock cycles!! We must optimise this!

==== POST OPTIMISATION ======

50ms = 232 (3016) samples
20ms = 97 (1261) samples
15ms = 74 (962) samples
10ms = 51 (663) samples
8ms = 42 (546) samples
5ms = 27 (351) samples
2ms = 13 (169) samples
1ms = 9 (117) samples

HUGE 3x improvement!!!


void Benchmark(void)
{
   uint8_t i = 0;
   uint8_t SelectedChannel = 0;
   uint16_t sample;
   
	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      SelectedChannel = ActiveChannels[i++];
      SensorChannel(SelectedChannel);
      sample = ADC12_Sample();
   }   
}



======= BENCHMARK 5 =======
1. Added Obtain Peak
2. Results

50ms = 194 (2522) samples
20ms = 80 (1040) samples
15ms = 61 (793) samples
10ms = 42 (546) samples
8ms = 35 (455) samples
5ms = 23 (299) samples
2ms = 11 (143) samples
1ms = 7 (91) samples


void Benchmark(void)
{
   uint8_t i = 0;
   uint8_t SelectedChannel = 0;
   uint16_t sample;
   
	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      SelectedChannel = ActiveChannels[i++];      
		SensorChannel(SelectedChannel);
      sample = ADC12_Sample();                   
      ObtainPeak(SelectedChannel, sample);
   }   
}


======= BENCHMARK 6 =======
1. Now introducing the 150us delay... Oh dear!
2. Results

Okay it sux... like real bad! 5 samples for 15ms etc.

In light of this, we shall return back to using the play function, but attempt to increase
thresholds and reduce crosstalk delay.

Returning to play mode, and reducing Crosstalk delay makes performance much the same as
benchmark 5.

void Benchmark(void)
{
   uint8_t i = 0;
   uint8_t SelectedChannel = 0;
   uint16_t sample;
   
	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      SelectedChannel = ActiveChannels[i++];      
		SensorChannel(SelectedChannel);
		_delay_us(SensorSettings->CrosstalkDelay);
      sample = ADC12_Sample();                   
      ObtainPeak(SelectedChannel, sample);
   }   
}

======= BENCHMARK 6 =======
1. Gain's have been adjusted and so have thresholds
2. Cross talk delay has been reduced from 150us to 12us.
2. Results

This is about a 50% reduction in speed after introducing delay. And an overall
reduction in max speed by 

50ms = 108 (1404) samples
20ms = 44 (572) samples
15ms = 33 (429) samples
10ms = 22 (286) samples
8ms = 18 (234) samples
5ms = 12 (156) samples
2ms = 5 (65) samples
1ms = 3 (39) samples


*/
#endif

uint16_t BenchMarkCount = 0;

uint8_t ActiveProcess = 0;

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

   /* Implement the changes */
	MIDI_SetRate(MIDI_GetRate());
	MIDI_SetBaud(MIDI_GetBaud());
	MIDI_SetChannelCode( MIDI_GetChannelCode() );

   /* Update Activated Analogue Channels */
   UpdateActiveChannels();

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
	UI_LCD_Init(&PrimaryDisplay);
	UI_LCD_LoadDefaultChars();
	
   LCD_BL_DDR |= (1 << LCD_BL_PIN);
   LCD_BL_PORT &= ~(1 << LCD_BL_PIN);

   MenuSetDisplay(&primaryMenu, MENU_LCD);
   MenuSetDisplay(&analogueMenu, MENU_LCD);
   MenuSetDisplay(&digitalMenu, MENU_LCD);      
   /* Menu Setup */
   MenuSetInput(&primaryMenu, 0); 
	
	/* Menu must be Initialised first */
	/* Backlight 'off' is at 5% */
   UI_LCD_BLInit(5);
	UI_LCD_BL_On();
	
	aboutScroll(MAIN_SCREEN);
	SoftTimerStart( SoftTimer2[SC_LCD_BL_Period] );	
	

	
	_delay_ms(900);	
	UI_LCD_LoadDefaultChars();					  
   /* Reprint Menu */  
   MenuUpdate(&primaryMenu, RESET_MENU);   
   
   /* Enable interrupts */
   eint();  
   
   while(1)
   {     
      switch( ActiveProcess )
      {
         case PLAY_MODE:
            Play();
            //Benchmark();
            //BenchMarkCount++;
         break;
         
         case CONTROLLER_MODE:
				MidiControllerMode();
			break;
         
         case RECEIVE_SYSEX:
            ReceiveSysEx();
         break;
         

         
      }
   }
   
   return 0;

}

void Benchmark(void)
{
   uint8_t i = 0;
   uint8_t SelectedChannel = 0;
   uint16_t sample;
   
	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      
      SelectedChannel = ActiveChannels[i++];
      SensorChannel(SelectedChannel);
   	_delay_us(SensorSettings->CrosstalkDelay);

         /* Take a sample */
         sample = ADC12_Sample();
                           
         /* Obtain Peak */
      ObtainPeak(SelectedChannel, sample);
   }   
}

void MidiControllerMode(void)
{
   uint8_t i = 0;
   uint16_t sample;
   uint16_t conditionedSignal;
	   
	for( i = 0; i <= ANALOGUE_INPUTS; i++ )
	{
         SensorChannel(i);
         _delay_us(SensorSettings->CrosstalkDelay);

         /* Take a sample */
         sample = ADC12_Sample();
			
			/* If the difference is not due to noise */
			if( (sample > GetChannelThresh(i)) &&
				 ((sample > (GetLastSampleValue(i) + GetChannelThresh(i))) ||
				  (sample < (GetLastSampleValue(i) - GetChannelThresh(i)))))
			{
				         
				/* Determine the resulting MIDI code */
				conditionedSignal = GainFunction(i, sample - GetChannelThresh(i));
			
				if( conditionedSignal > 127 )
				{
					conditionedSignal = 127;       
				}
				
				

				if( conditionedSignal != GetLastMIDIValue(i))
				{				
					/* Ensure the MIDI code is sent */			
					ObtainPeak(i, sample);   
					SetLastSampleValue(i, sample); 
				}
			}
   }   
}


void Play(void)
{
   uint8_t i = 0;
   uint8_t SelectedChannel = 0;
   uint16_t sample;
   
	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      
      SelectedChannel = ActiveChannels[i++];
      
      if( !(RetriggerPeriod[ SelectedChannel ].timerEnable) )
      {
         SensorChannel(SelectedChannel);
         _delay_us(SensorSettings->CrosstalkDelay);

         /* Take a sample */
         sample = ADC12_Sample();
                           
         /* Obtain Peak */
         ObtainPeak(SelectedChannel, sample);
      } 
   }   
}


interrupt (USART0RX_VECTOR) usart0_rx(void)
{
   uint8_t buffer = U0RXBUF;


   static uint8_t channel = 0;
   
   /* Start the Aux Timer for any key press or UART input */
	TBCCTL2 |= (CCIE);
   
   switch( ActiveProcess )
   {
      case PLAY_MODE:
         if( buffer == '+' )
         {
            SoftTimer2[SC_AutoMenuUpdate].timeCompare++;
            UART_Tx(SoftTimer2[SC_AutoMenuUpdate].timeCompare);
         }
         
         if( buffer == '-' )
         {
            SoftTimer2[SC_AutoMenuUpdate].timeCompare--;
            UART_Tx(SoftTimer2[SC_AutoMenuUpdate].timeCompare);
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
      		UI_LCD_Pos(&PrimaryDisplay, 3, 10);
      		UI_LCD_Char(&PrimaryDisplay, 0);
      		UI_LCD_Char(&PrimaryDisplay, 1);
      		UI_LCD_Char(&PrimaryDisplay, 2);
      		UI_LCD_Char(&PrimaryDisplay, 3);		
      		UI_LCD_Char(&PrimaryDisplay, 4);
      		UI_LCD_Char(&PrimaryDisplay, 5);
      		UI_LCD_Char(&PrimaryDisplay, 6);
      		UI_LCD_Char(&PrimaryDisplay, 7);		
      	}
      break;
      
      case RECEIVE_SYSEX:
         ringbuffer_put( (RINGBUFFER_T*)&ReceiveBuffer, buffer);
      break;
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

      if( IntResult != KP_INVALID)
      {
         switch( ActiveProcess )
         {
            case PLAY_MODE:
               
            break;
            
            case CONTROLLER_MODE:
					/* If keys A,B,C are simultaneously pressed */
					if( IntResult == 0x78 )
					{
						IntResult = KP_BACK;
						break;
					}
					
					
					MIDI_KeypadOutput(UI_KP_ReturnID(IntResult));
					/* Reset Button States */
					UI_INT_IFG &= ~(UI_COLS);
      			UI_Activate();
					eint();
				return;
            
            /* Any key will cancel SysEx reception */
            case RECEIVE_SYSEX:
               ActiveProcess = PLAY_MODE;
            break;
         }
         
			UI_LCD_BL_On();
			/* Start the Aux Timer for any key press or UART input */
			TBCCTL2 |= (CCIE);
         MenuSetInput(ActiveMenu, IntResult);   
         MenuUpdate(ActiveMenu, RESET_MENU);  
			
	      /* Toggles the permanent state of the LCD BL */
	      if( IntResult == KP_5 )
	      {
	         LCD_BL_Mode ^= 1;
	         
	         /* Turn it off, but enable the timer */
	         if( LCD_BL_Mode == LCD_BL_TIMED )
	         {
	            UI_LCD_BL_Off();    
	         }
	         else
	         {
               /* Permanently ON */
					SoftTimerReset( SoftTimer2[SC_LCD_BL_Period] );
					SoftTimerStop( SoftTimer2[SC_LCD_BL_Period] );
					
					/* If no SoftTimer2's are enabled, then turn off the Timer2 module */
					if( !SoftTimer_IsTimer2Active() )
					{
                  /* Stop the Auxuliary Timer */
	               TBCCTL2 &= ~(CCIE);
               }
					
				}
	      }   
			
			if( LCD_BL_Mode == LCD_BL_TIMED )
			{
	         SoftTimerReset( SoftTimer2[SC_LCD_BL_Period] );
			   SoftTimerStart( SoftTimer2[SC_LCD_BL_Period] );
	      }						     
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

