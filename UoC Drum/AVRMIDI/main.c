/* Name: main.c
 * Project: AVR MIDI on low speed USB
 * Author: Adrian Gin - Based off Martin Homuth-Rosemann from VUSB-MIDI
 * Creation Date: 2010-10-19
 */
 
 
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <string.h>
 
 
#include "usbdrv.h"
#include "oddebug.h"
#include "hardUART/hardUart.h"
#include "USBMIDI/USBMIDI.h"
#include "RingBuffer/ringbuffer.h"
#include "MIDICodes/MIDICodes.h"


 

 /* Needs to be a power of 2 */
#define MIDI_OUT_BUFFER    (128)
#define MIDI_OUT_MASK      (MIDI_OUT_BUFFER - 1)
#define SYS_COMMON_MSG (22)

/* Consider all the rest to be 1-Byte System Common Messages */
uint8_t MIDILookupTable[] PROGMEM =
{
   // Start of Voice Messages
   0x80, 3, 0x08,
   0x90, 3, 0x09,
   0xA0, 3, 0x0A,
   0xB0, 3, 0x0B,
   0xC0, 2, 0x0C,
   0xD0, 2, 0x0D,
   0xE0, 3, 0x0E,
   0x00,       
   // Start of System Common Messages
   0xF0, 3, 0x04,
   0xF1, 2, 0x02,
   0xF2, 3, 0x03,
   0xF3, 2, 0x02,
   0x00
};

uint8_t wMIDImsgCount = 0;
uint8_t rMIDImsgCount = 0; 

/* Hold enough data for a few messages,
 */
static usbMIDIMessage_t MIDImsgComplete[MIDI_OUT_BUFFER];

 
 
// This descriptor is based on http://www.usb.org/developers/devclass_docs/midi10.pdf
// 
// Appendix B. Example: Simple MIDI Adapter (Informative)
// B.1 Device Descriptor
//
static PROGMEM char deviceDescrMIDI[] = {	/* USB device descriptor */
	18,			/* sizeof(usbDescriptorDevice): length of descriptor in bytes */
	USBDESCR_DEVICE,	/* descriptor type */
	0x10, 0x01,		/* USB version supported */
	0,			/* device class: defined at interface level */
	0,			/* subclass */
	0,			/* protocol */
	8,			/* max packet size */
	USB_CFG_VENDOR_ID,	/* 2 bytes */
	USB_CFG_DEVICE_ID,	/* 2 bytes */
	USB_CFG_DEVICE_VERSION,	/* 2 bytes */
	1,			/* manufacturer string index */
	2,			/* product string index */
	0,			/* serial number string index */
	1,			/* number of configurations */
};

