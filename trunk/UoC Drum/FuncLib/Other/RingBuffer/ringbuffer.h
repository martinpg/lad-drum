#ifndef MSPGCC_RINGBUFFER_H
#define MSPGCC_RINGBUFFER_H

/**
 * Descriptor for a ringbuffer.
 * 
 * To use a ringbuffer, initialize the first two fields and zero the
 * remaining fields.
 * 
 * E.g. 
 *      char ringbuf_memory[40];
 *      RINGBUFFER_T ringbuffer = {ringbuf_memory, sizeof(ringbuf_memory)};
 */
typedef struct {
    char *memory;                       ///< memory used for the buffer itself
    unsigned int size;                  ///< bytes used for the buffer
    volatile unsigned int fill;         ///< number of bytes currently in the buffer
    volatile unsigned int read_pos;     ///< reading position
    volatile unsigned int write_pos;    ///< writing position
} RINGBUFFER_T;

/**
 * This macro allocates an new ringbuffer and initializes the descriptor.
 * NOTE: This macro allocates global variables.
 */
#define RINGBUFFER_NEW(name, size) \
    static char ringmem##name[size]; \
    RINGBUFFER_T name = {ringmem##name, (size)};

/**
 * This macro creates an "extern" declaration for the ringbuffer.
 * For use in header files.
 */
#define RINGBUFFER_EXTERN(name) extern RINGBUFFER_T name;

/**
 * Check the number of bytes in the ringbuffer.
 * 
 * @param buffer        [in] The ringbuffer descriptor
 * @return the number of bytes in the buffer
 */
int ringbuffer_len(RINGBUFFER_T *buffer);

/**
 * Clear the ringbuffer (setting its length to zero).
 * 
 * @param buffer        [in] The ringbuffer descriptor
 */
void ringbuffer_clear(RINGBUFFER_T *buffer);

/**
 * Put a character in the ringbuffer. 
 *
 * @param buffer        [in] The ringbuffer descriptor
 * @param character     [in] Byte to store.
 * @return              -1 if the buffer is full, the number of bytes in the
 *                      buffer otherwise.
 */
int ringbuffer_put(RINGBUFFER_T *buffer, char character);

/**
 * Get a character from the ringbuffer. Handle wrap arounds.
 *
 * @param buffer        [in] The ringbuffer descriptor
 * @return              Byte from the buffer, -1 if the buffer is empty
 */
int ringbuffer_get(RINGBUFFER_T *buffer);

#endif //MSPGCC_RINGBUFFER_H
