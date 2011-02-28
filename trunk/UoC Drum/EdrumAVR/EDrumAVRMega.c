
#include "EDrumAVRMega.h"
#include "hardwareSpecific.h" 
#include "MIDI/SysEx/SysEx.h"
#include "UI_LCD/UI_LCD.h"
#include "UI_KP/UI_KP.h"
#include "LCDSettings.h"
#include "MenuSettings.h"
#include "Profiles/profiles.h"
#include "UserFunctions/userfunctions.h"

#include "SPI/spi.h"
#include "VUmeter/vumeter.h"
#include "Sample/sample.h"
#include "Sensor/sensor.h"
#include "TimerCallbacks/TimerCallbacks.h"

#include "hardUart/hardUart.h"
#include "avrADC/adc.h"

#include "mmculib/uint8toa.h"
#include "edrumAVRsharedfunctions.h"
#include "USBMIDI/USBMIDI.h"

#include <stdlib.h>

const prog_char VersionId[] = "1.5 28/02/11";

uint16_t BenchMarkCount = 0;

uint8_t ActiveProcess = 0;

/* 20/02/2011
   Welcome to 2011 Edrum AVR Mega

   Flash Memory usage is nearing 100%, consider moving the profiles to EEPROM.
   Completed initial version, which somewhat works.

   Make Profile1 load up by default.
   Try to reduce the amount of Flash used.

   Incorporate a bootloader via SYS-EX.

*/



/* 19/10/2010 
   Update: USB MIDI to be incorporated using an external ATMEGA chip. 
   The next version of the LAD drum will be based off an ATMEGA32. 
   (Maybe single chip, USB->MIDI conversion takes up quite a bit of CPU time.
   
   Also going to improve the ROM usage of the profiles as well as make Profile1 
   start up by default so your changes are kept upon start up.

*/


int main(void)
{
   DDRD &= ~(1 << 3);
   PORTD &= ~(1<<3);


   MCUCSR = (1 << JTD);
   MCUCSR = (1 << JTD);

   _delay_ms(100);

   ProfileInit();
   
   /* Make profile 1 the default profile on start up */
   Profile_Read(1);
   
   SPI_Init();
   SensorInit();
   SensorInputSelect(GetSensorInput());
   DigitalInputInit();
   /* Setup the communications module */   
   UART_Init(0);
   uartSetBaud(0,39);

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

   /*Activate Interrupt */
   MCUCR |= ((1 << ISC11) | (1 << ISC10));
   GICR |= (1 << INT1);

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
   SoftTimerStart( SoftTimer2[SC_LCD_BL_Period] );	
	
   _delay_ms(900);
   _delay_ms(900);	
   UI_LCD_LoadDefaultChars();					  
   /* Reprint Menu */  
   MenuUpdate(&primaryMenu, RESET_MENU);

   usbInit();

   sei();

   /* Flush the buffer */
   UI_KP_GetPress();
   uint8_t inByte;

   while (1)
   {   
      usbPoll();
      USBMIDI_OutputData();

      switch( ActiveProcess )
      {
         case PLAY_MODE:
            Play();



            /*Benchmark();
            BenchMarkCount++;*/
         break;
             
         case RECEIVE_SYSEX:
            inByte = USBMIDI_GetByte();
            if(inByte != NO_DATA_BYTE)
            {
               ringbuffer_put( (RINGBUFFER_T*)&ReceiveBuffer, inByte);
            }

            if( ringbuffer_len((RINGBUFFER_T*)&ReceiveBuffer)  )
            {
               ParseSysExData(ringbuffer_get((RINGBUFFER_T*)&ReceiveBuffer));
            }
         break;

         case FIRMWARE_UPGRADE:
            Shutdown();
            cli();
            //ReceiveFirmware();
            
            bootloader_enter();
         break;
      }      
   }

   return 0;

}

void Shutdown(void)
{
   DISABLE_KEYPAD();
   DISABLE_PRIMARY_TIMER();
   DISABLE_AUXILIARY_TIMER();
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
         sample = ADC_Sample();
                        
         /* Obtain Peak */
         ObtainPeak(SelectedChannel, sample);
      } 
   }
}

#if SET_BENCHMARK
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
      sample = ADC_Sample();
                           
      /* Obtain Peak */
      ObtainPeak(SelectedChannel, sample);

   }   
}
#endif



ISR(SIG_SPM_READY)
{
}

ISR(SIG_UART_RECV)
{
   uint8_t buffer;

   buffer = UDR;
   sei();

/* Echo the data back out */
   //USBMIDI_PutByte(buffer);
   //uartTx(buffer);

   switch( ActiveProcess )
   {
      case PLAY_MODE:
         if( buffer <= '3' && buffer >= '0' )
         {
            buffer = buffer - '0';
            SetGainType(0x0D, NON_LINEAR_GAIN);
            SetChannelGain(0x0D, PresetGain1[buffer] );
      		SetSlope2Gain(0x0D, PresetGain2[buffer] );
      		SetCrossover(0x0D, PresetGainCrossover[buffer]);
         }        

         if( buffer == 'u' )
         {
            SoftTimerStop(SoftTimer2[SC_Debug]);
         }


      break;
      
      case RECEIVE_SYSEX:
         ringbuffer_put( (RINGBUFFER_T*)&ReceiveBuffer, buffer);
      break;

      case FIRMWARE_UPGRADE:
//         ParseFirmwareData(buffer);
      break;
   }
}


// This must be declared "naked" because we want to let the
// bootloader function handle all of the register push/pops
// and do the RETI to end the handler.
void INT0_vect(void) __attribute__((naked));
ISR(INT0_vect, ISR_NOBLOCK)
{
    asm("jmp 0x7004");
}



ISR(INT1_vect, ISR_NOBLOCK)
{
   DISABLE_KEYPAD();

   char outputString[5];
   static uint8_t j;
   uint8_t result, result2;

   result = UI_KP_GetPress();
   _delay_ms(30);
   result2 = UI_KP_GetPress();

   //_delay_ms(40);

   if( result == result2 )
   {
      if( result != KP_INVALID  )
      {
         MenuSetInput(ActiveMenu, result);
         DISABLE_KEYPAD();
         sei();
         MenuUpdate(ActiveMenu, RESET_MENU);

         /* Active the back light */
         ENABLE_AUXILIARY_TIMER();
         UI_LCD_BL_On();
         SoftTimerStart(SoftTimer2[SC_LCD_BL_Period]);
      }
   }
   
   GIFR |= (1 << INTF1);
   ENABLE_KEYPAD();
}



ISR(TIMER2_COMP_vect, ISR_NOBLOCK)
{
   OCR2 += (SAMPLE_1MS);
   RunCriticalTimer(); 
}


ISR(TIMER0_COMP_vect, ISR_NOBLOCK)
{  
   OCR0 += (SAMPLE_1MS);
   RunAuxTimers();
}


