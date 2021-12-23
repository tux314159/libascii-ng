#include <init.h>

enum la_status ll_init(void)
{
    /* Memory init */
    _la_state = calloc(sizeof(struct la_state), 1);
    if (_la_state == NULL) {
        return LASCII_ALLOC_FAIL;
    }

    /* Get window size */
    _la_state->scr_size = ll_scr_getsize();

    /* More memory init */
    RETIFNOK(string_init(&_la_state->buf));
    /* Renderer */
    _la_state->rr_curframe = malloc(_la_state->scr_size.h * sizeof(char *));
    _la_state->rr_oldframe = malloc(_la_state->scr_size.h * sizeof(char *));
    for (int i = 0; i < _la_state->scr_size.h; i++) {
        _la_state->rr_curframe[i] =
            malloc(_la_state->scr_size.w * sizeof(char));
        _la_state->rr_oldframe[i] =
            malloc(_la_state->scr_size.w * sizeof(char));
        memset(
            _la_state->rr_curframe[i],
            ' ',
            _la_state->scr_size.w * sizeof(char));
        memset(
            _la_state->rr_oldframe[i],
            ' ',
            _la_state->scr_size.w * sizeof(char));
    }

    /* Set driver stuff */
    /* TODO: detect terminal type and set driver and functions
     * Probably by querying $TERM? */
    _la_state->driver         = VT100;
    _la_state->ll_curs_mov    = &vt100_curs_mov;
    _la_state->ll_curs_vis    = &vt100_curs_vis;
    _la_state->ll_curs_invis  = &vt100_curs_invis;
    _la_state->ll_scr_clear   = &vt100_scr_clear;
    _la_state->ll_ln_clear    = &vt100_ln_clear;
    _la_state->ll_alt_scr_on  = &xterm_alt_scr_on;
    _la_state->ll_alt_scr_off = &xterm_alt_scr_off;
    /* Init the screen */
    /* Raw mode */
    memset(&_la_state->orig_termios, 0, sizeof(struct termios));
    memset(&_la_state->raw_termios, 0, sizeof(struct termios));
    tcgetattr(STDIN_FILENO, &_la_state->orig_termios);
    _la_state->raw_termios = _la_state->raw_termios;
    _la_state->raw_termios.c_iflag &= ~(unsigned int)(ICRNL | IXON);
    _la_state->raw_termios.c_lflag &=
        ~(unsigned int)(ECHO | ICANON | ISIG | IEXTEN);
    _la_state->raw_termios.c_oflag &= ~(unsigned int)(OPOST);
    _la_state->raw_termios.c_cc[VMIN]  = 1;
    _la_state->raw_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_la_state->raw_termios);

    /* Turn off buffering */
    setvbuf(stdout, NULL, _IONBF, 1024);

    /* Screen clearing and stuff */
    ll_buf_clear();
    _la_state->ll_alt_scr_on();
    _la_state->ll_curs_mov((struct screen_coord){1, 1});
    _la_state->ll_curs_vis();
    ll_buf_write();
    ll_buf_clear();
    _la_state->rr_curs_vis_p = true;

    return LASCII_OK;
}

void ll_deinit(void)
{
    /* Reset screen */
    _la_state->ll_alt_scr_off();
    _la_state->ll_curs_vis();
    ll_buf_write();
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_la_state->orig_termios);

    /* Deallocate memory */
    for (int i = 0; i < _la_state->scr_size.h; i++) {
        free(_la_state->rr_curframe[i]);
        free(_la_state->rr_oldframe[i]);
    }
    free(_la_state->rr_curframe);
    free(_la_state->rr_oldframe);
    /**/
    string_deinit(&_la_state->buf);
    free(_la_state);

    return;
}
