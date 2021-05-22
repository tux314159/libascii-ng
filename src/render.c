#include <render.h>

enum la_status rr_curs_mov(const struct screen_coord pos)
{
    _la_state->rr_curs_pos = pos;
    return LASCII_OK;
}

void rr_curs_vis(void)
{
    _la_state->rr_curs_vis_p = true;
    return;
}

void rr_curs_invis(void)
{
    _la_state->rr_curs_vis_p = false;
    return;
}

void rr_scr_putc(const char in, const struct screen_coord pos)
{
    if (in != _la_state->rr_oldframe[pos.y][pos.x]) {
        _la_state->rr_curframe[pos.y][pos.x] = in;
        _la_state->rr_update_cell_p[pos.y][pos.x] = true;
    }
}

void rr_scr_puts(const char *in, const struct screen_coord pos)
{
    const int len = strlen(in);

    for (int i = 0; i < len; i++) {
        rr_scr_putc(*(in + i), (struct screen_coord){pos.x + i, pos.y});
    }
}

void rr_scr_puts_len(const char *in, const struct screen_coord pos, const size_t len)
{
    for (size_t i = 0; i < len; i++) {
        rr_scr_putc(*(in + i), (struct screen_coord){pos.x + i, pos.y});
    }
}

void rr_scr_render(void)
{
    ll_buf_clear();
    for (int i = 0; i < _la_state->scr_size.ws_row; ++i) {
        for (int j = 0; j < _la_state->scr_size.ws_col; ++j) {
            if (_la_state->rr_update_cell_p[i][j] == false) {
                continue;
            }
            ll_curs_mov((struct screen_coord){j, i});
            ll_buf_append((char[2]){_la_state->rr_curframe[i][j], '\0'});
        }
    }

    ll_curs_mov(_la_state->rr_curs_pos);

    if (_la_state->rr_curs_vis_p) {
        ll_curs_vis();
    } else {
        ll_curs_invis();
    }
    ll_buf_write();

    for (int i = 0; i < _la_state->scr_size.ws_row; ++i) {
        memset(_la_state->rr_update_cell_p[i], 0, sizeof(bool) * _la_state->scr_size.ws_col);
    }

    return;
}
