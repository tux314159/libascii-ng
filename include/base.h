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
    short x;
    short y;
};

/*
 * Struct: winsz
 * Same thing as <screen_coord>, really
 */
struct winsz {
    short w;
    short h;
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
    char    borders[4]; /* NESW */
    size_t  scroll_v; /* how far we've scrolled (top line) */
    size_t  scroll_h; /* how far we've scrolled (first char) */
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
    void (*ll_curs_mov)(const struct screen_coord);
    void (*ll_curs_vis)(void);
    void (*ll_curs_invis)(void);
    void (*ll_scr_clear)(void);
    void (*ll_ln_clear)(void);
    void (*ll_alt_scr_on)(void);
    void (*ll_alt_scr_off)(void);

    /**/

    /* Renderer */
    struct screen_coord rr_curs_pos;
    char                **rr_curframe; /* To be rendered next */
    char                **rr_oldframe;
    bool                **rr_update_cell_p;
    bool                rr_curs_vis_p;

    /**/

    /* Windowing system */
    int             ws_n_frames;
    int             ws_n_bufs;
    struct frame    *ws_frames;
    struct buffer   *ws_bufs;
};

extern struct la_state *_la_state; /* actually defined in global.c */

#endif /* BASE_H */
