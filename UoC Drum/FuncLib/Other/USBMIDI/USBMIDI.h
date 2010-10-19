/* These functions convert 32bit USB data to MIDI data */



#ifndef 	_USB_MIDI
#define	_USB_MIDI

#include <stdint.h>

extern char MIDIResponseMap[];



typedef struct usbMIDIMessage
{
   uint8_t  header;
   uint8_t  MIDIData[3];
} usbMIDIMessage_t;




/* Returns the number of bytes of MIDI data */
/* data: Parameter: Pointer to 1 or more USB MIDI Messages */
/* RAW Midi Data is put back into data */
uint8_t usbMIDI_ParseData(uint8_t* data, uint8_t len);





#endif
