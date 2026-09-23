#include "space_invaders/game.h"

int game_interest(const Game *g) {
  int interest = g->credits / 5;
  return interest > 3 ? 3 : interest;
}
