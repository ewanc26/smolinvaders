# Smol Invaders

An arcade shooter with seeded, Balatro-inspired blinds, shop choices, and
run-local relics. The simulation is C23; the windowed UI is C++23 with SDL2
and SDL2_ttf.

The invader is driven by a tiny deterministic neural policy that reads the
player's position, movement direction, shot state, remaining shield integrity,
and EMP state. Its fixed weights react to play; it does not train online. The
movement signal records accepted input only, so edge-blocked movement cannot
make the invader anticipate a move that did not happen. EMP pressure suppresses
enemy firing, while damaged cover changes its firing pressure. Kills advance
blinds; the player has three lives.

Three destructible energy shields sit between the player and the invader. Both
player and enemy shots damage them, so preserving cover is part of the strategy.

An occasional gold signal saucer crosses the top of the arena. Shoot it for
three points; missing it lets the opportunity pass.

Runs are seeded (`0xC0FFEE` by default), including the first enemy position,
and divide into eight antes of three blinds. Each blind needs `4 + ante` new points;
surplus score stays in the run total but cannot pre-clear the next blind.
Meeting the target opens a shop:
`1` repairs every shield, `2` grants a life, and `3` weakens the neural
policy's steering and firing pressure. The jammer stacks up to three times. You can keep your credits by
skipping the shop with `0`.

While the shop is open, the next non-boss blind can be routed deliberately:
`7` selects Combat (baseline), `8` selects Elite (three armor and a relic),
or `9` selects Cache (earlier saucer, one EMP charge on entry, and one free
reroll in the next shop). Route selection is free, deterministic,
and remains locked on every third blind because that blind is always a boss.
Only the first route key in a shop takes effect, so Cache rewards cannot be
duplicated by changing your mind; the shop then displays the locked route.
This adds replayable risk/reward path planning to the seeded run.

`4` buys the seeded scoring module for five credits. Runs begin with three module
slots; press `M` in a shop to buy one additional slot for six credits. Later
slots cost eight and ten credits, up to six total.
The shop shows occupied slots over total capacity and explains when the module
card is blocked by a full build.
Sell an owned module with `Shift+1` through `Shift+6` for two credits; selling is
available once per shop and does not consume any RNG.
Modules last for the run,
never appear twice once owned, and stack: Amplifier adds one point per kill,
Cadence adds two on every third kill (including saucers), and Signal doubles
saucer points after those additions. Armor damage does not count as a kill.
Afterburner adds one collision-checked substep to every player shot, letting a
build trade shop credits for faster interception rather than raw score.
Scavenger restores one EMP charge when you shoot a signal saucer, up to the
two-charge carry limit. It creates a defensive loop for players who can take
the risk of hunting the bonus target.
Barrier negates the first enemy shot that reaches a shield in each blind, then
refreshes when the next blind begins. Static and Breach bosses suppress that
protection; Breach shots bypass cover and cannot consume it.
Barrier plus Scavenger also restores one EMP charge when the barrier triggers,
up to the normal carry cap.
Build pairs have extra identities: Amplifier plus Cadence adds two points on
every fourth kill, while Signal plus Scavenger pays one bonus credit whenever a
saucer is destroyed. These synergies are deterministic and remain suppressed
by the Static boss restriction.
Amplifier plus Signal adds one extra saucer point before Signal's multiplier,
making bonus-target hunting a distinct scoring build.
The HUD lists owned modules. Shops remain open until you press `0` to leave.
The HUD and shop also show clean-clear progress and banked free rerolls, so the
streak reward is visible before choosing the next route.
The shop previews the current interest payout (one credit per five saved,
capped at three), making the save-versus-spend decision explicit.
That payout is calculated by the deterministic C23 core and only rendered by
the C++23 GUI.
Buy several cards if you can afford them; repair, life, and jammer each have
one unit per visit. Intact shields cannot be repaired for a wasted credit.
Press `5` to reroll the module slot: two credits initially, rising by one per
reroll to a cap of ten. Rerolls use only the shop RNG, guarantee a different
unowned module, and are disabled when no alternative exists. Buying a module
empties its slot; a paid reroll can restock it. Stock and reroll prices reset
at the next shop. Leaving preserves unspent credits for interest.
Shop randomness is separate from combat randomness. A Cache clear's free reroll
is consumed before credits and still advances the seeded shop offer.
Leaving an untouched shop with `0` grants one free skip-tag reroll for the next
shop; any purchase, reroll, hold, or route choice forfeits that tag.
Press `H` to hold the current module offer for the next shop. Holding costs two
credits and is available once per shop; the held offer bypasses the next module
roll without consuming combat RNG.

Each run starts with one EMP charge; carry up to two. Press `X` during combat
to erase the incoming enemy shot and freeze enemy movement, neural firing,
and saucer timers for 12 simulation ticks (about 0.84 seconds at normal speed).
You can still move and fire. Frozen enemies turn cyan and the HUD says ACTIVE.
The EMP works against bosses too, but does not damage them or bypass armor.
`6` buys one charge for three credits, once per shop. Unused charges carry
between blinds; an active freeze ends when a new blind starts. You cannot
stack active pulses or spend charges while paused, shopping, or after a run.

