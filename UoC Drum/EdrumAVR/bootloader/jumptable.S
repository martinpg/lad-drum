.section .jumps,"ax",@progbits

.global _jump_table
_jump_table:
   jmp usbPoll
   jmp usbFunctionWriteOut
   jmp usbSetInterrupt
   jmp uartTx
   jmp usbInit
   jmp USBMIDI_PutByte
   jmp USBMIDI_ProcessBuffer
   jmp USBMIDI_OutputData
   jmp USBMIDI_EnableRequests
   jmp USBMIDI_GetByte
