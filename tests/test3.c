#include <stdio.h>
#include <string.h>
#include <time.h>

#include <base.h>
#include <init.h>
#include <drivers/vt100.h>
#include <render.h>
#include <winsys.h>

static inline void msleep(long msec)
{
    struct timespec ts;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    nanosleep(&ts, &ts);
    return;
}

int main(void)
{
    char c;
    int x0, x1, y1, x2, y2, x3, y3, c1, c2;

    lascii_init();
    ll_stdin_nonblock();
    rr_curs_invis();

    x0 = ws_frame_new((struct screen_coord){0, 0}, ll_scr_getsize(), ' ', ' ', ' ', ' ');

    x1 = ws_frame_new((struct screen_coord){0, 0}, (struct winsz){15, 3}, '-', '|', '-', '|');
    y1 = ws_buf_new();
    ws_buf_aline(y1, "Hello, world!");
    ws_frame_bind_buf(x1, y1);

    x2 = ws_frame_new((struct screen_coord){20, 4}, (struct winsz){20,  20}, '-', '|', '-', '|');
    y2 = ws_buf_new();
    ws_buf_aline(y2, "Whee!");
    ws_buf_aline(y2, "How do you do?");
    ws_frame_bind_buf(x2, y2);

    c1 = 3;
    c2 = 8;
    x3 = ws_frame_new((struct screen_coord){c1, c2}, (struct winsz){25,  5}, '-', '|', '-', '|');
    y3 = ws_buf_new();
    ws_buf_aline(y3, "");
    ws_buf_aline(y3, "HALLO NIANNY!!! :DDDD");
    ws_frame_bind_buf(x3, y3);

    c = ' ';
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
            default:
                break;
        }
        ws_frame_mv(x3, (struct screen_coord){c1, c2});
    } while (c != 'q');

    ll_stdin_getchar();
    ws_frame_free(x0);
    ws_frame_free(x1);
    ws_buf_free(y1);
    ws_frame_free(x2);
    ws_buf_free(y2);
    ws_frame_free(x3);
    ws_buf_free(y3);
    lascii_deinit();
    return 0;
}
