#include "space_invaders/game.h"

int game_interest_cap(const Game *g) {
  int cap = (g->modules & MODULE_BANKER) &&
                    !(g->boss_rules & BOSS_STATIC) ? 4 : 3;
  if (cap == 4 && (g->modules & MODULE_BOUNTY)) cap = 5;
  return cap;
}

int game_interest(const Game *g) {
  int interest = g->credits / 5;
  int cap = game_interest_cap(g);
  return interest > cap ? cap : interest;
}
