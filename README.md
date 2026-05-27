# 🐍 Snake — Terminal Edition

Classic Snake written in **C** using `ncurses` (Linux/macOS) and `PDCurses` (Windows).  
Clean, cross-platform, zero dependencies beyond the curses library.

---

## Build & Run

### Linux
```bash
sudo apt install libncurses5-dev   # Ubuntu/Debian
make run
```

### macOS
```bash
# ncurses ships with Xcode Command Line Tools
xcode-select --install
make run
```

### Windows (MSYS2 / MinGW)
```bash
pacman -S mingw-w64-x86_64-pdcurses
make run
```

---

## Controls

| Key           | Action        |
|---------------|---------------|
| `W` / `↑`     | Move up       |
| `S` / `↓`     | Move down     |
| `A` / `←`     | Move left     |
| `D` / `→`     | Move right    |
| `P`           | Pause / unpause |
| `Q`           | Quit          |
| `R`           | Restart (game over screen) |

---

## Project Structure

```
snake/
├── include/
│   └── snake.h       ← all types, constants, declarations
├── src/
│   ├── main.c        ← entry point, game loop, input handling
│   ├── game.c        ← game logic (init, update, collision, food)
│   ├── draw.c        ← all ncurses rendering
│   └── platform.c    ← cross-platform sleep + input
└── Makefile          ← auto-detects OS, links correct curses lib
```

---

## Features

- Smooth terminal rendering via `ncurses` / `PDCurses`
- Direction-aware head character (`^` `v` `<` `>`)
- Progressive speed increase every 5 food eaten
- Pause support
- Restart without restarting the process
- Centered board, adapts to terminal size
- Color support (green snake, red food, cyan border)

---

## Technical Notes

Cross-platform curses selection is handled at compile time:

```c
#ifdef _WIN32
  #include <curses.h>   // PDCurses
#else
  #include <ncurses.h>  // ncurses
#endif
```

Sleep is implemented per-platform:
```c
// Linux/macOS
nanosleep(&ts, NULL);

// Windows
Sleep(ms);
```
