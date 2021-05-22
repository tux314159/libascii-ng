#include <stdio.h>
#include <string.h>

#include <base.h>
#include <init.h>
#include <drivers/vt100.h>
#include <render.h>
#include <winsys.h>

int main(void)
{
    int x1, y1, x2, y2, x3, y3;
    lascii_init();
    x1 = ws_frame_new((struct screen_coord){0, 0}, (struct winsz){15, 3});
    y1 = ws_buf_new();
    ws_buf_aline(y1, "Hello, world!");
    ws_frame_bind_buf(x1, y1);

    x2 = ws_frame_new((struct screen_coord){20, 4}, (struct winsz){20,  20});
    y2 = ws_buf_new();
    ws_buf_aline(y2, "Whee!");
    ws_buf_aline(y2, "How do you do?");
    ws_frame_bind_buf(x2, y2);

    x3 = ws_frame_new((struct screen_coord){3, 8}, (struct winsz){25,  5});
    y3 = ws_buf_new();
    ws_buf_aline(y3, "");
    ws_buf_aline(y3, "HALLO NIANNY!!! :DDDD");
    ws_frame_bind_buf(x3, y3);

    ws_render();

    ll_stdin_getchar();
    ws_frame_free(x1);
    ws_buf_free(y1);
    ws_frame_free(x2);
    ws_buf_free(y2);
    ws_frame_free(x3);
    ws_buf_free(y3);
    lascii_deinit();
    return 0;
}
