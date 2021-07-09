#ifndef FRAME_H
#define FRAME_H

#include <string.h>
#include <base.h>
#include <drivers/common.h>
#include <render.h>

/* A window is made of two things: a frame and a buffer.
 * The frame is physically rendered onto the screen, while
 * buffers are buffers of text. Buffers may be _bound_
 * or _unbound_ from frames. */

/* sometimes we don't use la_status because we need to return other 
 * values as well */

/* Function: ws_frame_new
 * Creates a new frame.
 * Parameters:
 *  pos         - position of the top left corner
 *  winsz       - size of the frame
 *  stackpos    - pointer to frame to place this one before; if NULL, place at back.
 *  borderN     - north/top border 
 *  borderE     - east/right border
 *  borderS     - south/bottom border
 *  borderW     - west/left border */
struct llist_node *ws_frame_new(const struct screen_coord pos, const struct winsz winsz,
                                struct llist_node *stackpos, char borderN, char borderE,
                                char borderS, char borderW);

/* Function: ws_frame_bind_buf
 * Binds a frame to a buffer.
 * Parameters:
 *  frameptr    - frame to bind to
 *  bufid       - buffer to bind */
void ws_frame_bind_buf(struct llist_node *frameptr, const int bufid);

/* Function: ws_frame_mv
 * Moves a frame to a new position.
 * Parameters:
 *  frameid - id of the frame to be moved
 *  pos     - position to move it to */
void ws_frame_mv(struct llist_node *frameptr, const struct screen_coord pos);

/* Function: ws_frame_rs
 * Resizes a frame. */
void ws_frame_rs(struct llist_node *frameptr, const struct winsz ws);

/* Function: ws_frame_swapstackpos
 * Moves a frame in front or behind by one. Does nothing if impossible.
 * I feel sad :( why am I even writing this code
 * Is there even a meaning to anything?
 *
 * Parameters:
 *  frameptr  - frame to move
 *  forward_p - if true, moves in front; else, moves behind */
void ws_frame_swapstackpos(struct llist_node *frameptr, bool forward_p);

/* Function: ws_buf_new
 * Creates a new buffer. */
int ws_buf_new(void);

/* Function: ws_buf_free
 * Frees a buffer.
 * Parameters:
 *  bufid - buffer id to free
 * Warning:
 *  Do not use the buffer after freeing! */
void ws_buf_free(const int bufid);

/* Function: ws_buf_aline
 * Appends a line to a buffer.
 * Parameters:
 *  bufid - buffer to append line to
 *  str   - string to append */
void ws_buf_aline(const int bufid, const char *str);

/* Function: ws_render
 * Renders everything */
void ws_render(void);

/* Macro: WS_BUF_OP
 * Performs an operation to a buffer buffer.
 * Parameters:
 *  bufid   - buffer to operate on
 *  i       - the index
 *  f       - the operation; MUST BE ONE FROM <dstring.h>!!!
 *  ... - arguments to the function <f> */
#define WS_BUF_OP(bufid, i, f, ...) (f(_la_state->ws_bufs[(bufid)].buf[i], ##__VA_ARGS__))

#endif /* FRAME_H */
