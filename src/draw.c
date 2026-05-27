#include "snake.h"

/* ─────────────────────────────────────────────
   draw_init
   Initialize ncurses + colors.
   ───────────────────────────────────────────── */
void draw_init(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);   /* non-blocking getch */
    curs_set(0);             /* hide cursor */

    if (has_colors()) {
        start_color();
        use_default_colors();

        /* pair, fg, bg */
        init_pair(COLOR_SNAKE_HEAD, COLOR_GREEN,  -1);
        init_pair(COLOR_SNAKE_BODY, COLOR_GREEN,  -1);
        init_pair(COLOR_FOOD,       COLOR_RED,    -1);
        init_pair(COLOR_BORDER,     COLOR_CYAN,   -1);
        init_pair(COLOR_UI,         COLOR_YELLOW, -1);
        init_pair(COLOR_TITLE,      COLOR_WHITE,  -1);
    }
}

/* ─────────────────────────────────────────────
   draw_border
   Draw the game board border.
   ───────────────────────────────────────────── */
static void draw_border(int off_y, int off_x) {
    attron(COLOR_PAIR(COLOR_BORDER) | A_BOLD);

    /* Top and bottom */
    for (int x = 0; x < BOARD_W; x++) {
        mvaddch(off_y,              off_x + x, (x == 0 || x == BOARD_W-1) ? '+' : '-');
        mvaddch(off_y + BOARD_H-1,  off_x + x, (x == 0 || x == BOARD_W-1) ? '+' : '-');
    }

    /* Left and right */
    for (int y = 1; y < BOARD_H - 1; y++) {
        mvaddch(off_y + y, off_x,            '|');
        mvaddch(off_y + y, off_x + BOARD_W-1,'|');
    }

    attroff(COLOR_PAIR(COLOR_BORDER) | A_BOLD);
}

/* ─────────────────────────────────────────────
   draw_frame
   Full render of one game tick.
   ───────────────────────────────────────────── */
void draw_frame(const GameState *g) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    /* Center the board */
    int off_y = (rows - BOARD_H - 4) / 2;
    int off_x = (cols - BOARD_W)     / 2;
    if (off_y < 0) off_y = 0;
    if (off_x < 0) off_x = 0;

    clear();

    /* ── Title ── */
    attron(COLOR_PAIR(COLOR_TITLE) | A_BOLD);
    const char *title = "~ S N A K E ~";
    mvprintw(off_y - 2, off_x + (BOARD_W - (int)strlen(title)) / 2, "%s", title);
    attroff(COLOR_PAIR(COLOR_TITLE) | A_BOLD);

    /* ── Border ── */
    draw_border(off_y, off_x);

    /* ── Food ── */
    attron(COLOR_PAIR(COLOR_FOOD) | A_BOLD);
    mvaddch(off_y + g->food.y, off_x + g->food.x, '@');
    attroff(COLOR_PAIR(COLOR_FOOD) | A_BOLD);

    /* ── Snake body ── */
    attron(COLOR_PAIR(COLOR_SNAKE_BODY));
    for (int i = 1; i < g->length; i++) {
        mvaddch(off_y + g->body[i].y, off_x + g->body[i].x, 'o');
    }
    attroff(COLOR_PAIR(COLOR_SNAKE_BODY));

    /* ── Snake head ── */
    attron(COLOR_PAIR(COLOR_SNAKE_HEAD) | A_BOLD);
    char head_ch;
    switch (g->dir) {
        case DIR_UP:    head_ch = '^'; break;
        case DIR_DOWN:  head_ch = 'v'; break;
        case DIR_LEFT:  head_ch = '<'; break;
        case DIR_RIGHT: head_ch = '>'; break;
        default:        head_ch = 'O'; break;
    }
    mvaddch(off_y + g->body[0].y, off_x + g->body[0].x, head_ch);
    attroff(COLOR_PAIR(COLOR_SNAKE_HEAD) | A_BOLD);

    /* ── HUD ── */
    attron(COLOR_PAIR(COLOR_UI));
    mvprintw(off_y + BOARD_H + 1, off_x,
             "  Score: %-5d  Length: %-3d  Speed: %dms",
             g->score, g->length, g->speed);

    mvprintw(off_y + BOARD_H + 2, off_x,
             "  WASD / Arrow keys  |  P = pause  |  Q = quit");
    attroff(COLOR_PAIR(COLOR_UI));

    if (g->paused) {
        attron(COLOR_PAIR(COLOR_TITLE) | A_BOLD | A_BLINK);
        const char *pmsg = "[ PAUSED ]";
        mvprintw(off_y + BOARD_H / 2,
                 off_x + (BOARD_W - (int)strlen(pmsg)) / 2,
                 "%s", pmsg);
        attroff(COLOR_PAIR(COLOR_TITLE) | A_BOLD | A_BLINK);
    }

    refresh();
}

/* ─────────────────────────────────────────────
   draw_start_screen
   Simple press-any-key splash.
   ───────────────────────────────────────────── */
void draw_start_screen(void) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int cy = rows / 2;
    int cx = cols / 2;

    clear();
    attron(COLOR_PAIR(COLOR_TITLE) | A_BOLD);
    mvprintw(cy - 3, cx - 7,  "~ S N A K E ~");
    attroff(A_BOLD);
    mvprintw(cy - 1, cx - 13, "Classic terminal snake in C");
    mvprintw(cy,     cx - 16, "WASD or Arrow keys to move  |  P pause");
    mvprintw(cy + 2, cx - 10, "Press any key to begin...");
    attroff(COLOR_PAIR(COLOR_TITLE));
    refresh();

    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
}

/* ─────────────────────────────────────────────
   draw_game_over
   Final screen with score.
   ───────────────────────────────────────────── */
void draw_game_over(const GameState *g) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int cy = rows / 2;
    int cx = cols / 2;

    clear();
    attron(COLOR_PAIR(COLOR_FOOD) | A_BOLD);
    mvprintw(cy - 2, cx - 5, "GAME OVER");
    attroff(A_BOLD);
    attron(COLOR_PAIR(COLOR_UI));
    mvprintw(cy,     cx - 8, "Final score : %d", g->score);
    mvprintw(cy + 1, cx - 8, "Length      : %d", g->length);
    mvprintw(cy + 3, cx - 11, "Press R to restart  |  Q to quit");
    attroff(COLOR_PAIR(COLOR_UI) | COLOR_PAIR(COLOR_FOOD));
    refresh();
}
