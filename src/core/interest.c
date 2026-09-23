#include "space_invaders/game.h"

int game_interest(const Game *g) {
  int interest = g->credits / 5;
  int cap = (g->modules & MODULE_BANKER) &&
                    !(g->boss_rules & BOSS_STATIC) ? 4 : 3;
  return interest > cap ? cap : interest;
}
