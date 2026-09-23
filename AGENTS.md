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
- `ai_mood` is a presentation-safe diagnostic from the core policy. The fixed
  deterministic neural policy observes player/alien alignment, row, player
  shot, shield integrity, and EMP state. UI may visualize mood, but must not
  use it to alter simulation decisions. EMP must suppress enemy firing.
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
  relic has up to three permanent tiers; each tier absorbs one player hit per
  blind. Restore charges to the tier count on room entry and cap the tier.
- Runs have eight antes of three blinds. Every third blind requires a boss
  kill; killing it clears that blind even if saucers did not meet the score
  target. Only room `RUN_BLINDS` wins.
  `won` is distinct from `over`; restart clears both while preserving seed.
- Boss rules are seed/ante-derived in `boss.c`, independent of combat draws.
  Static suppresses module effects without deleting inventory; Breach bypasses
  shields for enemy shots only; Blackout stops saucers. The final boss combines
  all three. Restrictions expire on room transition and are shown in the HUD.
- Progression follows a Balatro-inspired run loop: rooms are blinds, every
  three blinds form an ante, and the upgrade offer is the shop phase. Keep
  `blind_target` and `ante` in core state so SDL stays presentational.
- Credits and shop prices belong in the core. UI choices must call
  `game_choose_upgrade` and tolerate unaffordable purchases without mutating
  the economy directly.
- Blind completion pays a base reward plus capped interest on credits held
  before the reward. Keep this calculation in `room.c` and test its boundary.
- Blind completion also cashes out the current combo at one credit per four
  steps, capped at two; room entry resets the combo.
- Shop cards use `game_upgrade_cost` and `game_upgrade_available`. Always let
  the player skip an unaffordable shop with `game_skip_upgrade`.
- Shop transactions live in `shop.c`, rerolls in `shop_reroll.c`. Purchases
  never unpause combat; only leaving closes the shop. Services stock one unit
  per visit; reject repairs of intact shields. Module purchases empty the slot.
- Rerolls draw a different unowned module using shop RNG only, and are disabled
  without an alternative or enough credits. Costs rise from two to ten credits.
  Room entry resets stock, offer, and reroll count; failed transactions mutate
  nothing. Cover multi-purchase sessions and stock reset with headless tests.
- `routes.c` owns the next-blind route choice. During a shop, keys 7/8/9 map
  to Combat/Elite/Cache and update armor consistently; Cache also grants one
  EMP charge on entry up to the carry cap. Route choice never
  consumes RNG, money, or shop stock; `route_chosen` makes it one-shot per
  shop so Cache rewards cannot be duplicated. Every third blind is a fixed boss and
  rejects route changes. UI must show the selected route as locked. Test route
  decisions across seeded non-boss blinds.
- Projectiles store their own launch columns. Neither SDL nor subsequent
  shooter movement should change a shot's path.
- `game_tick` applies normalized held movement and fire before simulation.
  SDL events only update held controls; do not move directly on key-repeat.
  Clear controls on focus loss, pause, shop entry, and restart. Focus loss
  pauses rather than letting a hidden run take damage.
- Player shots use three collision-checked substeps per tick. Never jump over
  shield or enemy cells. Stop substeps immediately on a room transition.
- Enemy movement is deterministic and ramps from one to three cells per tick
  at eight-kill intervals, capped at three. Keep the ramp in `enemy_profile.c`
  and test its thresholds independently of rendering. HUD may expose the
  read-only multiplier but must not calculate or mutate it.
- The arena is 48 by 20 cells at 16 pixels per cell and must fit inside the
  960 by 640 window. Keep labels legible and terminal screens restartable.
- `best_room` is core run state, initialized and restarted to one, and updated
  only on successful blind transitions. Terminal UI may report it with seed
  and score; it must not infer progression from rendering state.
- Keep files focused. Prefer a new small module over growing a catch-all file.
- `N` opens UI-only seed entry. Digits and Backspace edit an unsigned 32-bit
  decimal seed; Enter calls `game_init_seed`, Escape cancels. While editing,
  gameplay and shop shortcuts are inert. The UI seed must be initialized from
  the command-line seed and never use wall-clock or global randomness.
