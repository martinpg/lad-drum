# Introduction #

Some issues regarding USB MIDI.


# Details #

27/02/2011 The bootloader now has the USB MIDI driver in it. Since Win7 and newer OSes don't support Bulk Transfers on low speed devices, I've had to change it to use interrupt transfers (which are about 8x slower :( )

The effects of this are:

#1 UART data comes in faster than the USB can deliver it.

#2 USB Data coming from the PC will most likely always be processed without any buffer overflows.

#3 Control Flow can be disabled.

#4 USB data which comes in gets placed in a queue, if no-one 'gets' it, newer data will overwrite the queue.

The most annoying effect is #1. This is because when the Unit Dumps data such as Firmware Dumps / Configuration Dumps, it will overflow the buffer since the USB is not going to be receiving it very fast.

As a result, there needs to be a check each time you put in a byte, to make sure that if there is an overflow condition, we have to wait in a while loop attempting to send data.