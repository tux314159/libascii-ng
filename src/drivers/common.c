#include <drivers/common.h>

/* OUTPUT */

enum la_status ll_buf_append(const char *in)
{
    if (string_append(&_la_state->buf, in) == LASCII_ALLOC_FAIL) {
        return LASCII_ALLOC_FAIL;
    }
    return LASCII_OK;
}

void ll_buf_clear(void)
{
    if (_la_state->buf.len > 0) {
        string_delete(&_la_state->buf, 0, _la_state->buf.len - 1);
    }
    return;
}

size_t ll_buf_len(void)
{
    return _la_state->buf.len;
}

void ll_buf_write(void)
{
    write(STDOUT_FILENO, _la_state->buf.str, _la_state->buf.len);
    return;
}

enum la_status ll_curs_mov(const struct screen_coord pos)
{
    if (ll_scr_coords_check(pos) == LASCII_SCR_OOB) {
        return LASCII_SCR_OOB;
    }
    _la_state->ll_curs_mov(pos);
    _la_state->curs_pos = (struct screen_coord){1, 1};
    return LASCII_OK;
}

void ll_curs_invis(void)
{
    _la_state->ll_curs_invis();
    return;
}

void ll_curs_vis(void)
{
    _la_state->ll_curs_vis();
    return;
}

struct winsize ll_scr_getsize(void)
{
    /* TODO: Maybe make this less platform-specific?
     * Maybe, just maybe... */
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w;
}

enum la_status ll_scr_coords_check(struct screen_coord coord)
{
    if (coord.row > _la_state->scr_size.ws_row ||
            coord.col > _la_state->scr_size.ws_col ||
            coord.row < 1 || coord.col < 1) {
        return LASCII_SCR_OOB;
    }
    return LASCII_OK;
}

void ll_alt_scr_on(void)
{
    _la_state->ll_alt_scr_on();
    return;
}

void ll_alt_scr_off()
{
    _la_state->ll_alt_scr_off();
    return;
}

/* INPUT */

void ll_stdin_nonblock(void)
{
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);
    return;
}

void ll_stdin_block(void)
{
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) & ~O_NONBLOCK);
    return;
}

char ll_stdin_getchar(void)
{
    char buf;
    read(STDIN_FILENO, &buf, 1);
    return buf;
}
