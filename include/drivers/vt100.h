#ifndef VT100_H
#define VT100_H

#include <drivers/common.h>
#define LA_DONT_ABSTRACT_LL
#include <base.h>

void vt100_curs_mov(struct screen_coord pos);
void vt100_curs_invis(void);
void vt100_curs_vis(void);
void vt100_scr_clear(void);
void vt100_ln_clear(void);

#endif /* VT100_H */
