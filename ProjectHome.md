Latest:
http://www.youtube.com/watch?v=00Bwia-GhpE

New Ideas:
To Do:
Digital Inputs to work with invisible states.
Change MIDI output so we only output data relevant to the Channel Command
One day to add a MIDI channel monitor

27/02/2011
http://code.google.com/p/lad-drum/wiki/USBMIDI?ts=1298777605&updated=USBMIDI

26/02/2011
Functions from the bootloader were exported via ASM.
It's like a DLL. tehehe.

The Assembler Jump Table (placed in the last part of the bootloader)
http://code.google.com/p/lad-drum/source/browse/trunk/UoC%20Drum/EdrumAVR/bootloader/jumptable.S

The exported functions
http://code.google.com/p/lad-drum/source/browse/trunk/UoC%20Drum/EdrumAVR/edrumavrsharedfunctions.h

23/02/2011

It is working well. The bootloader part via SysEx.

Firstly, I with the entire Menu System, I'm not sure if you've seen the code, but the way the user functions are implemented aren't that elegant.

It uses up quite a bit of memory. At the moment, it nicely fits on a ATMEGA32 which has 16 button keypad as input.

I think your board has only 4 key inputs (which is enough), but it would mean significant changes to the code to make it work.

At the moment. It works like this.

If the state is Change Settings
> if keypress == 1 do func1
> if keypress == 2 do func2
> if keypress == 3 do func3
....
> if keypress == 16 do func16
etc

If the state is Edit MIDI rate
> if keypress == 1 do Xfunc4
> if keypress == 2 do Xfunc5
> if keypress == 3 do Xfunc6
else do nothing

What I propose is to change the way the menu functions are interfaced with, and make it into a big lookup table system. So it would look like

{ChangeSettings, func1, func2....., func16}
{EditMidi, Xfunc4, Xfun5, Xfun6, 0, 0, 0}

I think this would save a lot of memory.




Latest News: 25/10/10
This project for MSP430 has now stopped development.

The project is now being ported to the ATMEGA32. Hopefully it can be somewhat compatible with the MegaDrum hardware.

Updates to come.



Electronic Drum kit software used to convert mechanical signals to electrical signals. These electrical signals will then be converted to lightning arc drawings.

Photos
of EDrum & AVR MIDI
http://picasaweb.google.com/adrian.gin/EDrums#

Youtube
http://www.youtube.com/watch?v=w_LkwHiHXw8

To Do:
Edrum -
1. Make Profile 1 start up by default (Easy)
2. Improve SysEx ROM usage (Medium)
3. Enable firmware upgrade via SysEx, not just SysEx data  (Hard :( )

However, doing firmware upgrade via SysEx, although possible, it's just not as simple on the MSP as the AVR. There is soo much support for the AVR code examples etc... I'm wondering if it's worth it...

Perhaps I can make a MegaDrum compatible AVR Drum project and switch my code from MSP to AVR and finally call it a day.

It's strange how the fun is in the implementation and getting it to work right as opposed to actually playing the drums. **big sigh**.

22/10/10 Update:
AVRMIDI now supports Running Status and RealTime messages just thrown into the Mix willy nilly.

Suggestions are most welcome!! Thanks!



19/10/2010 Update:
USB MIDI to be incorporated using an external ATMEGA chip. The next version of the LAD drum will be based off an ATMEGA32. (Maybe single chip, USB->MIDI conversion takes up quite a bit of CPU time.

Also going to improve the ROM usage of the profiles as well as make Profile1 start up by default so your changes are kept upon start up.


To Do: Improve the signal speeds by pulling the inputs down low after a successful read to reset the electrical capacitance. Result is that it's best to refine the retriggers, thresholds, gains and the crosstalk delay time.

2/5/10 Update: This project is now considered complete. This module is compatible with GuitarRig as it is a midi controller.

Congratulations on all contributors on a job complete!!
- Dane Rumble
- NZ Music Month
- The Naked and Famous
- Black Eyed Peas The E.N.D
- Yea yea, the typical David Guetta and the 'You got me' song!

Adrian