#include "space_invaders/game.h"

bool game_choose_route(Game *g, int room_type) {
  if (!g->upgrade_offer || g->room % 3 == 0 ||
      room_type < ROOM_COMBAT || room_type > ROOM_CACHE) return false;
  g->room_type = room_type;
  g->alien_hp = room_type == ROOM_ELITE ? 3 : 1;
  return true;
}
