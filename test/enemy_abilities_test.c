#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  Game stable, noisy;
  game_init_seed(&stable, 42u);
  noisy = stable;
  for (int i = 0; i < 64; ++i) game_random(&noisy);
  assert(stable.enemy_abilities == noisy.enemy_abilities);
  assert(stable.enemy_abilities == game_enemy_abilities(&stable));

  int saw_haste = 0, saw_piercing = 0, saw_volley = 0;
  for (uint32_t seed = 0; seed < 100; ++seed) {
    Game g;
    game_init_seed(&g, seed);
    saw_haste |= !!(g.enemy_abilities & ENEMY_HASTE);
    saw_piercing |= !!(g.enemy_abilities & ENEMY_PIERCING);
    saw_volley |= !!(g.enemy_abilities & ENEMY_VOLLEY);
  }
  assert(saw_haste && saw_piercing && saw_volley);
  return 0;
}