// B.2 Configuration Descriptor
static PROGMEM char configDescrMIDI[] = {	/* USB configuration descriptor */
	9,			/* sizeof(usbDescrConfig): length of descriptor in bytes */
	USBDESCR_CONFIG,	/* descriptor type */
	101, 0,			/* total length of data returned (including inlined descriptors) */
	2,			/* number of interfaces in this configuration */
	1,			/* index of this configuration */
	0,			/* configuration name string index */
	USBATTR_SELFPOWER,	/* attributes */
	USB_CFG_MAX_BUS_POWER / 2,	/* max USB current in 2mA units */

// B.3 AudioControl Interface Descriptors
// The AudioControl interface describes the device structure (audio function topology) 
// and is used to manipulate the Audio Controls. This device has no audio function 
// incorporated. However, the AudioControl interface is mandatory and therefore both 
// the standard AC interface descriptor and the classspecific AC interface descriptor 
// must be present. The class-specific AC interface descriptor only contains the header 
// descriptor.

// B.3.1 Standard AC Interface Descriptor
// The AudioControl interface has no dedicated endpoints associated with it. It uses the 
// default pipe (endpoint 0) for all communication purposes. Class-specific AudioControl 
// Requests are sent using the default pipe. There is no Status Interrupt endpoint provided.
	/* AC interface descriptor follows inline: */
	9,			/* sizeof(usbDescrInterface): length of descriptor in bytes */
	USBDESCR_INTERFACE,	/* descriptor type */
	0,			/* index of this interface */
	0,			/* alternate setting for this interface */
	0,			/* endpoints excl 0: number of endpoint descriptors to follow */
	1,			/* */
	1,			/* */
	0,			/* */
	0,			/* string index for interface */

// B.3.2 Class-specific AC Interface Descriptor
// The Class-specific AC interface descriptor is always headed by a Header descriptor 
// that contains general information about the AudioControl interface. It contains all 
// the pointers needed to describe the Audio Interface Collection, associated with the 
// described audio function. Only the Header descriptor is present in this device 
// because it does not contain any audio functionality as such.
	/* AC Class-Specific descriptor */
	9,			/* sizeof(usbDescrCDC_HeaderFn): length of descriptor in bytes */
	36,			/* descriptor type */
	1,			/* header functional descriptor */
	0x0, 0x01,		/* bcdADC */
	9, 0,			/* wTotalLength */
	1,			/* */
	1,			/* */

// B.4 MIDIStreaming Interface Descriptors

// B.4.1 Standard MS Interface Descriptor
	/* interface descriptor follows inline: */
	9,			/* length of descriptor in bytes */
	USBDESCR_INTERFACE,	/* descriptor type */
	1,			/* index of this interface */
	0,			/* alternate setting for this interface */
	2,			/* endpoints excl 0: number of endpoint descriptors to follow */
	1,			/* AUDIO */
	3,			/* MS */
	0,			/* unused */
	0,			/* string index for interface */

// B.4.2 Class-specific MS Interface Descriptor
	/* MS Class-Specific descriptor */
	7,			/* length of descriptor in bytes */
	36,			/* descriptor type */
	1,			/* header functional descriptor */
	0x0, 0x01,		/* bcdADC */
	65, 0,			/* wTotalLength */

// B.4.3 MIDI IN Jack Descriptor
	6,			/* bLength */
	36,			/* descriptor type */
	2,			/* MIDI_IN_JACK desc subtype */
	1,			/* EMBEDDED bJackType */
	1,			/* bJackID */
	0,			/* iJack */

	6,			/* bLength */
	36,			/* descriptor type */
	2,			/* MIDI_IN_JACK desc subtype */
	2,			/* EXTERNAL bJackType */
	2,			/* bJackID */
	0,			/* iJack */

//B.4.4 MIDI OUT Jack Descriptor
	9,			/* length of descriptor in bytes */
	36,			/* descriptor type */
	3,			/* MIDI_OUT_JACK descriptor */
	1,			/* EMBEDDED bJackType */
	3,			/* bJackID */
	1,			/* No of input pins */
	2,			/* BaSourceID */
	1,			/* BaSourcePin */
	0,			/* iJack */

	9,			/* bLength of descriptor in bytes */
	36,			/* bDescriptorType */
	3,			/* MIDI_OUT_JACK bDescriptorSubtype */
	2,			/* EXTERNAL bJackType */
	4,			/* bJackID */
	1,			/* bNrInputPins */
	1,			/* baSourceID (0) */
	1,			/* baSourcePin (0) */
	0,			/* iJack */


// B.5 Bulk OUT Endpoint Descriptors

//B.5.1 Standard Bulk OUT Endpoint Descriptor
	9,			/* bLenght */
	USBDESCR_ENDPOINT,	/* bDescriptorType = endpoint */
	0x1,			/* bEndpointAddress OUT endpoint number 1 */
	3,			/* bmAttributes: 2:Bulk, 3:Interrupt endpoint */ //This should be interrupt so the USB is slower than the UART.
	8, 0,			/* wMaxPacketSize */
	2,			/* bIntervall in ms */
	0,			/* bRefresh */
	0,			/* bSyncAddress */

// B.5.2 Class-specific MS Bulk OUT Endpoint Descriptor
	5,			/* bLength of descriptor in bytes */
	37,			/* bDescriptorType */
	1,			/* bDescriptorSubtype */
	1,			/* bNumEmbMIDIJack  */
	1,			/* baAssocJackID (0) */


//B.6 Bulk IN Endpoint Descriptors

//B.6.1 Standard Bulk IN Endpoint Descriptor
	9,			/* bLenght */
	USBDESCR_ENDPOINT,	/* bDescriptorType = endpoint */
	0x81,			/* bEndpointAddress IN endpoint number 1 */
	2,			/* bmAttributes: 2: Bulk, 3: Interrupt endpoint */ // This should be bulk so that it is faster than the UART
	8, 0,			/* wMaxPacketSize */
	2,			/* bIntervall in ms */
	0,			/* bRefresh */
	0,			/* bSyncAddress */

// B.6.2 Class-specific MS Bulk IN Endpoint Descriptor
	5,			/* bLength of descriptor in bytes */
	37,			/* bDescriptorType */
	1,			/* bDescriptorSubtype */
	1,			/* bNumEmbMIDIJack (0) */
	3,			/* baAssocJackID (0) */
};



 
 
 
uchar usbFunctionDescriptor(usbRequest_t * rq)
{

   if (rq->wValue.bytes[1] == USBDESCR_DEVICE) 
   {
      usbMsgPtr = (uchar *) deviceDescrMIDI;
      return sizeof(deviceDescrMIDI);
   } 
   else //(rq->wValue.bytes[1] == USBDESCR_CONFIG)  
   { /* must be config descriptor */
      
      usbMsgPtr = (uchar *) configDescrMIDI;
      return sizeof(configDescrMIDI);
   }

}
 
 
 
 
 
 
 
