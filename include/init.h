#ifndef INIT_H
#define INIT_H

#include <base.h>
#include <drivers/common.h>
#include <drivers/vt100.h>
#include <drivers/xterm.h>

/* Function: lascii_init
 * Initialises stuff. Must be run before any other libascii
 * functions are called.
 *
 * Returns:
 *  <la_state>
 *  LASCII_ALLOC_FAIL if memory allocation fails
 *  LASCII_OK otherwise
 */
enum la_status lascii_init(void);

/* Function: lascii_deinit
 * De-initialises stuff. Do not call any other libascii
 * functions after this.
 */
void lascii_deinit(void);

#endif /* INIT_H */
