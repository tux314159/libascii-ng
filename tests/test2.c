#include <stdio.h>
#include <string.h>
#include <base.h>
#include <init.h>
#include <render.h>
#include <drivers/vt100.h>

char x;
char s[100];
char s2[256];

int main(void)
{
    lascii_init();
    ll_stdin_block();
    rr_curs_invis();
    rr_scr_render();

    x = ll_stdin_getchar();
    sprintf(s, "You pressed the key '%c'.", x);
    rr_scr_puts(s, (struct screen_coord){1, 1});
    rr_scr_puts("Press any key to exit.", (struct screen_coord){2, 1});
    rr_scr_render();

    ll_stdin_getchar();

    lascii_deinit();
}
