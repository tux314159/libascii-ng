#ifndef BASE_H
#define BASE_H

/* System includes: all here */
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <dstring.h>
#include <llist.h>
#include <status.h>

/*
 * Enum: driver_type
 * Enumerates driver types
 */
enum driver_t
{
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

/* Global state */
struct la_state {
    enum driver_t       driver;
    struct string       buf;
    struct screen_coord curs_pos;
    struct winsz        scr_size;
    struct termios      orig_termios;
    struct termios      raw_termios;

    /* Function pointers to the appropriate driver-specific function */
    void (*ll_curs_mov)(struct screen_coord);
    void (*ll_curs_vis)(void);
    void (*ll_curs_invis)(void);
    void (*ll_scr_clear)(void);
    void (*ll_ln_clear)(void);
    void (*ll_alt_scr_on)(void);
    void (*ll_alt_scr_off)(void);
};
extern struct la_state *_la_state; // actually defined in global.c

#endif /* BASE_H */

#ifndef _LA_LL_ABSTRACTED
#define ll_curs_mov    _la_state->ll_curs_mov
#define ll_curs_vis    _la_state->ll_curs_vis
#define ll_curs_invis  _la_state->ll_curs_invis
#define ll_scr_clear   _la_state->ll_curs_mov
#define ll_ln_clear    _la_state->ll_curs_mov
#define ll_alt_scr_on  _la_state->ll_curs_mov
#define ll_alt_scr_off _la_state->ll_curs_mov
#define _LA_LL_ABSTRACTED
#endif /* _LA_LL_ABSTRACTED */

// clang-format off
#ifdef LA_DONT_ABSTRACT_LL
#undef ll_curs_mov
#undef ll_curs_vis
#undef ll_curs_invis
#undef ll_scr_clear
#undef ll_ln_clear
#undef ll_alt_scr_on
#undef ll_alt_scr_off
#undef _LA_LL_ABSTRACTED   // have been unabstracted
#undef LA_DONT_ABSTRACT_LL // we don't want this to cascade
#endif /* LA_DONT_ABSTRACT_LL */
// clang-format on
