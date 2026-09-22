# Space Invaders

Minimal windowed Space Invaders: a C23 game core with a C++23 SDL2 GUI.

## Build

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/space-invaders
```

Use the arrow keys to move, Space to fire, and Escape to quit.
