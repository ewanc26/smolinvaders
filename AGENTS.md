# AGENTS.md

## Project shape

- `src/core/` is the headless C23 game simulation. It must not include SDL.
- `src/ui/` is the C++23 SDL2 presentation and input layer.
- `include/space_invaders/` contains the small C ABI shared by both layers.
- `test/` contains deterministic, window-free tests for the core.

## Development rules

- Keep gameplay deterministic: do not use wall-clock time or uncontrolled global
  randomness in the C core.
- Add gameplay behavior to the C core first, then expose it visually in SDL.
- Every gameplay change needs a headless test; tests must run without a display.
- Pause and restart behavior must remain available through the core API so the
  SDL layer does not own game state transitions.
- Keep files focused. Prefer a new small module over growing a catch-all file.
- Use C23 for the core and C++23 for the UI. Preserve the `extern "C"` API.

## Verification

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The SDL executable is interactive; the test executable is the authoritative
headless verification path.
