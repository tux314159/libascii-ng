#ifndef IO_TOPLEVEL_H
#define IO_TOPLEVEL_H

#include <drivers/common.h>

/*
 * Function: iot_stdin_nonblock
 * Sets input to nonblocking mode
 */
void iot_stdin_nonblock(void);

/*
 * Function: iot_stdin_block
 * Sets input to blocking mode
 */
void iot_stdin_block(void);

/*
 * Function: iot_getchar
 * Gets a character from stdin
 *
 * Returns:
 *  the read character
 */
char iot_getchar(void);

/*
 * Function: iot_getstr_delim
 * Reads a string from stdin upto delim
 *
 * Parameters:
 *  delim - delimeter character
 *  buf - pointer to buffer to store read string
 *
 * Warning:
 *  This function will return if <max> characters
 *  are read. MAKE SURE THAT <max> IS STRICTLY LESS
 *  THAN THE SIZE OF THE BUFFER OR IT WIL OVERFLOW!
 */
void iot_getstr(const char delim, char *const buf, const size_t max);

#endif /* IO_TOPLEVEL_H */
