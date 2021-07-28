#include <winsys.h>

struct llist_node *ws_frame_new(const struct screen_coord pos, const struct winsz winsz,
                                struct llist_node *stackpos, char borderN, char borderE,
                                char borderS, char borderW)
{
    struct frame    cframe;

    cframe.pos          = pos;
    cframe.winsz        = winsz;
    cframe.boundbuf     = -1;
    cframe.activep      = true; /* active by default */
    cframe.borders[0]   = borderN;
    cframe.borders[1]   = borderE;
    cframe.borders[2]   = borderS;
    cframe.borders[3]   = borderW;
    cframe.scroll_v	    = 0;
    cframe.scroll_h	    = 0;

    _la_state->ws_n_frames += 1;

    return llist_addnode(&_la_state->ws_frames, stackpos, &cframe);
}

void ws_frame_bind_buf(struct llist_node *const frameptr, const int bufid)
{
    ((struct frame *)frameptr->data)->boundbuf = bufid;
    return;
}

void ws_frame_mv(struct llist_node *const frameptr, const struct screen_coord pos)
{
    ((struct frame *)frameptr->data)->pos = pos;
    return;
}

void ws_frame_rs(struct llist_node *const frameptr, const struct winsz ws)
{
    ((struct frame *)frameptr->data)->winsz = ws;
    return;
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

void ws_frame_swapstackpos(struct llist_node *const frameptr, const bool forward_p)
{
    if (forward_p && frameptr->next) {
        llist_nodeswap(&_la_state->ws_frames, frameptr, frameptr->next);
    }
    else if (!forward_p && frameptr->prev) {
        llist_nodeswap(&_la_state->ws_frames, frameptr, frameptr->prev);
    }
}

void ws_frame_focus(struct llist_node *frameptr)
{
    _la_state->ws_focused_frame = frameptr;
}

/* Gets kinda ugly here */

static inline int min(const int a, const int b) {
    return a < b ? a : b;
}

static void ws_render_1f(struct frame *const cframe)
{
    /* Render a single frame */
    const struct buffer *cbuf;

    if (!cframe->activep) {
        return; /* don't render if not active */
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
        return; /* nothing to look at here, move on! */
    }
    cbuf = &_la_state->ws_bufs[cframe->boundbuf];
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

void ws_render(void)
{
    const struct llist  *frames     = &_la_state->ws_frames;
    struct llist_node   *cframecont = frames->head;

    TRAVERSE_LLIST(cframecont, ws_render_1f((struct frame *)(cframecont->data)));
    rr_scr_render();
    return;
}
