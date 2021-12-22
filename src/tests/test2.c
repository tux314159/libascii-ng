#include <base.h>
#include <drivers/vt100.h>
#include <init.h>
#include <render.h>
#include <stdio.h>
#include <string.h>

char x;
char s[50];

int main(void)
{
    lascii_init();
    ll_stdin_block();
    rr_curs_invis();
    rr_scr_render();

    x = ll_stdin_getchar();
    sprintf(s, "You pressed the key '%c'.", x);
    rr_scr_puts(s, (struct screen_coord){0, 0});
    rr_scr_puts("Press any key to exit.", (struct screen_coord){0, 1});
    rr_scr_render();

    ll_stdin_getchar();

    lascii_deinit();
}
