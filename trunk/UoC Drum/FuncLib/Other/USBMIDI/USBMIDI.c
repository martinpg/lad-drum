
#include <stdint.h>
#include <string.h>
#include <USBMIDI/USBMIDI.h>


char MIDIResponseMap[] = {
   3, //CIN=0x0, MIDI Size = 1,2 or 3 (Misc)
   3, //CIN=0x1, MIDI Size = 1,2 or 3 (Cable Events)
   2, //CIN=0x2, MIDI Size = 2 (2 byte System Common Message)
   3, //CIN=0x3, MIDI Size = 3 (3 byte System Common Message)
   3, //CIN=0x4, MIDI Size = 3 (SysEx Starts / Continues)
   1, //CIN=0x5, MIDI Size = 1 (SysEx End Byte (7F) )
   2, //CIN=0x6, MIDI Size = 2 (SysEx 1 Byte + End )
   3, //CIN=0x7, MIDI Size = 3 (SysEx 2 Byte + End )
   3, //CIN=0x8, MIDI Size = 3 (Note Off)
   3, //CIN=0x9, MIDI Size = 3 (Note On)
   3, //CIN=0xA, MIDI Size = 3 (PolyKey Press)
   3, //CIN=0xB, MIDI Size = 3 (Control Change)
   2, //CIN=0xC, MIDI Size = 2 (Program Change)
   2, //CIN=0xD, MIDI Size = 2 (Channel Pressure)
   3, //CIN=0xE, MIDI Size = 3 (Pitchbend)
   1}; //CIN=0xF, MIDI Size = 1 (Single Byte Press)

/* This function extracts the MIDI messages from the USB MIDI messages */
/* Returns: number of bytes to send from *data */
uint8_t usbMIDI_ParseData(uint8_t* data, uint8_t len)
{
   uint8_t codeIndexNumber;
   uint8_t messageSize = 0;
   uint8_t byteCount = 0;
   uint8_t i = 0;

   for( i = 0 ; i < len ; i = i + sizeof(usbMIDIMessage_t) )
   {
      if( data[i] )
      {
         codeIndexNumber = data[i] & (0x0F);
         messageSize = MIDIResponseMap[codeIndexNumber];
         memcpy(data + byteCount, data+i+1 , messageSize);
         byteCount = byteCount + messageSize;
      }
   }

   return byteCount;
}



