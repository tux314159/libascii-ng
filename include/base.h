/* ALWAYS INCLUDE THIS FILE LAST. IF YOU DON'T, IT WILL BUILD FINE
 * AND WORK EXACTLY THE SAME AS OF 28 AUGUST 2021, BUT THAT IS SUBJECT
 * TO CHANGE. A LOT OF IT.
 */
#ifndef BASE_H
#define BASE_H

/* System includes: all here */
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <status.h>
#include <dstring.h>
#include <llist.h>

/* Enum: driver_type
 * Enumerates driver types
 */
enum driver_t {
    VT100 = 1
};

/*
 * Struct: screen_coord
 * Refers to a screen coordinate
 * This should be pretty self-explanatory
 */
struct screen_coord {
    unsigned short x;
    unsigned short y;
};

/*
 * Struct: winsz
 * Same thing as <screen_coord>, really
 */
struct winsz {
    unsigned short w;
    unsigned short h;
};

/*
 * Struct: frame
 * A physical window on the screen;
 * Not the frames in the renderer!
 * May have at most one buffer bound to it.
 */
struct frame {
    struct screen_coord pos;
    struct screen_coord oldpos;
    struct winsz        winsz;

    int     boundbuf;
    bool    activep;
    char    borders[4]; // NESW
    size_t  scroll_v; // how far we've scrolled (top line)
    size_t  scroll_h; // how far we've scrolled (first char)

    char    *input_buffer;
};

/*
 * Struct: buffer
 * A text buffer that may be bound to a frame.
 */
struct buffer {
    struct string   **buf; /* one per line */
    size_t          n_lines;
};

/* Global state */
struct la_state {
    /* Lower-level */
    enum    driver_t        driver;
    struct  string          buf;
    struct  screen_coord    curs_pos;
    struct  winsz           scr_size;
    struct  termios         orig_termios;
    struct  termios         raw_termios;

    /* Function pointers to the appropriate driver-specific function */
    void (*ll_curs_mov)(struct screen_coord);
    void (*ll_curs_vis)(void);
    void (*ll_curs_invis)(void);
    void (*ll_scr_clear)(void);
    void (*ll_ln_clear)(void);
    void (*ll_alt_scr_on)(void);
    void (*ll_alt_scr_off)(void);

    /**/

    /* Renderer */
    struct screen_coord rr_curs_pos;
    char                **rr_curframe; // To be rendered next; writing happens here
    char                **rr_oldframe; // Previous frame for deltas */
    bool                rr_curs_vis_p;

    /**/

    /* Windowing system */
    size_t              ws_n_frames;
    size_t              ws_n_bufs;
    struct llist        ws_frames;
    struct buffer       *ws_bufs;
    struct llist_node   *ws_focused_frame; // Most frame ops are done on this one
};

extern struct la_state *_la_state; // actually defined in global.c

#endif /* BASE_H */

#ifndef _LA_LL_ABSTRACTED
 #define ll_curs_mov _la_state->ll_curs_mov
 #define ll_curs_vis _la_state->ll_curs_vis
 #define ll_curs_invis _la_state->ll_curs_invis
 #define ll_scr_clear _la_state->ll_curs_mov
 #define ll_ln_clear _la_state->ll_curs_mov
 #define ll_alt_scr_on _la_state->ll_curs_mov
 #define ll_alt_scr_off _la_state->ll_curs_mov
 #define _LA_LL_ABSTRACTED
#endif /* _LA_LL_ABSTRACTED */

#ifdef LA_DONT_ABSTRACT_LL
 #undef ll_curs_mov
 #undef ll_curs_vis
 #undef ll_curs_invis
 #undef ll_scr_clear
 #undef ll_ln_clear
 #undef ll_alt_scr_on
 #undef ll_alt_scr_off
 #undef _LA_LL_ABSTRACTED  // have been unabstracted
 #undef LA_DONT_ABSTRACT_LL  // we don't want this to cascade
#endif /* LA_DONT_ABSTRACT_LL */
