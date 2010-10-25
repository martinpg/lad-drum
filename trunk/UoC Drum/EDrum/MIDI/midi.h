#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>
#include "MIDICodes/MIDICodes.h"


/* In 100us resolution */
#define MAX_MIDI_RATE   (5000)

/*#if F_CPU == 8000000
#define BAUD_31250      (0x0100)
#define BAUD_38400      (0x00D0)
#define BAUD_115200		(0x0045)
#define BAUD_1M			(0x0008)
#else*/
#define BAUD_31250      (F_CPU / 31250)
#define BAUD_38400      (F_CPU / 38400)
#define BAUD_115200		(F_CPU / 115200)
#define BAUD_1M			(F_CPU / 1000000)
//#endif


#define DEFAULT_BAUD_RATE	BAUD_31250
#define MAX_MIDI_CHANNEL (0x0F)
#define MIDI_MANUFACTURER (0x7D) /* Educational Use */
#define MIDI_DEVICE_CODE       (0x01)  /* LAD Drum */

extern const char MIDI_NOTES[];
extern const char MIDI_BAUD[][11];

enum {
	B31250 = 0,
	B38400,
	B115200,
	B1M
} MIDIbauds;



typedef struct {

	/* In 100us resolution */
   uint16_t  MIDI_OutputRate;
   uint16_t  MIDI_BaudRate;
   uint16_t	 MIDI_ChannelCode;
	
} MidiSettings_t;


extern MidiSettings_t* MIDISettings;

void MIDI_Output(void);
void MIDI_DigitalOutput(void);
void MIDI_MetronomeOutput(void);
void MIDI_KeypadOutput(uint8_t kpButton);

uint16_t MIDI_GetRate(void);
void MIDI_SetRate(uint16_t newRate);

uint16_t MIDI_GetBaud(void);
void MIDI_SetBaud(uint16_t newBaud);


uint8_t MIDI_GetChannelCode(void);
void MIDI_SetChannelCode(uint8_t newCode);

void MIDI_NoteString(uint8_t note, char* buffer);

uint8_t MIDI_Octave(uint8_t note);


void SetLastMIDIValue(uint8_t channel, uint8_t value);
uint8_t GetLastMIDIValue(uint8_t channel);

#endif
