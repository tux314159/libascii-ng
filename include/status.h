#ifndef STATUS_H
#define STATUS_H

/* Enum: la_status
 * Enumerates the return values of functions
 *
 * OK - OK
 * SCR_OOB - Co-ordinates are out-of-bounds
 */
enum la_status {
    LASCII_OK = 0,
    LASCII_SCR_OOB,
    LASCII_STR_OOB,
    LASCII_ALLOC_FAIL,
    LASCII_NULLPTR
};

/* Related error-checking macros */
#define RETIFNOK(val) if (val != LASCII_OK) return val;
#define NULLCHK(in) if (in == NULL) return LASCII_NULLPTR;

#endif /* STATUS_H */
