#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  Game g;
  game_init_seed(&g, 1);
  g.paused = g.upgrade_offer = true;
  g.credits = 0;
  game_choose_upgrade(&g, 1);
  assert(g.upgrade_offer && g.credits == 0);
  game_toggle_pause(&g);
  assert(g.paused);
  game_skip_upgrade(&g);
  assert(!g.upgrade_offer && !g.paused);

  g.upgrade_offer = g.paused = true;
  g.credits = 4;
  game_choose_upgrade(&g, 3);
  assert(g.upgrade_level == 1 && g.credits == 0);
  assert(g.upgrade_offer && g.paused);

  g.credits = 3;
  g.lives = 5;
  game_choose_upgrade(&g, 2);
  assert(g.credits == 3 && g.upgrade_offer);
  g.credits = 4;
  g.upgrade_level = 3;
  assert(!game_upgrade_available(&g, 3));
  game_choose_upgrade(&g, 3);
  assert(g.credits == 4 && g.upgrade_offer);

  game_init_seed(&g, 1);
  g.credits = 15;
  g.score = g.blind_target;
  game_room_progress(&g);
  assert(g.room == 2 && g.credits == 20);
  game_init_seed(&g, 9);
  g.upgrade_offer = g.paused = true;
  g.module_offer = MODULE_SIGNAL;
  g.credits = 2;
  assert(game_hold_available(&g));
  game_hold_module(&g);
  assert(g.held_module == MODULE_SIGNAL && g.module_offer == 0 && g.credits == 0);
  game_skip_upgrade(&g);
  g.score = g.blind_target;
  game_room_progress(&g);
  assert(g.module_offer == MODULE_SIGNAL && g.held_module == 0);
  g.credits = MODULE_SLOT_COST;
  assert(game_module_slot_available(&g));
  assert(game_module_slot_cost(&g) == MODULE_SLOT_COST);
  game_buy_module_slot(&g);
  assert(g.module_slots == MODULE_START_SLOTS + 1 &&
         g.credits == 0 && !game_module_slot_available(&g));
  g.credits = 8;
  assert(game_module_slot_cost(&g) == MODULE_SLOT_COST + 2);
  assert(game_module_count(&g) == 0);
  g.modules = MODULE_SIGNAL;
  g.credits = 0;
  assert(game_sell_module_available(&g, MODULE_SIGNAL));
  game_sell_module(&g, MODULE_SIGNAL);
  assert(!g.modules && g.credits == 2 && !game_sell_module_available(&g, MODULE_SIGNAL));
  return 0;
}
