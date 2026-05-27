#ifndef SNAKE_H
#define SNAKE_H

/* ─────────────────────────────────────────────
   Cross-platform curses include
   ───────────────────────────────────────────── */
#ifdef _WIN32
  #include <curses.h>      /* PDCurses on Windows */
  #include <windows.h>
#else
  #include <ncurses.h>     /* ncurses on Linux / macOS */
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ─────────────────────────────────────────────
   Game constants
   ───────────────────────────────────────────── */
#define BOARD_W        40
#define BOARD_H        20
#define MAX_SNAKE_LEN  (BOARD_W * BOARD_H)
#define INITIAL_SPEED  120    /* milliseconds per frame */
#define SPEED_STEP     5      /* ms faster every 5 food eaten */
#define MIN_SPEED      40     /* fastest possible */

/* ─────────────────────────────────────────────
   Directions
   ───────────────────────────────────────────── */
typedef enum {
    DIR_UP    = 0,
    DIR_DOWN  = 1,
    DIR_LEFT  = 2,
    DIR_RIGHT = 3
} Direction;

/* ─────────────────────────────────────────────
   Point
   ───────────────────────────────────────────── */
typedef struct {
    int x, y;
} Point;

/* ─────────────────────────────────────────────
   Game state
   ───────────────────────────────────────────── */
typedef struct {
    Point     body[MAX_SNAKE_LEN];
    int       length;
    Direction dir;
    Direction next_dir;   /* buffered input */
    Point     food;
    int       score;
    int       speed;      /* current delay in ms */
    int       food_count; /* total food eaten */
    int       running;
    int       paused;
} GameState;

/* ─────────────────────────────────────────────
   Color pair IDs
   ───────────────────────────────────────────── */
#define COLOR_SNAKE_HEAD  1
#define COLOR_SNAKE_BODY  2
#define COLOR_FOOD        3
#define COLOR_BORDER      4
#define COLOR_UI          5
#define COLOR_TITLE       6

/* ─────────────────────────────────────────────
   Function declarations
   ───────────────────────────────────────────── */
void  game_init(GameState *g);
void  game_update(GameState *g);
int   game_check_collision(GameState *g);
void  food_spawn(GameState *g);

void  draw_init(void);
void  draw_frame(const GameState *g);
void  draw_game_over(const GameState *g);
void  draw_start_screen(void);

void  platform_sleep_ms(int ms);
int   platform_get_input(void);

#endif /* SNAKE_H */
