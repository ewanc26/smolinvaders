#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  for (uint32_t seed = 0; seed < 100; ++seed) {
    Game g, replay;
    game_init_seed(&g, seed);
    g.score = g.blind_target;
    game_room_progress(&g);
    g.credits = 70;
    g.module_slots = MODULE_MAX_SLOTS;
    replay = g;
    uint32_t combat_rng = g.rng;
    int first_offer = g.module_offer;
    game_reroll_shop(&g);
    game_reroll_shop(&replay);
    assert(g.module_offer != first_offer && g.module_offer != 0);
    assert(g.module_offer == replay.module_offer && g.shop_rng == replay.shop_rng);
    assert(g.credits == 68 && game_reroll_cost(&g) == 3);
    assert(g.rng == combat_rng && g.paused && g.upgrade_offer);

    assert(!game_upgrade_available(&g, 1)); /* Don't charge for intact cover. */
    g.shields[0][0] = 0;
    game_choose_upgrade(&g, 1);
    game_choose_upgrade(&g, 2);
    game_choose_upgrade(&g, 3);
    game_choose_upgrade(&g, 4);
    assert(g.credits == 55 && g.lives == 4 && g.upgrade_level == 1);
    assert(g.shields[0][0] == 3 && !g.module_offer);
    assert(g.paused && g.upgrade_offer);
    for (int choice = 1; choice <= 4; ++choice) {
      assert(!game_upgrade_available(&g, choice));
      game_choose_upgrade(&g, choice);
    }
    assert(g.credits == 55);
    int alien = g.alien, player = g.player;
    game_tick(&g, 1, true);
    game_step(&g);
    assert(g.alien == alien && g.player == player && g.bullet == -1);

    game_reroll_shop(&g);
    game_choose_upgrade(&g, 4);
    game_reroll_shop(&g);
    assert(g.module_offer && game_reroll_available(&g));
    int credits = g.credits;
    int cost = game_reroll_cost(&g);
    game_reroll_shop(&g);
    assert(g.credits == credits - cost);
    game_choose_upgrade(&g, 4);
    g.credits += 10; /* fund the sixth module introduced by Barrier */
    while (game_reroll_available(&g)) {
      game_reroll_shop(&g);
      game_choose_upgrade(&g, 4);
    }
    assert(game_module_count(&g) == MODULE_MAX_SLOTS && !game_upgrade_available(&g, 4));
    uint32_t shop_rng = g.shop_rng;
    game_skip_upgrade(&g);
    assert(!g.paused && !g.upgrade_offer);
    game_reroll_shop(&g);
    assert(g.shop_rng == shop_rng);
    g.score = g.blind_target;
    game_room_progress(&g);
    assert(g.shop_bought == 0 && g.shop_rerolls == 0);
    assert(g.module_offer && game_module_count(&g) == MODULE_MAX_SLOTS &&
           !game_upgrade_available(&g, 4) && game_reroll_cost(&g) == 2);
    game_restart(&g);
    assert(!g.modules && !g.shop_bought && !g.shop_rerolls && g.seed == seed);
  }
  Game poor;
  game_init(&poor);
  poor.score = poor.blind_target;
  game_room_progress(&poor);
  poor.credits = 1;
  uint32_t rng = poor.shop_rng;
  int offer = poor.module_offer;
  game_reroll_shop(&poor);
  assert(poor.credits == 1 && poor.shop_rng == rng && poor.module_offer == offer);
  poor.credits = 1000;
  for (int i = 0; i < 12; ++i) {
    int price = game_reroll_cost(&poor);
    int credits = poor.credits;
    game_reroll_shop(&poor);
    assert(poor.credits == credits - price && price <= 10);
  }
  assert(game_reroll_cost(&poor) == 10);
  poor.over = true;
  assert(!game_reroll_available(&poor) && !game_upgrade_available(&poor, 4));
  return 0;
}
