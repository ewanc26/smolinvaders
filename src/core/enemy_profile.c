#include "space_invaders/game.h"

int game_enemy_speed(const Game *g) {
  int speed = 1 + g->kills / 8;
  return speed > 3 ? 3 : speed;
}
