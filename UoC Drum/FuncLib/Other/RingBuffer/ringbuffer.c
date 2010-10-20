/* Ring buffer implementation */

#include "ringbuffer.h"


/* Critical means disable interrupts on entry and restore interrupt 
 * state on exit */

int ringbuffer_get(RINGBUFFER_T *buffer) {
    //buffer not empty?
    if (buffer->read_pos != buffer->write_pos) {
        //wrap around read position
        buffer->read_pos &= (buffer->size - 1);
        //get byte from buffer, update read position and return
        return buffer->memory[buffer->read_pos++];
    } else {
        return BUFFER_OVERFLOW; /* This is really trying to get a nonexistant byte */
    }


}


int ringbuffer_put(RINGBUFFER_T *buffer, char character) {
    //is there space in the buffer?
    int bufferMask = (buffer->size - 1);

    if ( ((buffer->write_pos + 1) & bufferMask) != (buffer->read_pos & bufferMask)) {
        //wrap around write position
        buffer->write_pos &= bufferMask;

        //write the character
        buffer->memory[buffer->write_pos++] = character;
        //update size info

        return ringbuffer_len(buffer);
    } else {
        return BUFFER_OVERFLOW;
    }
}

unsigned int ringbuffer_len(RINGBUFFER_T *buffer) {
   
    if( buffer->read_pos > buffer->write_pos )
    {
        return buffer->write_pos + buffer->size - buffer->read_pos;
    }
    return buffer->write_pos - buffer->read_pos;
}


void ringbuffer_clear(RINGBUFFER_T *buffer) {
    buffer->read_pos = 0;
    buffer->write_pos = 0;
}
