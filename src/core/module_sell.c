#include "space_invaders/game.h"

bool game_sell_module_available(const Game *g, int module) {
  return g->upgrade_offer && module >= MODULE_AMPLIFIER &&
         module <= MODULE_BARRIER && (g->modules & module) &&
         !(g->shop_bought & (1 << 9));
}

void game_sell_module(Game *g, int module) {
  if (!game_sell_module_available(g, module)) return;
  g->modules &= ~module;
  g->credits += 2;
  g->shop_bought |= 1 << 9;
}