/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */
 
 
uchar usbFunctionSetup(uchar data[8])
{
   usbRequest_t *rq = (void *) data;

   if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) 
   { /* class request type */
 
      /*  Prepare bulk-in endpoint to respond to early termination   */
      if ((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_HOST_TO_DEVICE)
      {
      }
   }
   return 0xff;
}
 
 
 
 
/*---------------------------------------------------------------------------*/
/* usbFunctionRead                                                           */
/*---------------------------------------------------------------------------*/
/* We're never asked to send lots of data, so lets not send anything */
uchar usbFunctionRead(uchar * data, uchar len)
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	data[5] = 0;
	data[6] = 0;
	return 7;
   
}
 
 
 
 
/*---------------------------------------------------------------------------*/
/* usbFunctionWrite                                                          */
/*---------------------------------------------------------------------------*/
/* We don't expect any data so return 1 */
uchar usbFunctionWrite(uchar * data, uchar len)
{
   return 1;
}
 
 
 
 
/*---------------------------------------------------------------------------*/
/* usbFunctionWriteOut                                                       */
/*                                                                           */
/* this Function is called if a MIDI Out message (from PC) arrives.          */
/*                                                                           */
/*---------------------------------------------------------------------------*/
 
 
void usbFunctionWriteOut(uchar * data, uchar len)
{
   uint8_t byteCount = 0;

   /* Route it all to the UART port at 31250 baud */
   byteCount = usbMIDI_ParseData(data, len);

   /* Here if the TxBuffer cannot hold the new data, we should disable all 
    * USB requests and wait for the MIDI TxBuffer to catch up.
    * Since we need to process the next request, we have to disable BEFORE
    * the buffer will be full (hence the 16).
    *
    * We shall re-enable the request in the main loop, not the interrupt.    
   /* This is required if our endpoints are interrupt based, note that you can't
    * DisableAllRequests on a bulk transfer. */*/
   if( (ringbuffer_len( (RINGBUFFER_T*)&TransmitBuffer ) + 16) >= (TXBUFFER_SIZE-1) )
   {
      usbDisableAllRequests();
   }


   uartTxDump(data, byteCount);
   
   /* for internal loopback */
   /*for(uint8_t i = 0 ; i < byteCount ; i++ )
   {
      ringbuffer_put( (RINGBUFFER_T*)&ReceiveBuffer, data[i]);
   }*/

   /* This shouldn't happen since the USB output is quite fast (bulk) */
   /* This is required if our endpoints are interrupt based, note that you can't
    * DisableAllRequests on a bulk transfer. */
   if( ((wMIDImsgCount + 1) & MIDI_OUT_MASK) == ((rMIDImsgCount) & MIDI_OUT_MASK) )
   {
      usbDisableAllRequests();
   }
}
 

 



 

 
 
 
ISR(SIG_UART_RECV)
{
   uint8_t buffer = UDR;
   sei();
   if( ringbuffer_put( (RINGBUFFER_T*)&ReceiveBuffer, buffer) == BUFFER_OVERFLOW)
   {
   }
}
 
 
 
 
 
