#include "space_invaders/game.h"

bool game_is_boss(const Game *g) { return g->room % 3 == 0; }

void game_boss_prepare(Game *g) {
  g->boss_rules = 0;
  if (!game_is_boss(g)) return;
  /* A separate seed-derived draw makes boss identity independent of combat. */
  uint32_t draw = game_seed_hash(g->seed, (uint32_t)g->room * 3u + 2u);
  g->boss_rules = g->room == RUN_BLINDS ? 7 : 1 << (draw % 3);
  g->room_type = ROOM_ELITE;
  g->alien_hp = 2 + g->ante;
}

const char *game_boss_description(const Game *g) {
  switch (g->boss_rules) {
    case BOSS_STATIC: return "STATIC: scoring modules suppressed";
    case BOSS_BREACH: return "BREACH: enemy shots bypass shields";
    case BOSS_BLACKOUT: return "BLACKOUT: no signal saucers";
    case 7: return "ECLIPSE: no modules, cover or saucers";
    default: return "";
  }
}
