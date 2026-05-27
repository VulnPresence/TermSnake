#include "snake.h"

/* ─────────────────────────────────────────────
   handle_input
   Map key -> direction / action on game state.
   ───────────────────────────────────────────── */
static void handle_input(GameState *g, int key) {
    switch (key) {
        /* Movement */
        case KEY_UP:    case 'w': case 'W': g->next_dir = DIR_UP;    break;
        case KEY_DOWN:  case 's': case 'S': g->next_dir = DIR_DOWN;  break;
        case KEY_LEFT:  case 'a': case 'A': g->next_dir = DIR_LEFT;  break;
        case KEY_RIGHT: case 'd': case 'D': g->next_dir = DIR_RIGHT; break;

        /* Pause */
        case 'p': case 'P':
            g->paused = !g->paused;
            break;

        /* Quit */
        case 'q': case 'Q':
            g->running = 0;
            break;

        default:
            break;
    }
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */
int main(void) {
    GameState g;

    draw_init();
    draw_start_screen();

restart:
    game_init(&g);

    /* ── Main game loop ── */
    while (1) {
        int key = platform_get_input();
        if (key != ERR) handle_input(&g, key);

        if (!g.running) break;

        game_update(&g);
        draw_frame(&g);

        platform_sleep_ms(g.speed);
    }

    /* ── Game over screen ── */
    draw_game_over(&g);

    /* Wait for R (restart) or Q (quit) */
    nodelay(stdscr, FALSE);
    while (1) {
        int key = getch();
        if (key == 'r' || key == 'R') {
            nodelay(stdscr, TRUE);
            goto restart;
        }
        if (key == 'q' || key == 'Q') break;
    }

    endwin();
    return 0;
}
