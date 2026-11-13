#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  Game stable, noisy;
  game_init_seed(&stable, 42u);
  noisy = stable;
  for (int i = 0; i < 64; ++i) game_random(&noisy);
  assert(stable.enemy_abilities == noisy.enemy_abilities);
  assert(stable.enemy_abilities == game_enemy_abilities(&stable));
  stable.enemy_abilities = ENEMY_HASTE | ENEMY_VOLLEY;
  assert(game_enemy_ability_description(&stable)[0] == 'T');
  stable.enemy_abilities = ENEMY_HASTE | ENEMY_PIERCING | ENEMY_VOLLEY;
  assert(game_enemy_ability_description(&stable)[0] == 'T');

  int saw_haste = 0, saw_piercing = 0, saw_volley = 0;
  for (uint32_t seed = 0; seed < 100; ++seed) {
    Game g;
    game_init_seed(&g, seed);
    saw_haste |= !!(g.enemy_abilities & ENEMY_HASTE);
    saw_piercing |= !!(g.enemy_abilities & ENEMY_PIERCING);
    saw_volley |= !!(g.enemy_abilities & ENEMY_VOLLEY);
  }
  assert(saw_haste && saw_piercing && saw_volley);

  Game baseline, haste, piercing;
  game_init_seed(&baseline, 5u);
  haste = piercing = baseline;
  baseline.enemy_abilities = 0;
  haste.enemy_abilities = ENEMY_HASTE;
  assert(game_enemy_speed(&haste) == game_enemy_speed(&baseline) + 1);
  piercing.enemy_abilities = ENEMY_PIERCING;
  piercing.enemy_bullet = SHIELD_ROW - 1;
  piercing.enemy_bullet_x = 1;
  int cover = piercing.shields[0][0];
  game_step(&piercing);
  assert(piercing.shields[0][0] == cover);
  return 0;
}
