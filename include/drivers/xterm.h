#ifndef XTERM_H
#define XTERM_H

#include <base.h>
#include <drivers/common.h>
#include <drivers/vt100.h>

void xterm_curs_mov(struct screen_coord pos);
void xterm_curs_invis(void);
void xterm_curs_vis(void);
void xterm_scr_clear(void);
void xterm_ln_clear(void);

void xterm_alt_scr_on(void);
void xterm_alt_scr_off(void);

#endif /* XTERM_H */
