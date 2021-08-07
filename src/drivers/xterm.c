#include <drivers/xterm.h>

void xterm_curs_mov(struct screen_coord pos)
{
    vt100_curs_mov(pos);
    return;
}

void xterm_curs_invis(void)
{
    vt100_curs_invis();
    return;
}

void xterm_curs_vis(void)
{
    vt100_curs_vis();
    return;
}

void xterm_scr_clear(void)
{
    vt100_scr_clear();
    return;
}

void xterm_ln_clear(void)
{
    vt100_ln_clear();
    return;
}

void xterm_alt_scr_on(void)
{
    ll_buf_append("\x1b[?1049h");
    return;
}

void xterm_alt_scr_off(void)
{
    ll_buf_append("\x1b[?1049l");
    return;
}
