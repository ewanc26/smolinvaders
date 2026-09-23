#include "space_invaders/game.h"

int game_room_tag(const Game *g) {
  return game_seed_hash(g->seed, 0x4000u + (uint32_t)g->room) % 3;
}

const char *game_room_tag_description(const Game *g) {
  switch (game_room_tag(g)) {
    case ROOM_TAG_FRENZY: return "FRENZY: enemy pressure +1";
    case ROOM_TAG_RICH: return "RICH: clear payout +1 credit";
    default: return "STANDARD BLIND";
  }
}
