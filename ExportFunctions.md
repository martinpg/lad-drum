# Introduction #

Accessing bootloader functions from the application area


# Details #

Sometimes you can save space if there are common functions between the bootloader and the application. For example, both the bootloader and the application require the USB driver so they can talk to the PC via USB.

These functions can be shared.

Functions from the bootloader were exported via ASM. It's like a DLL. tehehe.

The Assembler Jump Table (placed in the last part of the bootloader)

http://code.google.com/p/lad-drum/source/browse/trunk/UoC%20Drum/EdrumAVR/bootloader/jumptable.S

The exported functions
[http://code.google.com/p/lad-drum/source/browse/trunk/UoC%20Drum/EdrumAVR/edrumavrsharedfunctions.h ]

Note that the SRAM of the application needs to start where the bootloader SRAM ends otherwise, we'll get memory writing errors. We can do this by adding in the linker the line:

-Wl,-section-start=.data=0x8000ff

This means the application heap will start at 0xFF. Instead of 0x60, so the bootloader and the application get their own memory areas. Since the stack is shared, there is no problem for Automatic variables.