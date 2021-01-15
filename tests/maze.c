#include <stdio.h>
#include <time.h>
#include <errno.h>

#include <base.h>
#include <init/init.h>

#define CURR curr-1 /* because screen is 1-indexed but the array is 0-indexed */
#define CURC curc-1 /* ditto */

int main(int argc, char **argv)
{
    FILE *mapfile;
    char *wall_chars;
    char goal;
    char msgaux[70];
    char scan;
    int won;
    short int curr, curc;
    size_t wall_chars_n;
    time_t starttime;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mapfile>\n", argv[0]);
        exit(1);
    }
    mapfile = fopen(argv[1], "r");
    perror("Opening map file");
    if (errno != 0)
        exit(2);

    libascii_init();
    /* Read the map */

    {
        char t;
        int r, c;
        wall_chars = NULL;
        wall_chars_n = 0;

        getline(&wall_chars, &wall_chars_n, mapfile);
        wall_chars[strlen(wall_chars)-1] = '\0'; /* strip newline */
        goal = fgetc(mapfile);
        fgetc(mapfile); /* skip the next newline */
        fscanf(mapfile, "%hd%hd", &curr, &curc);
        fgetc(mapfile); /* skip the next newline */

        r = 0;
        c = 0;
        t = '\0';
        while ((t = fgetc(mapfile)) != EOF) {
            if (t == '\n') {
                r++;
                c = 0;
                continue;
            }
            txt_grid_cell_add(t, MKSPOS(r + 1, c + 1));
            c++;
        }
        fclose(mapfile);
    }

    object_create('@', MKSPOS(curr, curc));

    for (int i = 1; i <= getwinrows(); i++) {
        for (int j = 1; j <= getwincols(); j++) {
            curs_mov(MKSPOS(i, j));
            buf_putstr(char2str(txt_grid_getcell(MKSPOS(i, j))));
        }
    }
    curs_mov(MKSPOS(curr, curc));
    paintscreen();

    won = 0;
    starttime = time(NULL);
    scan = '\0';

    while (scan != 'q') {
        scan = scankey();
        curs_mov(MKSPOS(curr, curc));
        buf_putstr(char2str(' '));
        switch (scan) {
        case 'h':
            if (strchr(wall_chars, txt_grid_getcell(MKSPOS(curr, curc - 1))) == NULL)
                  curc -= 1;
            break;
        case 'j':
            if (strchr(wall_chars, txt_grid_getcell(MKSPOS(curr + 1, curc))) == NULL)
                curr += 1;
            break;
        case 'k':
            if (strchr(wall_chars, txt_grid_getcell(MKSPOS(curr - 1, curc))) == NULL)
                curr -= 1;
            break;
        case 'l':
            if (strchr(wall_chars, txt_grid_getcell(MKSPOS(curr, curc + 1))) == NULL)
                curc += 1;
            break;
        default:
            break;
        }
        object_mov(1, MKSPOS(curr, curc));
        curs_mov(MKSPOS(curr, curc));
        paintscreen();

        if (txt_grid_getcell(MKSPOS(curr, curc)) == goal) {
            won = 1;
            break;
        }
    }

    object_del(1);
    if (won) {
        clearscreen();
        curs_mov(MKSPOS(getwinrows()/2, getwincols()/2 - 5));
        buf_putstr("You WIN! Yay!");
        curs_mov(MKSPOS(getwinrows()/2 + 1, getwincols()/2 - 25));
        sprintf(msgaux, "But you took %ld seconds... Can you do better?", time(NULL) - starttime);
        buf_putstr(msgaux);
        strcpy(msgaux, "(Press any key to exit)");
        curs_mov(MKSPOS(getwinrows()/2 + 2, getwincols()/2 - 12));
        buf_putstr(msgaux);
        paintscreen();
        scankey();
    }
    libascii_exit();
    free(wall_chars);
}
