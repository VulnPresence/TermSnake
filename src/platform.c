#include "snake.h"

/* ─────────────────────────────────────────────
   platform_sleep_ms
   Sleep for given milliseconds, cross-platform.
   ───────────────────────────────────────────── */
void platform_sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

/* ─────────────────────────────────────────────
   platform_get_input
   Non-blocking key read. Returns ERR if no key.
   Maps arrow keys + WASD + pause/quit.
   ───────────────────────────────────────────── */
int platform_get_input(void) {
    return getch();   /* ncurses/PDCurses: non-blocking because of nodelay() */
}
