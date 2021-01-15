#include <init/init.h>

enum la_status lascii_init(void)
{
    /* Memory init */
    _la_state = malloc(sizeof(struct la_state));
    if (_la_state == NULL) {
        return LASCII_ALLOC_FAIL;
    }

    /* Get window size */
    _la_state->scr_size = ll_scr_getsize();

    /* More memory init */
    if (string_init(&_la_state->buf) == LMDS_ALLOC_FAIL) {
        return LASCII_ALLOC_FAIL;
    }
    /* For the renderer */
    _la_state->rr_curframe = malloc(_la_state->scr_size.ws_row * sizeof(char*));
    _la_state->rr_oldframe = malloc(_la_state->scr_size.ws_row * sizeof(char*));
    _la_state->rr_update_cell_p = malloc(_la_state->scr_size.ws_row * sizeof(bool*));
    for (int i = 0; i < _la_state->scr_size.ws_row; i++) {
        _la_state->rr_curframe[i] = calloc(_la_state->scr_size.ws_col, sizeof(char));
        _la_state->rr_oldframe[i] = calloc(_la_state->scr_size.ws_col, sizeof(char));
        _la_state->rr_update_cell_p[i] = calloc(_la_state->scr_size.ws_col, sizeof(bool));
    }

    /* Set driver stuff */
    /* TODO: detect terminal type and set driver and functions
     * Probably by querying $TERM? */
    _la_state->driver = VT100;
    _la_state->ll_curs_mov = &vt100_curs_mov;
    _la_state->ll_curs_vis = &vt100_curs_vis;
    _la_state->ll_curs_invis = &vt100_curs_invis;
    _la_state->ll_scr_clear = &vt100_scr_clear;
    _la_state->ll_ln_clear = &vt100_ln_clear;
    _la_state->ll_alt_scr_on = &xterm_alt_scr_on;
    _la_state->ll_alt_scr_off = &xterm_alt_scr_off;

    /* Init the screen */
    /* Raw mode */
    memset(&_la_state->orig_termios, 0, sizeof(struct termios));
    memset(&_la_state->raw_termios, 0, sizeof(struct termios));
    tcgetattr(STDIN_FILENO, &_la_state->orig_termios);
    _la_state->raw_termios = _la_state->raw_termios;
    _la_state->raw_termios.c_iflag &= ~(ICRNL | IXON);
    _la_state->raw_termios.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    _la_state->raw_termios.c_oflag &= ~(OPOST);
    _la_state->raw_termios.c_cc[VMIN] = 1;
    _la_state->raw_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_la_state->raw_termios);

    ll_buf_clear();
    ll_alt_scr_on();
    ll_curs_mov((struct screen_coord){1, 1});
    ll_curs_vis();
    ll_buf_write();
    ll_buf_clear();
    _la_state->rr_curs_vis_p = true;

    return LASCII_OK;
}

void lascii_deinit(void)
{
    /* Reset screen */
    ll_alt_scr_off();
    ll_buf_write();
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_la_state->orig_termios);

    /* Deallocate memory */
    for (int i = 0; i < _la_state->scr_size.ws_row; i++) {
        free(_la_state->rr_curframe[i]);
        free(_la_state->rr_oldframe[i]);
        free(_la_state->rr_update_cell_p[i]);
    }
    free(_la_state->rr_curframe);
    free(_la_state->rr_oldframe);
    free(_la_state->rr_update_cell_p);
    /**/
    string_deinit(&_la_state->buf);
    free(_la_state);

    return;
}
