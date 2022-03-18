#include <stdio.h>
#include <string.h>
#include <time.h>

#include <base.h>
#include <drivers/vt100.h>
#include <init.h>
#include <render.h>
#include <winsys.h>

static inline void msleep(long msec)
{
    struct timespec ts;

    ts.tv_sec  = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    nanosleep(&ts, &ts);
    return;
}

int main(void)
{
    char               c;
    int                c1, c2;
    struct llist_node *x0, *x1, *x2, *x3;
    (void)x0;
    (void)x1;

    ll_init();
    rr_init();
    ws_init();
    ll_stdin_nonblock();
    rr_curs_invis();

    x0 = ws_window_new(
        (struct screen_coord){0, 0},
        ll_scr_getsize(),
        NULL,
        ' ',
        ' ',
        ' ',
        ' ');

    x1 = ws_window_new(
        (struct screen_coord){0, 0},
        (struct winsz){15, 3},
        NULL,
        '-',
        '|',
        '-',
        '|');

    x2 = ws_window_new(
        (struct screen_coord){20, 4},
        (struct winsz){20, 20},
        NULL,
        '-',
        '|',
        '-',
        '|');

    c1 = 3;
    c2 = 8;
    x3 = ws_window_new(
        (struct screen_coord){c1, c2},
        (struct winsz){25, 5},
        x2,
        '-',
        '|',
        '-',
        '|');

    do {
        ws_render();
        msleep(20);
        c = ll_stdin_getchar();
        switch (c) {
        case 'h':
            c1--;
            break;
        case 'j':
            c2++;
            break;
        case 'k':
            c2--;
            break;
        case 'l':
            c1++;
            break;
        case 'w':
            ws_window_swapstackpos(x3, true);
            break;
        case 's':
            ws_window_swapstackpos(x3, false);
            break;
        default:
            break;
        }
        ws_window_mv(x3, (struct screen_coord){c1, c2});
    } while (c != 'q');

    ws_deinit();
    rr_deinit();
    ll_deinit();
    return 0;
}
