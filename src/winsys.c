#include <winsys.h>

void ws_init(void)
{
    _la_ws_state = malloc(sizeof(*_la_ws_state));
    _la_ws_state->n_frames = 0;
    _la_ws_state->n_bufs   = 0;
    _la_ws_state->frames   = malloc(sizeof(*_la_ws_state->frames));
    llist_init(_la_ws_state->frames, sizeof(struct frame));
    _la_ws_state->bufs = malloc(0);

    return;
}

void ws_deinit(void)
{
    llist_deinit(_la_ws_state->frames);
    free(_la_ws_state->frames);
    for (size_t i = 0; i < _la_ws_state->n_bufs; i++) {
        // can't just use ws_buf_free cos no guarantee it's contiguous
        for (size_t j = 0; j < _la_ws_state->bufs[i].n_lines; j++) {
            string_deinit(_la_ws_state->bufs[i].buf[j]);
            free(_la_ws_state->bufs[i].buf[j]);
        }
        free(_la_ws_state->bufs[i].buf);
    }
    free(_la_ws_state->bufs);
    free(_la_ws_state);
}

struct llist_node *ws_frame_new(
    struct screen_coord pos,
    struct winsz        winsz,
    struct llist_node  *stackpos,
    char                borderN,
    char                borderE,
    char                borderS,
    char                borderW)
{
    struct frame cframe;

    cframe.pos        = pos;
    cframe.winsz      = winsz;
    cframe.boundbuf   = -1;
    cframe.activep    = true; /* active by default */
    cframe.borders[0] = borderN;
    cframe.borders[1] = borderE;
    cframe.borders[2] = borderS;
    cframe.borders[3] = borderW;
    cframe.scroll_v   = 0;
    cframe.scroll_h   = 0;

    _la_ws_state->n_frames += 1;

    return llist_addnode(_la_ws_state->frames, stackpos, &cframe);
}

void ws_frame_bind_buf(struct llist_node *frameptr, int bufid)
{
    ((struct frame *)frameptr->data)->boundbuf = bufid;
    return;
}

void ws_frame_mv(struct llist_node *frameptr, struct screen_coord pos)
{
    ((struct frame *)frameptr->data)->pos = pos;
    return;
}

void ws_frame_rs(struct llist_node *frameptr, struct winsz ws)
{
    ((struct frame *)frameptr->data)->winsz = ws;
    return;
}

int ws_buf_new(void)
{
    const size_t   n_buf = _la_ws_state->n_bufs;
    struct buffer *tempbufs =
        realloc(_la_ws_state->bufs, (n_buf + 1) * sizeof(struct buffer));
    if (tempbufs == NULL) {
        return -1;
    }

    _la_ws_state->bufs                = tempbufs;
    _la_ws_state->bufs[n_buf].buf     = malloc(0);
    _la_ws_state->bufs[n_buf].n_lines = 0;

    _la_ws_state->n_bufs += 1;
    return n_buf; /* our old copy */
}

void ws_buf_free(int bufid)
{
    struct buffer *cbuf = &_la_ws_state->bufs[bufid];
    for (size_t i = 0; i < cbuf->n_lines; i++) {
        string_deinit(cbuf->buf[i]);
        free(cbuf->buf[i]);
    }

    _la_ws_state->n_bufs -= 1;
    free(_la_ws_state->bufs[bufid].buf);

    return;
}

void ws_buf_aline(int bufid, const char *str)
{
    struct buffer  *cbuf = &_la_ws_state->bufs[bufid];
    struct string **tempbufbuf =
        realloc(cbuf->buf, (cbuf->n_lines + 1) * sizeof(struct string *));
    cbuf->buf                = tempbufbuf;
    cbuf->buf[cbuf->n_lines] = malloc(sizeof(struct string));
    string_init(cbuf->buf[cbuf->n_lines]);
    WS_BUF_OP(bufid, cbuf->n_lines, string_append, str);
    cbuf->n_lines += 1;

    return;
}

void ws_frame_swapstackpos(struct llist_node *frameptr, bool forward_p)
{
    if (forward_p && frameptr->next) {
        llist_nodeswap(_la_ws_state->frames, frameptr, frameptr->next);
    } else if (!forward_p && frameptr->prev) {
        llist_nodeswap(_la_ws_state->frames, frameptr, frameptr->prev);
    }
}

void ws_frame_focus(struct llist_node *frameptr)
{
    _la_ws_state->focused_frame = frameptr;
}

/* Gets kinda ugly here */
static void ws_render_1f(struct frame *cframe)
{
    /* Render a single frame */
    const struct buffer *cbuf;

    if (!cframe->activep) {
        return; /* don't render if not active */
    }

    /* blank the background for drawing */
    for (int j = 1; j < cframe->winsz.w - 1; j++) {
        for (int k = 1; k < cframe->winsz.h - 1; k++) {
            rr_scr_putc(
                ' ',
                (struct screen_coord){j + cframe->pos.x, k + cframe->pos.y});
        }
    }

    /* Next two loops render borders */
    for (int j = 0; j < cframe->winsz.h; j++) { /* left & right */
        rr_scr_putc(
            cframe->borders[3],
            (struct screen_coord){cframe->pos.x, cframe->pos.y + j});
        rr_scr_putc(
            cframe->borders[1],
            (struct screen_coord){
                cframe->pos.x + cframe->winsz.w - 1,
                cframe->pos.y + j});
    }

    for (int j = 0; j < cframe->winsz.w; j++) { /* top & bottom */
        rr_scr_putc(
            cframe->borders[0],
            (struct screen_coord){cframe->pos.x + j, cframe->pos.y});
        rr_scr_putc(
            cframe->borders[2],
            (struct screen_coord){
                cframe->pos.x + j,
                cframe->pos.y + cframe->winsz.h - 1});
    }

    /* Render buffer */
    if (cframe->boundbuf == -1) {
        return; /* nothing to look at here, move on! */
    }
    cbuf = &_la_ws_state->bufs[cframe->boundbuf];
    for (size_t j = 0; j < cframe->winsz.h; ++j) {
        size_t jj = j + cframe->scroll_v;

        if (jj >= cbuf->n_lines) {
            break;
        }
        rr_scr_puts_len(
            cbuf->buf[jj]->str,
            (struct screen_coord){
                cframe->pos.x + 1,
                jj - cframe->scroll_v + cframe->pos.y + 1},
            cbuf->buf[j]->len < cframe->winsz.w - 2u ? cbuf->buf[j]->len
                                                     : cframe->winsz.w - 2u);
    }
}

void ws_render(void)
{
    const struct llist *frames     = _la_ws_state->frames;
    struct llist_node  *cframecont = frames->head;

    TRAVERSE_LLIST(
        cframecont,
        ws_render_1f((struct frame *)(cframecont->data)));
    rr_scr_render();
    return;
}
