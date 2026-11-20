# Contributing to Smol Invaders

Smol Invaders is an arcade shooter with seeded, Balatro-inspired blinds, shop choices and run-local relics: a deterministic C23 simulation core with a C++23 SDL2/SDL2_ttf front end, driven by a tiny fixed-weight neural policy that reacts to your play. Thank you for wanting to help!

## Getting started

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/space-invaders          # resumes the saved run
./build/space-invaders 42      # start a specific seed instead
```

The test suite runs headlessly: `gui-smoke` uses SDL's dummy video driver, so no display is needed. `--smoke` never reads or writes saves.

## Notes for this project

- **RNG discipline is the whole project.** Combat and shop RNG advance independently and never reshuffle already-generated rooms; the run seed (`0xC0FFEE` by default) derives room type, placements, boss rules and shield layouts stably. If your change consumes RNG, say from which stream and update the README's determinism notes.
- Anything shown in the HUD (blind tags like FRENZY/RICH, ability profiles like HASTE/PIERCING/VOLLEY) must stay reproducible without consuming combat RNG.
- The movement signal records **accepted input only**: an edge-blocked move must not let the neural policy anticipate a move that did not happen. Keep that honesty rule for any new sensor.
- The neural policy is fixed weights that react but do not train online; don't add online learning without a design discussion first.
- The simulation core (`space_invaders_core`) stays SDL-free and reusable from any C23/C++23 front end; keep UI code out of it.
- Save handling rules: versioned generic JSON at `~/.ewanc26/smolinvaders/save/run.json`, atomic writes, shop decisions saved while the shop is open.

## Before opening a PR

- `ctest --test-dir build --output-on-failure` passes, including `gui-smoke` and `--smoke`; you ran the game on a fixed seed and got the same run twice.
- Conventional commits; one focused PR per change.
- AI-assisted PRs are welcome; stand behind every line.

## Licence

AGPL-3.0 (see [LICENSE](LICENSE)). Contributions are accepted under the same terms.