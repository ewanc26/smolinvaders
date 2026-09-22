#include "space_invaders/game.h"
#include <assert.h>

int main(void) {
  Game g;
  game_init_seed(&g, 42);
  game_fire(&g);
  int lane = g.bullet_x;
  game_move(&g, 1);
  game_step(&g);
  assert(g.bullet_x == lane);
  assert(g.player + 1 != lane);

  g.enemy_bullet = SHIELD_ROW - 1;
  g.enemy_bullet_x = 1;
  g.alien = 30;
  game_step(&g);
  assert(g.shields[0][0] == 2);

  game_toggle_pause(&g);
  int frozen = g.alien;
  game_step(&g);
  assert(g.alien == frozen);
  game_toggle_pause(&g);
  return 0;
}
