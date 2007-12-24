#ifndef MAIN_H
#define MAIN_H

#include "Profiles/profiles.h"

/* Interrupt Macros */

#define UI_INT_PORT  P1IN
#define UI_INT_PIN   (1<<3)
#define DEFAULT_BAUD_RATE	BAUD_31250


extern const Profile_t Default_Profile;



#endif
