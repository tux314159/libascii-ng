#ifndef RENDER_H
#define RENDER_H

#include <base.h>
#include <drivers/common.h>
#include <string.h>

/*
 * Structure: la_rr_state
 * Global state struct for renderer.
 */
struct la_rr_state {
    struct screen_coord curs_pos;
    char              **curframe; // To be rendered next; writing happens here
    char              **oldframe; // Previous frame for deltas
    bool                curs_vis_p;
};
extern struct la_rr_state *_la_rr_state; // actually defined in global.c

/*
 * Function: rr_init
 * Initialises the renderer subsystem.
 */
void rr_init(void);

/*
 * Function: rr_deinit
 * Deinitialises the renderer subsystem.
 */
void rr_deinit(void);

/*
 * Function: rr_curs_mov
 * Moves the virtual renderer cursor. This only affects
 * what the frame looks like after rendering, and does not
 * affect the positioning of anything.
 */
void rr_curs_mov(struct screen_coord pos);

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
 * Writes a character to a cell on the screen
 *
 * Parameters:
 *  in  - what character sequence to write; ignored iff NUL.
 *  pos - where to write it
 */
void rr_scr_putc(char in, struct screen_coord pos);

/*
 * Function: rr_scr_puts
 * Writes a string to the screen
 *
 * Parameters:
 *  in  - what character sequence to write
 *  pos - where to write it (from the first character)
 */
void rr_scr_puts(const char *in, struct screen_coord pos);

/*
 * Function: rr_scr_puts_len
 * Writes a string of length <len> to the screen
 *
 * Parameters:
 *  in  - what character sequence to write
 *  pos - where to write it (from the first character)
 *  len - length of string
 */
void rr_scr_puts_len(const char *in, struct screen_coord pos, size_t len);

/*
 * Function: rr_scr_render
 * Renders the frame to the screen
 */
void rr_scr_render(void);

#endif /* RENDER_H */
