#include "space_invaders/game.h"

int game_interest(const Game *g) {
  int interest = g->credits / 5;
  int cap = (g->modules & MODULE_BANKER) &&
                    !(g->boss_rules & BOSS_STATIC) ? 4 : 3;
  if (cap == 4 && (g->modules & MODULE_BOUNTY)) cap = 5;
  return interest > cap ? cap : interest;
}
