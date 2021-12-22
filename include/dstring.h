#ifndef DSTRING_H
#define DSTRING_H

#include <status.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Struct: string
 * A dynamic string.
 * Supports append, random insertion, random deletion and
 * on-demand resizing.
 * You may rather liberally modify the str array:
 * === Code
 * in->str[4] = 'H'
 * ===
 * But be careful that you do not write past the end of the buffer.
 */
struct string {
    /*
     * Property: len
     * Length of the vector.
     * Trying to access elements after this is
     * undefined behaviour.
     */
    size_t len;

    /*
     * Property: str
     * The C string used to store data
     */
    char *str;
};

/* Function: string_init
 * Initialises a string
 *
 * Parameters:
 *  in - string to operate on
 *
 * Returns:
 *  <la_status>
 */
enum la_status string_init(struct string *in);

/* Function: string_deinit
 * De-initialises a string
 *
 * Parameters:
 *  in - string to operate on
 *
 * Returns:
 *  <la_status>
 */
enum la_status string_deinit(struct string *in);

/* Function: string_grow
 * Grows a string
 *
 * Parameters:
 *  in - string to operate on
 *  add - how many characters to grow by
 *
 * Returns:
 *  <la_status>
 *
 * Note:
 *  The new space will be uninitialised, save for the final
 *  NULL byte.
 */
enum la_status string_grow(struct string *in, size_t add);

/* Function: string_append
 * Appends a C char array to a string
 * Parameters:
 *  in - string to operate on
 *  str - char array to append
 *
 * Returns:
 *  <la_status>
 *
 * Note:
 *  str must be NULL-terminated.
 */
enum la_status string_append(struct string *in, const char *str);

/* Function: string_insert
 * Inserts a C char array in a string _at_ the specified index.
 * All other characters, if any, will be pushed back.
 * Parameters:
 *  in - string to operate on
 *  idx - index to insert at
 *  str - char array to insert
 *
 * Returns:
 *  <la_status>
 *
 * Note:
 *  str must be NULL-terminated.
 */
enum la_status string_insert(struct string *in, size_t idx, const char *str);

/* Function: string_delete
 * Deletes characters from start to end inclusively.
 * Parameters:
 *  in - string to operate on
 *  start - index to start deleting from
 *  end - index to stop deleting at
 *
 * Returns:
 *  <la_status>
 */
enum la_status string_delete(struct string *in, size_t start, size_t end);

#endif /* DSTRING_H */
