# AGENTS.md

## Project shape

- `src/core/` is the headless C23 game simulation. It must not include SDL.
- `src/ui/` is the C++23 SDL2 presentation and input layer.
- `include/space_invaders/` contains the small C ABI shared by both layers.
- `test/` contains deterministic, window-free tests for the core.
- `gui-smoke` exercises SDL rendering under the dummy video driver.

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
- `game_restart` must preserve the current seed; changing the seed is an
  explicit `game_init_seed` operation and should never happen implicitly.
- Use unsigned arithmetic for run RNG. Same seed and same input/tick sequence
  must produce the same first encounter, room sequence, and combat state.
- The SDL executable accepts an optional unsigned integer seed as `argv[1]`;
  keep this as a thin UI entry point over `game_init_seed`.
- Elite room armor is core state (`alien_hp`); rendering may show it but must
  not bypass hit resolution.
- Relics are run-local state, never global state. Elite rewards must be granted
  exactly when the Elite dies and remain deterministic across replay. The
  relic absorbs one player hit per blind; restore its charge on room entry.
- Room 10 is the terminal boss room. `won` is distinct from `over`, and restart
  must clear both while preserving the seed.
- Progression follows a Balatro-inspired run loop: rooms are blinds, every
  three blinds form an ante, and the upgrade offer is the shop phase. Keep
  `blind_target` and `ante` in core state so SDL stays presentational.
- Credits and shop prices belong in the core. UI choices must call
  `game_choose_upgrade` and tolerate unaffordable purchases without mutating
  the economy directly.
- Blind completion pays a base reward plus capped interest on credits held
  before the reward. Keep this calculation in `room.c` and test its boundary.
- Shop cards use `game_upgrade_cost` and `game_upgrade_available`. Always let
  the player skip an unaffordable shop with `game_skip_upgrade`.
- Projectiles store their own launch columns. Neither SDL nor subsequent
  shooter movement should change a shot's path.
- The arena is 48 by 20 cells at 16 pixels per cell and must fit inside the
  960 by 640 window. Keep labels legible and terminal screens restartable.
- Keep files focused. Prefer a new small module over growing a catch-all file.
- Scoring modules live in `modules.c`: additive bonuses precede multipliers,
  and only kills advance Cadence. Shop offers use their own unsigned seeded
  RNG, exclude owned modules, and reset on replay. Test through shot resolution.
- Blind targets are cumulative score plus five on room entry. Surplus points
  remain in total score, but never pre-clear later blinds. Shop rendering lives
  separately in `gui_shop.cpp`; core availability is authoritative.
- Use C23 for the core and C++23 for the UI. Preserve the `extern "C"` API.

## Verification

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The suite checks gameplay, rooms, shops, and a GUI render with SDL's dummy
video driver. Use focused tests for new mechanics rather than expanding one
large assertion chain.
