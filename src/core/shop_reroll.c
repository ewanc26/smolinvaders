#include "space_invaders/game.h"

int game_reroll_cost(const Game *g) {
  return 2 + (g->shop_rerolls < 8 ? g->shop_rerolls : 8);
}

bool game_reroll_available(const Game *g) {
  return g->upgrade_offer && !g->over && !g->won &&
         g->credits >= game_reroll_cost(g) &&
         (MODULE_MASK & ~(g->modules | g->module_offer)) != 0;
}

void game_reroll_shop(Game *g) {
  if (!game_reroll_available(g)) return;
  g->credits -= game_reroll_cost(g);
  if (g->shop_rerolls < 8) ++g->shop_rerolls;
  game_module_offer(g);
}
