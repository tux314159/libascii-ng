#ifndef FRAME_H
#define FRAME_H

#include <base.h>
#include <drivers/common.h>
#include <render.h>
#include <string.h>

/*
 * A window is made of two things: a frame and a tbuffer.
 * The frame is physically rendered onto the screen, while
 * tbuffers are tbuffers of text. Buffers may be _bound_ to
 * or _unbound_ from frames.
 */

/*
 * Global windowing system struct
 */
struct la_ws_state {
    size_t             n_frames;
    size_t             n_tbufs;
    struct llist      *frames;
    struct tbuffer    *tbufs;
    struct llist_node *focused_frame; // Most frame ops are done on this one
};
extern struct la_ws_state *_la_ws_state; // actually defined in global.c

/*
 * Struct: frame
 * A physical window on the screen;
 * Not the frames in the renderer!
 * May have at most one tbuffer bound to it.
 */
struct frame {
    struct screen_coord pos;
    struct screen_coord oldpos;
    struct winsz        winsz;

    int    boundtbuf;
    bool   activep;
    char   borders[4]; // NESW
    size_t scroll_v;   // how far we've scrolled (top line)
    size_t scroll_h;   // how far we've scrolled (first char)

    char *input_tbuffer;
};

/*
 * Struct: tbuffer
 * A text tbuffer that may be bound to a frame.
 */
struct tbuffer {
    struct string **tbuf; /* one per line */
    size_t          n_lines;
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
 * Function: ws_frame_new
 * Creates a new frame.
 * Parameters:
 *  pos          - position of the top left corner
 *  winsz        - size of the frame
 *  stackpos     - pointer to frame to place this one before; if NULL,
 *                 place at the back.
 *  border[NESW] - {Top,Right,Bottom,Left} borders. If NUL, that border
 *                 will not exist.
 */
struct llist_node *ws_frame_new(
    struct screen_coord pos,
    struct winsz        winsz,
    struct llist_node  *stackpos,
    char                borderN,
    char                borderE,
    char                borderS,
    char                borderW);

/*
 * Function: ws_frame_bind_tbuf
 * Binds a frame to a tbuffer.
 * Parameters:
 *  frameptr    - frame to bind to
 *  tbufid       - tbuffer to bind
 */
void ws_frame_bind_tbuf(struct llist_node *frameptr, int tbufid);

/*
 * Function: ws_frame_mv
 * Moves a frame to a new position.
 * Parameters:
 *  frameid - id of the frame to be moved
 *  pos     - position to move it to
 */
void ws_frame_mv(struct llist_node *frameptr, struct screen_coord pos);

/*
 * Function: ws_frame_rs
 * Resizes a frame. */
void ws_frame_rs(struct llist_node *frameptr, struct winsz ws);

/*
 * Function: ws_frame_swapstackpos
 * Moves a frame in front or behind by one. Does nothing if impossible.
 *
 * Parameters:
 *  frameptr  - frame to move
 *  forward_p - if true, moves in front; else, moves behind
 */
void ws_frame_swapstackpos(struct llist_node *frameptr, bool forward_p);

/*
 * Function: ws_frame_focus
 * Sets a frame as the focused frame
 *
 * Parameters:
 *  frameptr - frame to focus
 */
void ws_frame_focus(struct llist_node *frameptr);

/*
 * Function: ws_tbuf_new
 * Creates a new tbuffer (text buffer).
 */
int ws_tbuf_new(void);

/*
 * Function: ws_tbuf_free
 * Frees a tbuffer.
 * Parameters:
 *  tbufid - tbuffer id to free
 * Warning:
 *  Do not use the tbuffer after freeing!
 */
void ws_tbuf_free(int tbufid);

/*
 * Function: ws_tbuf_aline
 * Appends a line to a tbuffer.
 * Parameters:
 *  tbufid - tbuffer to append line to
 *  str   - string to append
 */
void ws_tbuf_aline(int tbufid, const char *str);

/*
 * Function: ws_render
 * Renders everything
 */
void ws_render(void);

/*
 * Macro: WS_BUF_OP
 * Performs an operation on a tbuffer.
 * Parameters:
 *  tbufid   - tbuffer to operate on
 *  i        - the index
 *  f        - the operation; MUST BE ONE FROM <dstring.h>!!!
 *  ...      - arguments to the function <f>
 */
#define WS_TBUF_OP(tbufid, i, f, ...) \
    (f(_la_ws_state->tbufs[(tbufid)].tbuf[i], ##__VA_ARGS__))

#endif /* FRAME_H */
