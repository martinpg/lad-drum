#ifndef MIDI_CODES_H
#define MIDI_CODES_H


#define MIDI_
#define OCTAVE_OFFSET      (1)

#define MIDI_SYSTEM_COMMON_MSG (0xF0)


#define MIDI_DATA_READY     (0x01)
#define MIDI_DATA_NOT_READY (0x00)


#define UNKNOWN_SYS_MSG (0xFF)
#define UNKNOWN_VOICE_MSG (0xFE)
 

/* In 100us resolution */
#define MIDI_MAX_DATA   (127)
#define MIDI_MAX_KEY    (127)
#define MAX_MIDI_CHANNEL (0x0F)

enum {
   MIDI_NOTE_OFF = 0x80,
   MIDI_NOTE_ON = 0x90,
   MIDI_AFTERTOUCH = 0xA0,
   MIDI_CONTROL_CHANGE = 0xB0,
   MIDI_PROGRAM_CHANGE = 0xC0,
   MIDI_CHANNEL_PRESSURE = 0xD0,
   MIDI_PITCH_CHANGE = 0xE0,
   MIDI_SYSEX_START  =  0xF0,
   MIDI_TIME_CODE    = 0xF1,
   MIDI_SONG_POSITION =0xF2,
   MIDI_SONG_SELECT  = 0xF3,
   MIDI_SYSEX_STOP   = 0xF7,
   MIDI_TIMING_CLOCK = 0xF8
} MIDI_Commands;

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

enum {
	BANK_SELECT_MSB = 0,
	MODULATION_WHEEL,
	BREATH_CONTROLLER,
	FOOT_PEDAL = 4,
	PORTAMENTO_TIME_MSB,
	DATA_ENTRY_MSB,
	VOLUME_MSB,
	BALANCE_MSB,
	PAN_POSITION_MSB = 10,
	EXPRESSION_MSB,
	EFFECT_CONTROL1_MSB,
	EFFECT_CONTROL2_MSB,
	UNDEFINED14,
	UNDEFINED15,
	GENERAL_PURPOSE1,
	GENERAL_PURPOSE2,
	GENERAL_PURPOSE3,
	GENERAL_PURPOSE4,
	GENERAL_PURPOSE5,
	GENERAL_PURPOSE6,
	GENERAL_PURPOSE7,
	GENERAL_PURPOSE8,
	GENERAL_PURPOSE9,
	GENERAL_PURPOSE10,
	GENERAL_PURPOSE11,
	GENERAL_PURPOSE12,
	GENERAL_PURPOSE13,
	GENERAL_PURPOSE14,
	GENERAL_PURPOSE15,
	GENERAL_PURPOSE16,
	BANK_SELECT_LSB = 32,
	MODULATION_WHEEL_LSB,
	BREATH_CONTROLLER_LSB,
	FOOT_PEDAL_LSB = 36,
	PORTAMENTO_TIME_LSB,
	DATA_ENTRY_LSB,
	VOLUME_LSB,
	BALANCE_LSB = 42,
	PAN_POSITION_LSB,
	EXPRESSION_LSB,
	EFFECT_CONTROL1_LSB,
	EFFECT_CONTROL2_LSB,
	GENERAL_PURPOSE1_LSB,
	GENERAL_PURPOSE2_LSB,
	GENERAL_PURPOSE3_LSB,
	GENERAL_PURPOSE4_LSB,
	GENERAL_PURPOSE5_LSB,
	GENERAL_PURPOSE6_LSB,
	GENERAL_PURPOSE7_LSB,
	GENERAL_PURPOSE8_LSB,
	GENERAL_PURPOSE9_LSB,
	GENERAL_PURPOSE10_LSB,
	GENERAL_PURPOSE11_LSB,
	GENERAL_PURPOSE12_LSB,
	GENERAL_PURPOSE13_LSB,
	GENERAL_PURPOSE14_LSB,
	GENERAL_PURPOSE15_LSB,
	GENERAL_PURPOSE16_LSB,
	HOLD_PEDAL_TOGGLE,
	PORTAMENTO_TOGGLE,
	SUSTENUTO_PEDAL_TOGGLE,
	SOFT_PEDAL_TOGGLE,
	LEGATO_PEDAL_TOGGLE,
	HOLD_2_PEDAL_TOGGLE,
	SOUND_VARIATION,
	RESONANCE,
	SOUND_RELEASE_TIME,
	SOUND_ATTACK_TIME,
	FREQUENCY_CUTOFF,
	SOUND_CONTROL6,
	SOUND_CONTROL7,
	SOUND_CONTROL8,
	SOUND_CONTROL9,
	SOUND_CONTROL10,
	GENERAL_PURPOSE_BUTTON1,
	GENERAL_PURPOSE_BUTTON2,
	GENERAL_PURPOSE_BUTTON3,
	GENERAL_PURPOSE_BUTTON4,
	GENERAL_PURPOSE_BUTTON5,
	GENERAL_PURPOSE_BUTTON6,
	GENERAL_PURPOSE_BUTTON7,
	GENERAL_PURPOSE_BUTTON8,
	GENERAL_PURPOSE_BUTTON9,
	GENERAL_PURPOSE_BUTTON10,
	GENERAL_PURPOSE_BUTTON11,
	REVERB_LEVEL,
	TREMOLO_LEVEL,
	CHORUS_LEVEL,
	CELESTE_LEVEL,
	PHASER_LEVEL,
	DATA_BUTTON_INCREMENT,
	DATA_BUTTON_DECREMENT,
	NON_REGISTERED_PARAMETER_LSB,
	NON_REGISTERED_PARAMETER_MSB,
	REGISTERED_PARAMETER_LSB,
	REGISTERED_PARAMETER_MSB,
	ALL_SOUND_OFF = 120,
	ALL_CONTROLLERS_OFF,
	LOCAL_KEYBOARD_TOGGLE,
	ALL_NOTES_OFF,
	OMNI_MODE_OFF,
	OMNI_MODE_ON,
	MONO_OPERATION,
	POLY_OPERATION						
} CONTROL_CHANGE_CODES;


#endif
