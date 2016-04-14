# Introduction #

Currently, the MIDI configuration is stored in FLASH.

# Details #

MIDI Configuration shall be stored into the EEPROM.
This will save about 600bytes of flash.

Some devices such as MSPs and PIC do not have EEPROM.
Hopefully they will have more flash however.

But for the AVR port, the settings shall be stored in EEPROM.