/* Given the first MIDI Message, we return the
 * corresponding index value in MIDILookupTable
 * Returns: UNKNOWN_VOICE_MSG or UNKNOWN_SYS_MSG;
 * if it does not exist in the table */
uint8_t LookupMIDIMessage(uint8_t MIDIStatusCode)
{
   uint8_t i;
   /* System Common Message */
   if( MIDIStatusCode >= 0xF0 )
   {
      for( i = SYS_COMMON_MSG; pgm_read_byte(&MIDILookupTable[i]) ; i = i + 3 )
      {
         if( MIDIStatusCode == pgm_read_byte(&MIDILookupTable[i]) )
         {
            return i;
         }
      }
      return UNKNOWN_SYS_MSG;
   }
   else
   {
      MIDIStatusCode = (MIDIStatusCode & 0xF0);
      for( i = 0; pgm_read_byte(&MIDILookupTable[i]) ; i = i + 3 )
      {
         if( MIDIStatusCode == pgm_read_byte(&MIDILookupTable[i]) )
         {
            return i;
         }
      }
      return UNKNOWN_VOICE_MSG;
   }
}
 
/* Returns the CIN number */
uint8_t GetCINNumber(uint8_t messageIndex)
{
   if( messageIndex == UNKNOWN_SYS_MSG)
   {
        return 0x05;
   }
   uint8_t CIN = pgm_read_byte(&MIDILookupTable[messageIndex+2]);
   /* Most system common messages are 1 byte long and
    * therefore have a CIN of 0x05 */
   return CIN;
}
 
 
uint8_t GetNumberOfBytesToRead(uint8_t messageIndex)
{
   if( messageIndex == UNKNOWN_SYS_MSG)
   {
        return 1;
   }
   uint8_t bytesToReturn = pgm_read_byte(&MIDILookupTable[messageIndex+1]);
   /* Most system common messages are 1 byte long */
   return bytesToReturn;
}
 
 
 

/* When MIDI Data is ready
 * The function will return MIDI_DATA_READY and the
 * USB_MIDI packet will be in MIDIMessage */
