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
#define BAUD_1M			(0x0008)


#define DEFAULT_BAUD_RATE	BAUD_31250
#define MAX_MIDI_CHANNEL (0x0F)


extern const char MIDI_NOTES[];
extern const char MIDI_BAUD[][11];

enum {
	B31250 = 0,
	B38400,
	B115200,
	B1M
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

enum {
   NO_ITEM = 0,
   ACOUSTIC_BASS_DRUM = 35,
   BASS_DRUM1,
   SIDE_STICK,
   ACOUSTIC_SNARE,
   HAND_CLAP,
   ELECTRIC_SNARE,//40
   LOW_FLOOR_TOM, 
   CLOSED_HI_HAT,
   HIGH_FLOOR_TOM,
   PEDAL_HIGH_HAT,
   LOW_TOM,
   OPEN_HIGH_HAT,
   LOW_MID_TOM,
   HI_MID_TOM,
   CRASH_CYMBAL1,
   HIGH_TOM,
   RIDE_CYMBAL1,
   CHINESE_CYMBAL,
   RIDE_BELL,
   TAMBOURINE,
   SPLASH_CYMBAL,
   COWBELL,
   CRASH_CYMBAL2,
   VIBRASLAP,
   RIDE_CYMBAL2,
   HI_BONGO,
   LOW_BONGO,
   MUTE_HI_CONGA,
   OPEN_HI_CONGA,
   LOW_CONGA,
   HIGH_TIMBALE,
   LOW_TIMBALE,
   HIGH_AGOGO,
   LOW_AGOGO,
   CABASA,
   MARACAS,
   SHORT_WHISTLE,
   LONG_WHISTLE,
   SHORT_GUIRO,
   LONG_GUIRO,
   CLAVES,
   HI_WOOD_BLOCK,
   LOW_WOOD_BLOCK,
   MUTE_CUICA,
   OPEN_CUICA,
   MUTE_TRIANGLE,
   OPEN_TRIANGLE // 83
} GENERAL_MIDI_PERCUSSION;

typedef struct {

	/* In 100us resolution */
   uint16_t  MIDI_OutputRate;
   uint16_t  MIDI_BaudRate;
   uint16_t	 MIDI_ChannelCode;
	
} MidiSettings_t;


extern MidiSettings_t MIDISettings;

void MIDI_Output(void);
void MIDI_DigitalOutput(void);
void MIDI_MetronomeOutput(void);

uint16_t MIDI_GetRate(void);
void MIDI_SetRate(uint16_t newRate);

uint16_t MIDI_GetBaud(void);
void MIDI_SetBaud(uint16_t newBaud);


uint8_t MIDI_GetChannelCode(void);
void MIDI_SetChannelCode(uint8_t newCode);

void MIDI_NoteString(uint8_t note, char* buffer);

uint8_t MIDI_Octave(uint8_t note);


#endif
