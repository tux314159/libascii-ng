#include <drivers/vt100.h>

void vt100_curs_mov(const struct screen_coord pos)
{
    char buf[16];
    memset(buf, '\0', 16 * sizeof(char));
    sprintf(buf, "\x1b[%hd;%hdH", (short)(pos.y + 1), (short)(pos.x + 1));
    ll_buf_append(buf);
    return;
}

void vt100_curs_invis(void)
{
    ll_buf_append("\x1b[?25l");
    return;
}

void vt100_curs_vis(void)
{
    ll_buf_append("\x1b[?25h");
    return;
}

void vt100_scr_clear(void)
{
    ll_buf_append("\x1b[H\x1b[2J");
    return;
}

void vt100_ln_clear(void)
{
    ll_buf_append("\x1b[2K");
    return;
}
