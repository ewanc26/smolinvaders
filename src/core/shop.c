#include "space_invaders/game.h"

static bool damaged_shields(const Game *g) {
  for (int shield = 0; shield < SHIELD_COUNT; ++shield)
    for (int cell = 0; cell < SHIELD_WIDTH; ++cell)
      if (g->shields[shield][cell] < 3) return true;
  return false;
}

int game_upgrade_cost(int choice) {
  return choice == 1 ? 1 : choice == 2 ? 3 : choice == 3 ? 4 :
         choice == 4 ? 5 : choice == 6 ? 3 : -1;
}

bool game_upgrade_available(const Game *g, int choice) {
  int cost = game_upgrade_cost(choice);
  if (!g->upgrade_offer || g->over || g->won || cost < 0 ||
      g->credits < cost) return false;
  if (choice != 4 && (g->shop_bought & (1 << choice))) return false;
  return !(choice == 1 && !damaged_shields(g)) &&
         !(choice == 2 && g->lives >= 5) &&
         !(choice == 3 && g->upgrade_level >= 3) &&
         !(choice == 6 && g->emp_charges >= EMP_CAPACITY) &&
         !(choice == 4 && (!g->module_offer || (g->modules & g->module_offer)));
}

void game_choose_upgrade(Game *g, int choice) {
  if (!game_upgrade_available(g, choice)) return;
  g->credits -= game_upgrade_cost(choice);
  if (choice == 1) game_shields_init(g);
  if (choice == 2) ++g->lives;
  if (choice == 3) ++g->upgrade_level;
  if (choice == 6) ++g->emp_charges;
  if (choice != 4) g->shop_bought |= 1 << choice;
  if (choice == 4) {
    g->modules |= g->module_offer;
    g->module_offer = 0;
  }
}

void game_skip_upgrade(Game *g) {
  if (g->upgrade_offer) g->upgrade_offer = false, g->paused = false;
}
