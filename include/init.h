#ifndef INIT_H
#define INIT_H

#include <drivers/common.h>
#include <drivers/vt100.h>
#include <drivers/xterm.h>
#define _LA_LL_ABSTRACTED
#include <base.h>

/* Function: ll_init
 * Initialises stuff. Must be run before any other libascii
 * functions are called.
 *
 * Returns:
 *  <la_state>
 *  LASCII_ALLOC_FAIL if memory allocation fails
 *  LASCII_OK otherwise
 */
enum la_status ll_init(void);

/* Function: lascii_deinit
 * De-initialises stuff. Do not call any other libascii
 * functions after this.
 */
void ll_deinit(void);

#endif /* INIT_H */
