#ifndef RENDER_H
#define RENDER_H

#include <string.h>
#include <base.h>
#include <drivers/common.h>

/*
 * Function: rr_curs_mov
 * Moves the virtual renderer cursor. This only affects
 * what the frame looks like after rendering, and does not
 * affect the positioning of anything.
 * Returns:
 *  <la_status>
 */
enum la_status rr_curs_mov(struct screen_coord pos);

/*
 * Function: rr_curs_vis
 * Makes the cursor visible
 */
void rr_curs_vis(void);

/*
 * Function: rr_curs_invis
 * Makes the cursor invisible
 */
void rr_curs_invis(void);

/*
 * Function: rr_scr_putc
 * Writes a character code to a cell on the screen
 *
 * Parameters:
 *  in - what character sequence to write
 *  pos - where to write it
 *
 * Returns:
 *  <la_status>
 */
void rr_scr_putc(char in, struct screen_coord pos);

/*
 * Function: rr_scr_puts
 * Writes a string to the screen
 *
 * Parameters:
 *  in - what character sequence to write
 *  pos - where to write it (from the first character)
 *
 * Returns:
 *  <la_status>
 */
void rr_scr_puts(const char *in, struct screen_coord pos);

/*
 * Function: rr_scr_puts_len
 * Writes a string of length <len> to the screen
 *
 * Parameters:
 *  in - what character sequence to write
 *  pos - where to write it (from the first character)
 *  len - length of string
 *
 * Returns:
 *  <la_status>
 */
void rr_scr_puts_len(const char *in, struct screen_coord pos, size_t len);

/*
 * Function: rr_scr_render
 * Renders the frame to the screen
 */
void rr_scr_render(void);

#endif /* RENDER_H */
