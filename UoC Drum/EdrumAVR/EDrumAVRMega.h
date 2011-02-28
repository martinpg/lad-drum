#ifndef EDRUM_AVRMEGA_H
#define EDRUM_AVRMEGA_H

#include <stdint.h>

extern uint8_t ActiveProcess;
extern uint16_t BenchMarkCount;

extern const char VersionId[];

#define SET_BENCHMARK (1)
#define HAS_CONTROLLER_MODE (0)

enum {
   PLAY_MODE = 0,
   RECEIVE_SYSEX,
   CONTROLLER_MODE,
   FIRMWARE_UPGRADE
} processes;


#define DEFAULT_PROCESS (PLAY_MODE)

void Shutdown(void);
void Play(void);
void Benchmark(void);
void MidiControllerMode(void);

/* Interrupt Macros */
#define UI_INT_PORT  P1IN
#define UI_INT_PIN   (1<<3)


#endif