- EMP activation lives in `consumables.c`: start with one charge, carry two,
  and buy one per shop for three credits. Reject activation during pause,
  shop, terminal state, or an existing pulse without spending a charge.
  Clear the enemy shot, then freeze enemies and saucers for 12 ticks while
  player movement and collision-checked shots continue. Pause preserves the
  countdown; room entry clears the effect but not unused charges. Boss rules
  do not disable consumables. Test timing and state boundaries headlessly.
- Dash activation lives in `dash.c`: Left Shift moves four cells in a requested
  direction, clamps to the arena, and starts an 18-tick cooldown. Reject it in
  pause/shop/terminal states, at an edge, or while cooling down without
  mutation. Cooldown decreases through `game_tick`, including during EMP,
  while player movement and fire remain independent. Test core and SDL input.
- Enemy hits grant an 8-tick damage-grace window; later shots during that window
  are harmless and the UI exposes the state.
- Scoring kills build an eight-step combo while refreshed within 24 ticks. Combo
  scores multiply at four and eight, and life loss clears the chain.
- Module synergies live in `modules.c`: Amplifier+Cadence adds two points on
  fourth kills; Signal+Scavenger pays one credit per saucer. Boss Static
  suppresses both with other module effects.
- `shop_hold.c` lets the player pay two credits once per shop to carry the
  current module offer into the next shop; held offers bypass module RNG.
- Module inventory starts at three slots and caps at six. `module_slots.c`
  handles the escalating six/eight/ten-credit, once-per-shop slot purchase; module purchases must
  reject offers when the inventory is full. UI capacity is reported as
  occupied slots over total slots.
- `module_sell.c` permits one deterministic two-credit sale per shop through
  Shift+1..6, clearing the selected owned module without consuming RNG.
- Scoring modules live in `modules.c`: additive bonuses precede multipliers,
  and only kills advance Cadence. Afterburner is a non-scoring module that
  increases player shot substeps by one. Shop offers use their own unsigned
  seeded RNG, exclude owned modules, and reset on replay. Test through shot
  resolution, not just field mutation.
- Scavenger is a non-scoring module: a saucer kill restores one EMP charge up
  to `EMP_CAPACITY`, and Static suppresses that refill with other module
  effects. It must not consume combat RNG or exceed the charge cap.
- Barrier is a non-scoring module: it consumes once when an enemy shot reaches
  shield height, prevents that shield mutation, and refreshes at room entry.
  Static and Breach suppress it; Breach must not consume the charge because it
  bypasses cover.
- Barrier+Scavenger restores one EMP charge when Barrier successfully triggers,
  capped at `EMP_CAPACITY`; failed or suppressed triggers restore nothing.
- Blind targets are cumulative score plus four plus ante on room entry. Surplus points
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

The boss suite checks 100 seeded, controlled-collision runs through all 24
blinds; this verifies progression, not human playability or difficulty balance.
On the current macOS SDL build, an ASan-linked GUI can enter SDL's startup
error dialog before `main`. Core ASan/UBSan tests pass; report sanitized GUI
coverage separately from normal dummy-driver coverage. Keep smoke tests timed.

`replay-test` uses only normal controls/shop APIs across 32 seeds and compares
all core fields. Its pilot statistics diagnose changes, not human playability.
`input-test` exercises real SDL event dispatch without creating a window.
Keep assertions enabled for all test targets, including Release builds.
The C23 core and headless C tests build with `-Wall -Wextra -Wpedantic` on
non-MSVC toolchains; keep new core code warning-clean.
- CMake installs the GUI executable and public headers under the selected
  prefix. Keep install rules aligned with the modular target layout and verify
  them in a disposable prefix before publishing build changes.
- `.github/workflows/ci.yml` is the clean Linux gate: install SDL dependencies,
  configure with Ninja, build Release, run all CTest targets, and verify the
  install tree. New tests must remain display-free under SDL's dummy driver.
