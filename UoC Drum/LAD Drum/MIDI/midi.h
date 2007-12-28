#ifndef MIDI_H
#define MIDI_H


#include <stdint.h>

#define MIDI_NOTE_OFF		(0x80)
#define MIDI_NOTE_ON		   (0x90)
#define OCTAVE_OFFSET      (1)

/* In 100us resolution */
#define MAX_MIDI_RATE   (5000)
#define MIDI_MAX_KEY    (127)
#define BAUD_31250      (0x0100)
#define BAUD_38400      (0x00D0)
#define BAUD_115200		(0x0045)


#define DEFAULT_BAUD_RATE	BAUD_38400
#define MAX_MIDI_CHANNEL (0x10)


extern const char MIDI_NOTES[];
extern const char MIDI_BAUD[][8];

enum {
	B31250 = 0,
	B38400,
	B115200
} MIDIbauds;

enum {
   MN_C = 0,
   MN_CS,
   MN_D,
   MN_DS,
   MN_E,
   MN_F,
   MN_FS,
   MN_G,
   MN_GS,
   MN_A,
   MN_AS,
   MN_B,
   NOTE_COUNT
} MIDI_KEYS;



typedef struct {

	/* In 100us resolution */
   uint16_t  MIDI_OutputRate;
   uint16_t  MIDI_BaudRate;
   uint16_t	 MIDI_ChannelCode;
	
} MidiSettings_t;


extern MidiSettings_t MIDISettings;

void MIDI_Output(void);

uint16_t MIDI_GetRate(void);
void MIDI_SetRate(uint16_t newRate);

uint16_t MIDI_GetBaud(void);
void MIDI_SetBaud(uint16_t newBaud);


uint8_t MIDI_GetChannelCode(void);
void MIDI_SetChannelCode(uint8_t newCode);

void MIDI_NoteString(uint8_t note, char* buffer);

uint8_t MIDI_Octave(uint8_t note);


#endif
