# Space Invaders

Minimal windowed Space Invaders: a C23 game core with a C++23 SDL2 GUI.

The invader is driven by a tiny deterministic neural policy that reads the
player's position and shot state. It adapts its movement and fires back. Hits
advance the wave every five points; the player has three lives.

## Build

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/space-invaders
```

Use the arrow keys to move, Space to fire, `P` to pause, `R` to restart
after game over, and Escape to quit.

See [AGENTS.md](AGENTS.md) for architecture, development rules, and the
headless verification command.
