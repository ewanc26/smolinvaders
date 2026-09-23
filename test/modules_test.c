#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  for (uint32_t seed = 0; seed < 100; ++seed) {
    Game a, b;
    game_init_seed(&a, seed);
    game_init_seed(&b, seed);
    uint32_t combat_rng = a.rng;
    for (int i = 0; i < 4; ++i) {
      game_module_offer(&a);
      game_module_offer(&b);
      assert(a.module_offer == b.module_offer && a.module_offer);
      assert(!(a.modules & a.module_offer));
      a.upgrade_offer = a.paused = true;
      a.credits = 4;
      game_choose_upgrade(&a, 4);
      assert(a.credits == 4 && a.upgrade_offer);
      a.credits = 5;
      game_choose_upgrade(&a, 4);
      assert(a.credits == 0 && a.paused && a.upgrade_offer);
      b.modules = a.modules;
    }
    game_module_offer(&a);
    assert(a.module_offer == 0 && a.modules == MODULE_MASK && a.rng == combat_rng);
    a.upgrade_offer = true;
    a.credits = 100;
    assert(!game_upgrade_available(&a, 4));
    game_restart(&a);
    assert(a.seed == seed && !a.modules && !a.kills && !a.module_offer);
  }

  Game g;
  game_init(&g);
  g.modules = MODULE_AMPLIFIER | MODULE_SIGNAL | MODULE_CADENCE;
  g.kills = 2;
  g.bonus_active = true;
  g.bonus_x = 20;
  g.bullet_x = 21;
  g.bullet = 1;
  game_step(&g);
  assert(g.score == 12 && g.kills == 3); /* (3 + 1 + 2) * 2 */
  assert(g.room == 2 && g.blind_target == 17 && g.upgrade_offer);
  game_room_progress(&g);
  assert(g.room == 2);
  game_skip_upgrade(&g);
  g.alien_hp = 2;
  g.bullet_x = g.alien + 1;
  g.bullet = g.alien_row + 1;
  game_step(&g);
  assert(g.score == 12 && g.kills == 3); /* Armor hits are not kills. */
  g.bullet_x = g.alien + 1;
  g.bullet = g.alien_row + 1;
  game_step(&g);
  assert(g.score == 14 && g.kills == 4 && g.room == 2);
  return 0;
}
