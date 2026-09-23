#include "space_invaders/game.h"

int game_player_shot_speed(const Game *g) {
  return PLAYER_SHOT_SPEED + ((g->modules & MODULE_AFTERBURNER) != 0);
}
