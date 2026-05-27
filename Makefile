# ─────────────────────────────────────────────
#  Snake — Cross-platform Makefile
#  Linux/macOS : links ncurses
#  Windows     : links pdcurses (via MSYS2/MinGW)
# ─────────────────────────────────────────────

CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -Iinclude
TARGET  = snake

SRC     = src/main.c \
          src/game.c \
          src/draw.c \
          src/platform.c

# ── Detect OS ──────────────────────────────────
UNAME := $(shell uname 2>/dev/null || echo Windows)

ifeq ($(OS),Windows_NT)
    LIBS    = -lpdcurses
    TARGET := snake.exe
else ifeq ($(UNAME),Darwin)
    LIBS    = -lncurses
else
    LIBS    = -lncurses
endif

# ── Targets ────────────────────────────────────
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET) snake.exe

run: all
	./$(TARGET)

.PHONY: all clean run