uint8_t MIDIDataReady(uint8_t inByte, usbMIDIMessage_t* MIDIMessage)
{
   /* You can technically have two midi events per midi message, but lets just
    * keep it to one.
    */
   static uint8_t byteCount = 0;
   static uint8_t bytesToReceive = 0;
   static uint8_t receivingSysEx = 0;
   static uint8_t CIN = 0;
   uint8_t messageIndex = 0;
 
   /* Check for input errors */
   if( (byteCount == 0) && (inByte <= MIDI_MAX_DATA) && (!receivingSysEx))
   {
      byteCount = 0;
      return MIDI_DATA_NOT_READY;
   }
   /* A new status byte is received, discard the old one */
   if( (byteCount > 0) && (inByte > MIDI_MAX_DATA) && (inByte != MIDI_SYSEX_STOP))
   {
      byteCount = 0;
      receivingSysEx = 0;
   }
 
 
   if( inByte == MIDI_SYSEX_START )
   {
       receivingSysEx = 1;
   }
 
   /* Determine how many bytes to receive based on the 'first'
    * byte (MIDI_Status Code) */
   if( byteCount == 0 )
   {
 
      /* Clear */
      MIDIMessage->header = 0;
      MIDIMessage->MIDIData[0] = 0;
      MIDIMessage->MIDIData[1] = 0;
      MIDIMessage->MIDIData[2] = 0;
 
      if( !receivingSysEx )
      {
          messageIndex = LookupMIDIMessage(inByte);
          bytesToReceive = GetNumberOfBytesToRead(messageIndex);
          CIN = GetCINNumber(messageIndex);
          MIDIMessage->header = CIN;
      }
   }
 
   MIDIMessage->MIDIData[byteCount] = inByte;
   byteCount++;
 
   if( receivingSysEx )
   {
      /* By default we are continuing a SysEx Message */
      bytesToReceive = 3;
      MIDIMessage->header = 0x04;
 
      /* If we recieve a SYSEX STOP then send the message */
      if( inByte == MIDI_SYSEX_STOP )
      {
         bytesToReceive = byteCount;
         MIDIMessage->header = byteCount + 0x04;
         receivingSysEx = 0;
      }
   }
 
   if( byteCount == bytesToReceive)
   {
      byteCount = 0;
      return MIDI_DATA_READY;
   }

   return MIDI_DATA_NOT_READY;
}
 
 
 
 

 
int main(void)
{
   uint8_t midiReady = 0;
   uint8_t outputBuffer[8];
   
   usbInit();
   uint8_t temp[] = {0x05, 0xF8, 0x00, 0x00, 0x05, 0xF8, 0x00, 0x00};
 
/* Just use 31250 baud */
/* Formula = ((Hz / Baud) / 16) - 1 */
   uartInit(39, 0);
 
   sei();

   /*while(1)
   {
      usbPoll();
       if( usbInterruptIsReady() )
       {
             usbSetInterrupt( (uint8_t*)temp, 8);
        }
   }*/


   while(1)
   { /* main event loop */
      
       usbPoll();

       /* To save the UART output buffer from overflowing, due to a
        * UART Speed < USB Inspeed (likely under bulk mode, but unlikely under interrupt mode */
       /* To save the USB output buffer from overflowing, due to a
        * UART Speed > USB Outspeed (not likely under bulk mode, but likely under interrupt mode */
       if( usbAllRequestsAreDisabled() && ((wMIDImsgCount+ 1) & MIDI_OUT_MASK) != ((rMIDImsgCount) & MIDI_OUT_MASK)
            && (ringbuffer_len( (RINGBUFFER_T*)&TransmitBuffer ) + 16) < (TXBUFFER_SIZE-1))
       {
         usbEnableAllRequests();
       }

      /* Process messages in the buffer is there are any */
       if( ringbuffer_len((RINGBUFFER_T*)&ReceiveBuffer) )
       {
          /* Only process this stuff if we have enough room in the MIDI out buffer */
          if( ((wMIDImsgCount + 1) & MIDI_OUT_MASK) != rMIDImsgCount)
          {
             uint8_t nextByte = ringbuffer_get((RINGBUFFER_T*)&ReceiveBuffer);
             midiReady = MIDIDataReady(nextByte, &MIDImsgComplete[wMIDImsgCount]);
             /* Copy it out, so the tempbuffer is ready again */
             if( midiReady == MIDI_DATA_READY)
             {
                wMIDImsgCount = (wMIDImsgCount + 1) & MIDI_OUT_MASK;
             }
          }
       }
 

       if( (rMIDImsgCount != wMIDImsgCount) && usbInterruptIsReady() )
       {
       /* If the USB Interrupt isn't ready, then we have an issue
        * of data coming from the UART quicker than it is going out, to keep up
        * we need to send 2 data events sometimes */
          if( ((rMIDImsgCount + 1) & MIDI_OUT_MASK) != ((wMIDImsgCount) & MIDI_OUT_MASK) )
          {
             memcpy(&outputBuffer, &MIDImsgComplete[rMIDImsgCount], sizeof(usbMIDIMessage_t));
             memcpy(&outputBuffer[4], &MIDImsgComplete[(rMIDImsgCount+1) & MIDI_OUT_MASK], sizeof(usbMIDIMessage_t));
             usbSetInterrupt( (uint8_t*)&outputBuffer, 8);
             rMIDImsgCount = (rMIDImsgCount + 2) & MIDI_OUT_MASK;
          }
          else
          {
             usbSetInterrupt( (uint8_t*)&MIDImsgComplete[rMIDImsgCount], 4);
             rMIDImsgCount = (rMIDImsgCount + 1) & MIDI_OUT_MASK;
          }
       }
       
       

              
   }
   return 0;
}
