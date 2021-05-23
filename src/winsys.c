#include <winsys.h>

int ws_frame_new(const struct screen_coord pos, const struct winsz winsz,
                 char borderN, char borderE, char borderS, char borderW)
{
    const int       n_frames        = _la_state->ws_n_frames;
    struct frame    *cframe;


    struct frame *tempframes = realloc(_la_state->ws_frames, (n_frames+1) * sizeof(struct frame));
    if (tempframes == NULL) {
        return -1;
    }

    cframe = &tempframes[n_frames];

    cframe->pos         = pos;
    cframe->winsz       = winsz;
    cframe->boundbuf    = -1;
    cframe->activep     = true; /* active by default */
    cframe->borders[0]  = borderN;
    cframe->borders[1]  = borderE;
    cframe->borders[2]  = borderS;
    cframe->borders[3]  = borderW;
    cframe->scroll_v	= 0;
    cframe->scroll_h	= 0;
    _la_state->ws_frames = tempframes;
    _la_state->ws_n_frames += 1;
    return n_frames; /* this is our copy */
}

void ws_frame_free(const int frameid)
{
    (void)frameid;
    return;
}

void ws_frame_bind_buf(const int frameid, const int bufid)
{
    struct frame *curframe = &_la_state->ws_frames[frameid];

    curframe->boundbuf = bufid;
    return;
}

void ws_frame_mv(const int frameid, const struct screen_coord pos)
{
    _la_state->ws_frames[frameid].pos = pos;
}

void ws_frame_rs(const int frameid, const struct winsz ws)
{
    _la_state->ws_frames[frameid].winsz = ws;
}

int ws_buf_new(void)
{
    const int       n_buf       = _la_state->ws_n_bufs;
    struct buffer   *tempbufs   = realloc(_la_state->ws_bufs, (n_buf+1) * sizeof(struct buffer));
    if (tempbufs == NULL) {
        return -1;
    }

    _la_state->ws_bufs = tempbufs;
    _la_state->ws_bufs[n_buf].buf = malloc(0);
    _la_state->ws_bufs[n_buf].n_lines = 0;

    _la_state->ws_n_bufs += 1;
    return n_buf; /* our old copy */
}

void ws_buf_free(const int bufid)
{
    struct buffer *cbuf = &_la_state->ws_bufs[bufid];
    for (size_t i = 0; i < cbuf->n_lines; i++) {
        string_deinit(cbuf->buf[i]);
        free(cbuf->buf[i]);
    }
    free(_la_state->ws_bufs[bufid].buf);
}

void ws_buf_aline(const int bufid, const char *str)
{
    struct buffer *cbuf = &_la_state->ws_bufs[bufid];
    struct string **tempbufbuf = realloc(
            cbuf->buf,
            (cbuf->n_lines+1) * sizeof(struct string *)
    );
    cbuf->buf = tempbufbuf;
    cbuf->buf[cbuf->n_lines] = malloc(sizeof(struct string));
    string_init(cbuf->buf[cbuf->n_lines]);
    WS_BUF_OP(bufid, cbuf->n_lines, string_append, str);
    cbuf->n_lines += 1;

    return;
}

static inline int min(int a, int b) {
    return a < b ? a : b;
}

void ws_render(void)
{
    const int                   n_frames    = _la_state->ws_n_frames;
    const struct frame *const   frames      = _la_state->ws_frames;
    const struct buffer *const  bufs        = _la_state->ws_bufs;

    for (int i = 0; i < n_frames; i++) {
        const struct frame  *cframe = &frames[i];
        const struct buffer *cbuf;

        if (!cframe->activep) {
            continue; /* don't render if not active */
        }

        /* blank the background for drawing */
        for (int j = cframe->pos.x; j < cframe->pos.x + cframe->winsz.w; j++) {
            for (int k = cframe->pos.y; k < cframe->pos.y + cframe->winsz.h; k++) {
                rr_scr_putc(' ', (struct screen_coord){j, k});
            }
        }

        /* Next two loops render borders */
        for (int j = 0; j < cframe->winsz.h; j++) { /* left & right */
            rr_scr_putc(cframe->borders[3],
                    (struct screen_coord){cframe->pos.x, cframe->pos.y + j}
            );
            rr_scr_putc(cframe->borders[1], (struct screen_coord){
                        cframe->pos.x + cframe->winsz.w - 1,
                        cframe->pos.y + j
                    }
            );
        }

        for (int j = 0; j < cframe->winsz.w; j++) { /* top & bottom */
            rr_scr_putc(cframe->borders[0],
                    (struct screen_coord){cframe->pos.x + j, cframe->pos.y}
            );
            rr_scr_putc(cframe->borders[2], (struct screen_coord){
                        cframe->pos.x + j,
                        cframe->pos.y + cframe->winsz.h - 1
                    }
            );
        }

        /* Render buffer */
        if (cframe->boundbuf == -1) {
            continue; /* nothing to look at here, move on! */
        }
        cbuf = &bufs[cframe->boundbuf];
        for (size_t j = cframe->scroll_v; j < cframe->scroll_v + cframe->winsz.h; ++j) {
            if (j >= cbuf->n_lines) {
                break;
            }
            rr_scr_puts_len(
                    cbuf->buf[j]->str,
                    (struct screen_coord){
                        cframe->pos.x + 1,
                        j - cframe->scroll_v + cframe->pos.y + 1
                    },
                    min(cbuf->buf[j]->len, cframe->winsz.w - 2)
            );
        }
    }
    rr_scr_render();
    return;
}
