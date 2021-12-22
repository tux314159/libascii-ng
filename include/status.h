#ifndef STATUS_H
#define STATUS_H

/* Enum: la_status
 * Enumerates the return values of functions
 */
enum la_status
{
    LASCII_OK         = -1,
    LASCII_STR_OOB    = -3,
    LASCII_ALLOC_FAIL = -4,
    LASCII_NULLPTR    = -5
};

/* Related error-checking macros */
#define RETIFNOK(val)            \
    if (val != LASCII_OK) {      \
        return val;              \
    } /* ...we assume you do     \
         sane things with status \
         codes                  */
#define NULLCHK(in)            \
    if (in == NULL) {          \
        return LASCII_NULLPTR; \
    }

#endif /* STATUS_H */
