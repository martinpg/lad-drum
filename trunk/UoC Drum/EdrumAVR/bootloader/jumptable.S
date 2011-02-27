.section .jumps,"ax",@progbits

.global _jump_table
_jump_table:
   jmp usbPoll
   jmp usbFunctionWriteOut
   jmp usbSetInterrupt
   jmp bootloader_enter
   jmp usbInit
   jmp USBMIDI_PutByte
   jmp USBMIDI_OutputData
   jmp USBMIDI_EnableRequests
   jmp USBMIDI_GetByte
