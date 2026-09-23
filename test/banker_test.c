#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  Game normal, banker, static_banker;
  game_init_seed(&normal, 7u);
  banker = static_banker = normal;
  banker.modules = static_banker.modules = MODULE_BANKER;
  static_banker.boss_rules = BOSS_STATIC;
  normal.credits = banker.credits = static_banker.credits = 20;
  assert(game_interest(&normal) == 3);
  assert(game_interest(&banker) == 4);
  assert(game_interest(&static_banker) == 3);
  banker.credits = 100;
  assert(game_interest(&banker) == 4);
  return 0;
}
