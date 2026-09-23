#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  Game stable, noisy;
  game_init_seed(&stable, 123u);
  noisy = stable;
  for (int i = 0; i < 32; ++i) game_random(&noisy);
  assert(game_room_tag(&stable) == game_room_tag(&noisy));
  assert(game_enemy_speed(&stable) == game_enemy_speed(&noisy));

  for (uint32_t seed = 0; seed < 1000; ++seed) {
    Game g;
    game_init_seed(&g, seed);
    g.enemy_abilities = 0;
    int tag = game_room_tag(&g);
    g.kills = 0;
    assert(game_enemy_speed(&g) == (tag == ROOM_TAG_FRENZY ? 2 : 1));
  }
  return 0;
}
