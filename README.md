# Space Invaders

Minimal terminal Space Invaders: a C23 game core with a C++23 terminal UI.

## Build

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/space-invaders
```

Use `a`/`d` to move, Space to fire, and `q` to quit.
