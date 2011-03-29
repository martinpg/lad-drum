/* These functions convert 32bit USB data to MIDI data */
#ifndef 	_USB_MIDI
#define	_USB_MIDI

#include <stdint.h>
#include "MIDICodes/MIDICodes.h"

/* To be defined in hardwareSpecific.h */
#ifndef __HARDWARE_SPECIFIC
#define PROGRAM_SPACE PROGMEM
#define FLASH_GET_PGM_WORD(address) pgm_read_word(address)
#define FLASH_GET_PGM_BYTE(address) pgm_read_byte(address)
#endif

 /* Needs to be a power of 2 */
#define MIDI_OUT_BUFFER    (8)
#define MIDI_OUT_MASK      (MIDI_OUT_BUFFER - 1)
#define SYS_COMMON_MSG (22)
#define NO_DATA_BYTE    (0xFF)

typedef struct usbMIDIMessage
{
   uint8_t  header;
   uint8_t  MIDIData[3];
} usbMIDIMessage_t;


/* Hold enough data for a few messages,
 */
extern usbMIDIMessage_t MIDImsgComplete[];
extern uint8_t wMIDImsgCount;
extern uint8_t rMIDImsgCount; 


/* Returns the number of USB-MIDIMessages in the buffer */
uint8_t usbMIDI_bufferLen(void);

/* Returns 1 is the buffer is Ready to accept new data*/
uint8_t usbMIDI_bufferIsReady(void);

/* Returns the number of bytes of MIDI data */
/* data: Parameter: Pointer to 1 or more USB MIDI Messages */
/* RAW Midi Data is put back into data */
uint8_t usbMIDI_ParseData(uint8_t* data, uint8_t len);

/* Given the first MIDI Message, we return the
 * corresponding index value in MIDILookupTable
 * Returns: UNKNOWN_VOICE_MSG or UNKNOWN_SYS_MSG;
 * if it does not exist in the table */
uint8_t LookupMIDIMessage(uint8_t MIDIStatusCode);

/* When MIDI Data is ready
 * The function will return MIDI_DATA_READY and the
 * USB_MIDI packet will be in MIDIMessage
 * This is the MIDI parser, just keep throwing bytes into this function */
uint8_t MIDIDataReady(uint8_t inByte, usbMIDIMessage_t* MIDIMessage);

/* Parameter: The result from 'LookupMIDIMessage' */
uint8_t GetNumberOfBytesToRead(uint8_t messageIndex);

/* Returns the CIN number, based on result from 'LookupMIDIMessage' */
uint8_t GetCINNumber(uint8_t messageIndex);



#endif