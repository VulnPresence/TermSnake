#include "snake.h"

/* ─────────────────────────────────────────────
   food_spawn
   Places food at a random cell not occupied
   by the snake body.
   ───────────────────────────────────────────── */
void food_spawn(GameState *g) {
    Point candidate;
    int   collision;

    do {
        candidate.x = (rand() % (BOARD_W - 2)) + 1;
        candidate.y = (rand() % (BOARD_H - 2)) + 1;

        collision = 0;
        for (int i = 0; i < g->length; i++) {
            if (g->body[i].x == candidate.x &&
                g->body[i].y == candidate.y) {
                collision = 1;
                break;
            }
        }
    } while (collision);

    g->food = candidate;
}

/* ─────────────────────────────────────────────
   game_init
   Reset everything to a clean initial state.
   ───────────────────────────────────────────── */
void game_init(GameState *g) {
    memset(g, 0, sizeof(GameState));

    /* Start snake in the middle, length 3, heading right */
    g->length   = 3;
    g->dir      = DIR_RIGHT;
    g->next_dir = DIR_RIGHT;
    g->speed    = INITIAL_SPEED;
    g->running  = 1;
    g->paused   = 0;
    g->score    = 0;
    g->food_count = 0;

    int cx = BOARD_W / 2;
    int cy = BOARD_H / 2;

    g->body[0].x = cx;     g->body[0].y = cy;     /* head  */
    g->body[1].x = cx - 1; g->body[1].y = cy;
    g->body[2].x = cx - 2; g->body[2].y = cy;     /* tail  */

    srand((unsigned int)time(NULL));
    food_spawn(g);
}

/* ─────────────────────────────────────────────
   game_check_collision
   Returns 1 if head hits wall or self.
   ───────────────────────────────────────────── */
int game_check_collision(GameState *g) {
    Point head = g->body[0];

    /* Wall collision */
    if (head.x <= 0 || head.x >= BOARD_W - 1 ||
        head.y <= 0 || head.y >= BOARD_H - 1)
        return 1;

    /* Self collision (skip head itself at index 0) */
    for (int i = 1; i < g->length; i++) {
        if (g->body[i].x == head.x &&
            g->body[i].y == head.y)
            return 1;
    }

    return 0;
}

/* ─────────────────────────────────────────────
   game_update
   Advance one tick: move snake, check food,
   check collision, update speed.
   ───────────────────────────────────────────── */
void game_update(GameState *g) {
    if (!g->running || g->paused) return;

    /* Apply buffered direction (prevent 180° reversal) */
    Direction nd = g->next_dir;
    Direction cd = g->dir;

    int bad = (nd == DIR_UP    && cd == DIR_DOWN)  ||
              (nd == DIR_DOWN  && cd == DIR_UP)     ||
              (nd == DIR_LEFT  && cd == DIR_RIGHT)  ||
              (nd == DIR_RIGHT && cd == DIR_LEFT);

    if (!bad) g->dir = nd;

    /* Shift body backwards */
    for (int i = g->length - 1; i > 0; i--) {
        g->body[i] = g->body[i - 1];
    }

    /* Move head */
    switch (g->dir) {
        case DIR_UP:    g->body[0].y--; break;
        case DIR_DOWN:  g->body[0].y++; break;
        case DIR_LEFT:  g->body[0].x--; break;
        case DIR_RIGHT: g->body[0].x++; break;
    }

    /* Collision check */
    if (game_check_collision(g)) {
        g->running = 0;
        return;
    }

    /* Food eaten? */
    if (g->body[0].x == g->food.x &&
        g->body[0].y == g->food.y) {

        g->length++;
        g->score      += 10;
        g->food_count++;

        /* Speed up every 5 food */
        if (g->food_count % 5 == 0 && g->speed > MIN_SPEED)
            g->speed -= SPEED_STEP;

        food_spawn(g);
    }
}
