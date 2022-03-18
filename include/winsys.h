#ifndef FRAME_H
#define FRAME_H

#include <base.h>
#include <drivers/common.h>
#include <render.h>
#include <string.h>

/*
 * Global windowing system struct
 */
struct la_ws_state {
    size_t             n_wins;
    struct llist      *windows;
    struct llist_node *focused_win; // Most window ops are done on this one
};
extern struct la_ws_state *_la_ws_state; // actually defined in global.c

/*
 * Struct: window
 * A physical window on the screen
 */
struct window {
    struct screen_coord pos;
    struct screen_coord oldpos;
    struct winsz        winsz;
    char              **buffer;

    bool activep;
    char borders[4]; // NESW
};

/*
 * Function: ws_init
 * Initialises the windowing subsystem.
 */
void ws_init(void);

/*
 * Function: ws_deinit
 * Deinitialises the windowing subsystem.
 */
void ws_deinit(void);

/*
 * Function: ws_window_new
 * Creates a new window. The window buffer is initialised
 * to 2-d array the same size as the window.
 * Parameters:
 *  pos          - position of the top left corner
 *  winsz        - size of the window
 *  stackpos     - pointer to window to place this one before; if NULL,
 *                 place at the back.
 *  border[NESW]
 *  - {Top,Right,Bottom,Left} borders. If NUL, that border
 *                 will not exist.
 */
struct llist_node *ws_window_new(
    struct screen_coord pos,
    struct winsz        winsz,
    struct llist_node  *stackpos,
    char                borderN,
    char                borderE,
    char                borderS,
    char                borderW);

/*
 * Function: ws_window_mv
 * Moves a window to a new position.
 * Parameters:
 *  windowid - id of the window to be moved
 *  pos     - position to move it to
 */
void ws_window_mv(struct llist_node *windowptr, struct screen_coord pos);

/*
 * Function: ws_window_swapstackpos
 * Moves a window in front or behind by one.
 * Does nothing if impossible.
 *
 * Parameters:
 *  windowptr  - window to move
 *  forward_p - if true, moves in front; else, moves behind
 */
void ws_window_swapstackpos(struct llist_node *windowptr, bool forward_p);

/*
 * Function: ws_window_focus
 * Sets a window as the focused window
 *
 * Parameters:
 *  windowptr - window to focus
 */
void ws_window_focus(struct llist_node *windowptr);

/*
 * Function: ws_render
 * Renders everything
 */
void ws_render(void);

#endif /* FRAME_H */
