#include "space_invaders/game.h"

int game_bonus_interval(const Game *g) {
  uint32_t draw = game_seed_hash(g->seed, 0x3000u + (uint32_t)g->room);
  int base = g->room_type == ROOM_CACHE ? 90 : 140;
  int spread = g->room_type == ROOM_CACHE ? 20 : 30;
  return base + (int)(draw % (uint32_t)spread);
}
