/* Ring buffer implementation */
#include <signal.h>
#include "ringbuffer.h"


/* Critical means disable interrupts on entry and restore interrupt 
 * state on exit */

critical int ringbuffer_get(RINGBUFFER_T *buffer) {
    //buffer not empty?
    if (buffer->fill) {
        //wrap around read position
        if(buffer->read_pos >= buffer->size) {
            buffer->read_pos = 0;
        }
        //update size info
        buffer->fill--;
        //get byte from buffer, update read position and return
        return buffer->memory[buffer->read_pos++];
    } else {
        return BUFFER_OVERFLOW; /* This is really trying to get a nonexistant byte */
    }
}


critical int ringbuffer_put(RINGBUFFER_T *buffer, char character) {
    //is there space in the buffer?
    if (buffer->fill < buffer->size) {
        //wrap around write position
        if(buffer->write_pos >= buffer->size) {
            buffer->write_pos = 0;
        }
        //write the character
        buffer->memory[buffer->write_pos++] = character;
        //update size info
        buffer->fill++;
        return buffer->fill;
    } else {
        return BUFFER_OVERFLOW;
    }
}

int ringbuffer_len(RINGBUFFER_T *buffer) {
    return buffer->fill;
}


critical void ringbuffer_clear(RINGBUFFER_T *buffer) {
    buffer->fill = 0;
    buffer->read_pos = 0;
    buffer->write_pos = 0;
}
