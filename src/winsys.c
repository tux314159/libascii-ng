#include <winsys.h>

void ws_init(void)
{
    _la_ws_state          = malloc(sizeof(*_la_ws_state));
    _la_ws_state->n_wins  = 0;
    _la_ws_state->windows = malloc(sizeof(*_la_ws_state->windows));
    llist_init(_la_ws_state->windows, sizeof(struct window));

    return;
}

void ws_deinit(void)
{
    for (struct llist_node *n = _la_ws_state->windows->head; n != NULL;
         n                    = n->next) {
        const struct window win = *(struct window *)n->data;
        for (char **r = win.buffer; r < win.buffer + win.winsz.h; r++) {
            free(*r);
        }
        free(win.buffer);
    }
    llist_deinit(_la_ws_state->windows);
    free(_la_ws_state->windows);
    free(_la_ws_state);
}

struct llist_node *ws_window_new(
    struct screen_coord pos,
    struct winsz        winsz,
    struct llist_node  *stackpos,
    char                borderN,
    char                borderE,
    char                borderS,
    char                borderW)
{
    struct window cwin;

    cwin.pos        = pos;
    cwin.winsz      = winsz;
    cwin.activep    = true; /* active by default */
    cwin.borders[0] = borderN;
    cwin.borders[1] = borderE;
    cwin.borders[2] = borderS;
    cwin.borders[3] = borderW;

    _la_ws_state->n_wins += 1;

    cwin.buffer = malloc(winsz.h * sizeof(char *));
    for (char **r = cwin.buffer; r < cwin.buffer + winsz.h; r++) {
        // no, there is no missing null terminator
        *r = calloc(winsz.w, sizeof(char));
    }

    return llist_addnode(_la_ws_state->windows, stackpos, &cwin);
}
void ws_window_mv(struct llist_node *winptr, struct screen_coord pos)
{
    ((struct window *)winptr->data)->pos = pos;
    return;
}

void ws_window_swapstackpos(struct llist_node *winptr, bool forward_p)
{
    if (forward_p && winptr->next) {
        llist_nodeswap(_la_ws_state->windows, winptr, winptr->next);
    } else if (!forward_p && winptr->prev) {
        llist_nodeswap(_la_ws_state->windows, winptr, winptr->prev);
    }
}

void ws_window_focus(struct llist_node *winptr)
{
    _la_ws_state->focused_win = winptr;
}

static void ws_render_1f(struct window *cwin)
{
    /* Render a single window */

    if (!cwin->activep) {
        return; /* don't render if not active */
    }

    /* blank the background for drawing */
    for (int i = 1; i < cwin->winsz.w - 1; i++) {
        for (int j = 1; j < cwin->winsz.h - 1; j++) {
            rr_scr_putc(
                ' ',
                (struct screen_coord){i + cwin->pos.x, j + cwin->pos.y});
        }
    }

    /* Next two loops render borders */
    for (int i = 0; i < cwin->winsz.h; i++) { /* left & right */
        rr_scr_putc(
            cwin->borders[3],
            (struct screen_coord){cwin->pos.x, cwin->pos.y + i});
        rr_scr_putc(
            cwin->borders[1],
            (struct screen_coord){
                cwin->pos.x + cwin->winsz.w - 1,
                cwin->pos.y + i});
    }

    for (int i = 0; i < cwin->winsz.w; i++) { /* top & bottom */
        rr_scr_putc(
            cwin->borders[0],
            (struct screen_coord){cwin->pos.x + i, cwin->pos.y});
        rr_scr_putc(
            cwin->borders[2],
            (struct screen_coord){
                cwin->pos.x + i,
                cwin->pos.y + cwin->winsz.h - 1});
    }

    /* Draw window contents */
    for (int i = 0; i < cwin->winsz.h; i++) {
        for (int j = 0; j < cwin->winsz.w; j++) {
            rr_scr_putc(
                cwin->buffer[i][j],
                (struct screen_coord){
                    cwin->pos.x + j + 1,
                    cwin->pos.y + i + 1,
                });
        }
    }
}

void ws_render(void)
{
    const struct llist *wins     = _la_ws_state->windows;
    struct llist_node  *cwincont = wins->head;

    TRAVERSE_LLIST(cwincont, ws_render_1f((struct window *)(cwincont->data)));
    rr_scr_render();
    return;
}
