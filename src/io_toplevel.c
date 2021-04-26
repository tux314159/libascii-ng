#include <io_toplevel.h>

void iot_stdin_nonblock(void)
{
    ll_stdin_nonblock();
}

void iot_stdin_block(void)
{
    ll_stdin_block();
}

char iot_getchar(void)
{
    return ll_stdin_getchar();
}

void iot_getstr(const char delim, char *const buf, const size_t max)
{
    size_t i;
    char c;
    i = 0;

    do {
        c = iot_getchar();
        buf[i] = c;
    } while (c != delim && ++i < max);
}
