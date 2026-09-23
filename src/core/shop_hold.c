#include "space_invaders/game.h"

bool game_hold_available(const Game *g) {
  return g->upgrade_offer && g->module_offer && !g->held_module &&
         g->credits >= 2 && !(g->shop_bought & (1 << 7));
}

void game_hold_module(Game *g) {
  if (!game_hold_available(g)) return;
  g->credits -= 2;
  g->held_module = g->module_offer;
  g->module_offer = 0;
  g->shop_bought |= 1 << 7;
}
