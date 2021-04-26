#ifndef COMMON_H
#define COMMON_H

#include <base.h>

/* OUTPUT */

/*
 * Function: ll_buf_append
 * Appends a C string to the screen buffer
 *
 * Parameters:
 *  in - C string to append
 *
 * Returns:
 *  <lmds_status>
 */
enum la_status ll_buf_append(const char *in);

/*
 * Function: ll_buf_clear
 * Clears the screen buffer
 */
void ll_buf_clear(void);

/*
 * Function: ll_buf_len
 * Gets the size of the screen buffer
 */
size_t ll_buf_len(void);

/*
 * Function: ll_buf_write
 * Writes the buffer onto screen
 */
void ll_buf_write(void);

/*
 * Function: ll_curs_mov
 * Moves the cursor
 *
 * Parameters:
 *  pos - position to move cursor to
 *
 * Returns:
 *  <la_status>
 */
enum la_status ll_curs_mov(const struct screen_coord pos);

/*
 * Function: ll_curs_invis
 * Makes the cursor invisible
 */
void ll_curs_invis(void);

/*
 * Function: ll_curs_invis
 * Makes the cursor visible
 */
void ll_curs_vis(void);

/*
 * Function: ll_scr_getsize
 * Gets the screen size
 *
 * Returns:
 *  The screen size
 */
struct winsize ll_scr_getsize(void);

/*
 * Function: ll_scr_coords_check
 * Checks if the screen coordinates are out-of-bounds
 *
 * Parameters:
 *  coord - coordinates to check
 *
 * Returns:
 *  <la_status>
 */
enum la_status ll_scr_coords_check(struct screen_coord coord);

/* Don't use these two manually */
void ll_alt_scr_on(void);
void ll_alt_scr_off(void);

/* INPUT */

/*
 * Function: ll_stdin_nonblock
 * Sets input to nonblocking mode
 */
void ll_stdin_nonblock(void);

/*
 * Function: ll_stdin_block
 * Sets input to blocking mode
 */
void ll_stdin_block(void);

/*
 * Function: ll_stdin_getchar
 * Gets a character from stdin
 *
 * Returns:
 *  the read character
 */
char ll_stdin_getchar(void);

#endif /* COMMON_H */
