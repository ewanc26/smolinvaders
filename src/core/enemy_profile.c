#include "space_invaders/game.h"

int game_enemy_speed(const Game *g) {
  int speed = 1 + g->kills / 8;
  if (game_room_tag(g) == ROOM_TAG_FRENZY) ++speed;
  if (g->enemy_abilities & ENEMY_HASTE) ++speed;
  return speed > 3 ? 3 : speed;
}