These original shooter modifiers take inspiration from the shop-acquired,
score-changing Jokers described in the [official Balatro FAQ](https://www.playbalatro.com/faq).
The run follows the FAQ's eight-ante structure, with original shooter bosses.
Credits are part of the run economy: signal saucers pay two credits and Elite
commanders pay three. Shield repair costs one, an extra life costs three, and
the neural jammer costs four; unaffordable choices do nothing.
Clearing a blind pays two credits plus one interest credit per five saved,
capped at three. Saving through a shop can fund stronger purchases later.
The shop-opening payout also cashes out the current combat combo at one credit
per four combo steps, capped at two; taking a life loss before the clear gives
up that bonus.
During the shop phase, labeled cards show those prices and dim when the run
cannot afford them.

Each room is also seeded as Combat, Elite, or Cache. Elite rooms amplify the
invader policy; Cache rooms make the bonus saucer arrive sooner. The room type
is generated by the run seed and is visible through the enemy color.
Elite commanders also require three hits, with their remaining armor shown
above them.
Defeating an Elite grants a violet relic tier, capped at three. A no-relic-hit
Elite clear also pays one bonus credit. Each tier
absorbs one enemy hit per blind, so repeated Elite and boss victories improve
future survivability. The HUD shows the permanent tier and current charges.
Two consecutive no-relic-hit Elite or boss clears also earn a free shop reroll;
the streak resets when a relic is consumed, rewarding pressure without making
flawless play an automatic win.
Every third blind is a boss with `2 + ante` armor. Its seed-derived restriction
is announced beneath the arena and visible while shopping before that fight:
Static suppresses scoring modules (you keep them), Breach makes enemy shots
bypass shields, and Blackout prevents signal saucers. Boss identity depends
only on seed and ante, not how many shots you fired. Killing the boss awards
`4 + ante` base points, a relic charge, and three credits; reaching the score
target alone never skips a living boss. A boss kill itself clears the blind,
so a low-scoring but skillful fight cannot soft-lock the run.
Room 24 is the Eclipse commander, combining all three restrictions. Defeating
it wins the run. Boss victories also grant one permanent module slot up to the
six-slot cap, and one EMP charge up to the normal carry cap. Restrictions end
when leaving their blind. A boss cleared without consuming a relic charge pays
one additional credit.
The window stays open after death
or victory so `R` can
replay the same seed.
Terminal screens show the seed, score, furthest blind, clean-clear streak, and
banked rerolls, making a failed build easy to compare against a later route or
module combination.

## Build

Install SDL2, SDL2_ttf, CMake, and C23/C++23 compilers, then:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
cmake --install build --prefix ./dist
./build/space-invaders
./build/space-invaders 42
```

Installation includes the `space_invaders_core` static library and public C23
headers, so another C23/C++23 front end can reuse the deterministic simulation
without linking SDL.

The GUI looks for a system TrueType font. Set `SMOLINVADERS_FONT` to a `.ttf`
path if none of its default font locations exist. The `gui-smoke` test runs
with SDL's dummy video driver, so the test suite needs no display.

Hold the arrow keys to move and Space to fire. Input is sampled once per
simulation tick, independent of OS key-repeat settings. Opposite arrows cancel.
Left Shift dashes four cells in the held/facing direction and starts an
18-tick cooldown. Each relic tier reduces that cooldown by two ticks, down to
12; dashes clamp at arena edges and do not change shot lanes.
Use it to evade a firing lane or buy time while an EMP is unavailable.

An enemy hit grants an 8-tick damage-grace window, preventing rapid shot chains
while keeping the seeded simulation deterministic. The HUD shows when grace is active.
Consecutive kills inside a 24-tick window build a combo up to eight; at four and
eight kills, the next scoring event earns a 2x or 3x multiplier. Losing a life
breaks the combo, so chasing saucers can be worth the danger.
Losing window focus pauses combat and clears held keys; press `P` to resume.
Player shots move three cells per tick, checking every cell for collisions;
enemies and their shots move one. Cover still blocks your own shots.
Enemy movement accelerates from one to three cells per tick as the run’s kill
count rises, and the HUD exposes the current threat multiplier. This echoes
the classic clear-the-field pacing described by [The
Strong National Museum of Play](https://www.museumofplay.org/games/space-invaders/).

Use `P` to pause, `R` to replay after
game over or victory, `X` to use EMP, Left Shift to dash, `1`/`2`/`3`/`4`/`6` to buy,
`5` to reroll, `0` to leave the shop,
and Escape to quit. Each shot keeps its launch lane even if you move.
Press `N` at any time to enter a new unsigned seed in the window. Backspace
edits it, Enter starts the run, and Escape cancels. This resets the complete
run state and updates the replay seed without restarting the process.

`./build/replay-test` runs 32 deterministic input-only pilots and compares
every core field on replay, including clean-clear streak state. It never places
projectiles or grants resources.
The pilot uses and restocks EMPs, and prints wins, losses, and the furthest blind;
this is a regression/balance diagnostic, not proof of human difficulty balance.
The SDL input test injects key-up/down and focus-loss events without a window.

See [AGENTS.md](AGENTS.md) for architecture, development rules, and the
headless verification command.

GitHub Actions repeats the Release build, headless suite, and install smoke
check on every push and pull request.
