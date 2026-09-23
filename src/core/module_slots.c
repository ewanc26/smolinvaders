#include "space_invaders/game.h"

static int module_count(const Game *g) {
  int count = 0;
  for (int bit = 1; bit <= MODULE_BARRIER; bit <<= 1)
    if (g->modules & bit) ++count;
  return count;
}

bool game_module_slot_available(const Game *g) {
  return g->upgrade_offer && g->module_slots < MODULE_MAX_SLOTS &&
         g->credits >= MODULE_SLOT_COST && !(g->shop_bought & (1 << 8));
}

void game_buy_module_slot(Game *g) {
  if (!game_module_slot_available(g)) return;
  g->credits -= MODULE_SLOT_COST;
  ++g->module_slots;
  g->shop_bought |= 1 << 8;
}

bool game_modules_full(const Game *g) {
  return module_count(g) >= g->module_slots;
}
