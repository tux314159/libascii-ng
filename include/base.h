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
 * Refers to a coordinate
 * This should be pretty self-explanatory
 */
struct screen_coord {
    short row;
    short col;
};

struct la_state {
    /* Lower-level */
    enum driver_t driver;
    struct string buf;
    struct screen_coord curs_pos;
    struct winsize scr_size;
    struct termios orig_termios;
    struct termios raw_termios;

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
    char **rr_curframe; /* To be rendered next */
    char **rr_oldframe;
    bool **rr_update_cell_p;
    bool rr_curs_vis_p;
};

extern struct la_state *_la_state;

#endif /* BASE_H */
