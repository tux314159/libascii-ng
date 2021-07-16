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

void ll_scr_clear(void)
{
    _la_state->ll_scr_clear();
    return;
}

struct winsz ll_scr_getsize(void)
{
    /* TODO: Maybe make this less platform-specific?
     * Maybe, just maybe... */
    struct winsize w;
    struct winsz w2;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    w2.w = w.ws_col;
    w2.h = w.ws_row;
    return w2;
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
    struct termios ttystate;
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_cc[VMIN] = 0;
    ttystate.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    return;
}

void ll_stdin_block(void)
{
    struct termios ttystate;
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) & ~O_NONBLOCK);
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_cc[VMIN] = 1;
    ttystate.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    return;
}

char ll_stdin_getchar(void)
{
    ssize_t ret;
    char buf;
    ret = read(STDIN_FILENO, &buf, 1);
    if (ret == -1) {
        return -1;
    }
    return buf;
}
