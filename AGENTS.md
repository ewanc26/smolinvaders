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
- Shield geometry and damage belong in the core; the UI only visualizes shield
  cells and must not mutate them.
- `ai_mood` is a presentation-safe diagnostic from the core policy. UI may
  visualize it, but must not use it to alter simulation decisions.
- Bonus entities must use the core's deterministic timer/RNG state and expose
  their collision behavior to headless tests before being rendered by SDL.
- Roguelike state must be seedable through `game_init_seed`. Room transitions
  pause in the core and upgrades are selected through `game_choose_upgrade`; UI
  must never invent progression or mutate run state directly.
- Room types are generated from the run RNG. Combat, Elite, and Cache behavior
  must differ in the core and remain reproducible for a given seed.
- Elite room armor is core state (`alien_hp`); rendering may show it but must
  not bypass hit resolution.
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
