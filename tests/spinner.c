#include <time.h>

#include <base.h>
#include <init/init.h>
#include <render/render.h>

#define wait1ms rr_curs_mov((struct screen_coord){5, 5}); ts.tv_sec = 0; ts.tv_nsec = 1000000; nanosleep(&ts, NULL);

struct timespec ts;
time_t t;

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Wrong number of arguments\n");
		exit(1);
	}

	lascii_init();
	t = time(NULL);
	while (time(NULL) - t <= atoi(argv[1])) {
		rr_curs_mov((struct screen_coord){5, 5});
		rr_scr_render();
		wait1ms;
		rr_curs_mov((struct screen_coord){5, 6});
		rr_scr_render();
		wait1ms;
		rr_curs_mov((struct screen_coord){5, 7});
		rr_scr_render();
		wait1ms;
		rr_curs_mov((struct screen_coord){6, 7});
		rr_scr_render();
		wait1ms;
		rr_curs_mov((struct screen_coord){7, 7});
		rr_scr_render();
		wait1ms;
		rr_curs_mov((struct screen_coord){7, 6});
		rr_scr_render();
		wait1ms;
		rr_curs_mov((struct screen_coord){7, 5});
		rr_scr_render();
		wait1ms;
		rr_curs_mov((struct screen_coord){6, 5});
		rr_scr_render();
		wait1ms;
	}
	lascii_deinit();
	return 0;
}
